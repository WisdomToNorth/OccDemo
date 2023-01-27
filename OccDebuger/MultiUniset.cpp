#include "MultiUniset.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <thread>
#include <algorithm>

#include <qguiapplication.h>
#include <qscreen.h>
#include <qdebug.h>

#include "CADView.h"
#include "Ktimer.h"
#include "public.h"
#include "CustomQlistWidget.h"
#include "RobotLogger.h"
#include "global.h"

void MultiUniset::reGenerateData(int rowcnt, int colcnt, double dis)
{
    if (buf_.size() > 0)
    {
        std::vector<KBox> newbuf;
        buf_.swap(newbuf);
    }
    ConsoleLog("generating...");
    K_Timer timer;

    generateTestData(buf_, rowcnt, colcnt, dis);

    if (rowcnt * colcnt < 1000)
    {
        viewData();
    }
    ConsoleLog("generate data cost " + QString::number(timer.timeFromBegin(false)) + " ms.");
}

void MultiUniset::badWay()
{
    std::cout << "\n\n-----------origin method------------" << std::endl;
    int n = buf_.size();
    std::cout << "data size: " <<
        n << "\ncaculating..." << std::endl;

    int cnt = 0;
    K_Timer timer;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (!buf_[i].isOut(buf_[j]))
            {
                buf_[i].mergeTest(buf_[j]);

                ++cnt;
            }
        }
    }
    timer.timeFromBegin("Origin method");
    std::cout << "merge cnt : " << cnt << std::endl;
}

void MultiUniset::oneCoreUnionSet()
{
    std::cout << "\n\n-----------unionset single thread------------" << std::endl;
    unsigned long long data_count = buf_.size();
    std::cout << "data size: " << data_count << "\ncaculating..." << std::endl;
    K_Timer timer;
    UnionFind unionfinder(data_count);
    unsigned long long caculate_cnt = (data_count) * (data_count - 1) / 2;
    std::cout << "caculate_cnt: " << caculate_cnt << std::endl;
    caculateUnion(1, caculate_cnt + 1, unionfinder);
    unionfinder.update();

    timer.timeFromBegin("union single thread build");
    int cnt = handleUnionFinder(unionfinder, false);

    timer.timeFromBegin("union single all");
    std::cout << "merge count: " << cnt << std::endl;
}

void MultiUniset::multiCoreUnionSet(int user_set_num)
{

    std::cout << "\n\n-----------unionset multi thread------------" << std::endl;
    unsigned long long data_count = buf_.size();
    unsigned long long caculate_cnt = (data_count) * (data_count - 1) / 2;
    std::cout << "data size: " << data_count << "\ncaculating..." << std::endl;
    std::cout << "caculate cnt:" << caculate_cnt << std::endl;

    unsigned long long num_of_thread = getThreadCount(caculate_cnt, user_set_num);
    std::cout << "num of thread: " << num_of_thread << std::endl;
    unsigned long long block_size = caculate_cnt / num_of_thread;


    K_Timer timer;
    //������̵߳Ļ���Ҳ�����Ż�����ƽ������ֿ黮�֣�ʹ���Ӳ��鼯���غ��Ծ�����С
    //�˴�������x��y��Ϊż��������ǡ�ÿ��Ա��ĵȷ֡������̼߳���
    std::vector<std::thread> threads(num_of_thread - 1);
    std::vector<UnionFind> unionfinders(num_of_thread, UnionFind(data_count));

    unsigned long long l_start = 1;//��������������1��n��ʹ��β��index������n+1
    for (unsigned long long thread_index = 0; thread_index < num_of_thread - 1; ++thread_index)
    {
        unsigned long long l_end = l_start + block_size;
        //threads[thread_index] = std::thread(std::mem_fn(&MultiUniset::caculateUnion), this,
        //    l_start, l_end, std::ref(unionfinders[thread_index + 1]));
        threads[thread_index] = std::thread(std::mem_fn(&MultiUniset::caculateUnion), this,
            l_start, l_end, std::ref(unionfinders[thread_index + 1]));
        l_start = l_end;
    }
    this->caculateUnion(l_start, caculate_cnt + 1, std::ref(unionfinders[0]));
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    std::cout << "All thread join, begin to combine!" << std::endl;
    for (int numofunion = 1; numofunion < num_of_thread; ++numofunion)
    {
        unionfinders[0].merge(unionfinders[numofunion]);
    }
    unionfinders[0].update();

    timer.timeFromBegin("union mutil thread build");

    int cnt = handleUnionFinder(unionfinders[0], true);

    timer.timeFromBegin("union multi all");
    std::cout << "merge count: " << cnt << std::endl;
}

void MultiUniset::viewData()
{
    viewer_->removeAll();
    std::vector<TopoDS_Face> vecset;
    std::vector<Handle(AIS_TextLabel)> labs;
    drawData(buf_, vecset, labs);

    viewer_->drawTestData(vecset);
    viewer_->drawTestLabelData(labs);

    viewer_->fitAll();
    viewer_->update();
}

unsigned long long MultiUniset::getThreadCount(unsigned long long datasize, int defnum)
{
    if (defnum > 0)return defnum;

    unsigned long long const min_per_thread = 10;
    unsigned long long const max_thread = (datasize + min_per_thread - 1) / min_per_thread;
    unsigned long long const hardware_thread = std::thread::hardware_concurrency();
    unsigned long long const temp = hardware_thread != 0 ? hardware_thread : 2;
    unsigned long long const num_thrads = temp < max_thread ? temp : max_thread;

    return num_thrads;
}

// 1,
// 2, 3
// 4, 5, 6
// 7, 8, 9, 10
std::pair<int, int> MultiUniset::getLoc(unsigned long long num)
{
    unsigned long long testnum1 = std::floor(sqrt(2 * num));//6
    //std::cout << "\ntestnum: " << testnum1 << std::endl;
    unsigned long long testnum1_cache = testnum1 * testnum1;
    //if (!((testnum1 * (testnum1 - 1) < 2 * num) && (testnum1 * (testnum1 + 1) >= 2 * num)))
    if (!((testnum1_cache - testnum1 < 2 * num) && (testnum1_cache + testnum1 >= 2 * num)))
    {
        testnum1 += 1;
    }
    assert((testnum1 * (testnum1 - 1) < 2 * num) && (testnum1 * (testnum1 + 1) >= 2 * num));
    unsigned long long m = testnum1 - 1;
    unsigned long long n = num - testnum1 * (testnum1 - 1) / 2 - 1;
    int a = static_cast<int>(m) + 1;
    int b = static_cast<int>(n);

    //std::cout << "loc: " << a << " * " << b << " &" << std::endl;
    return { a,b };//m,n��0��ʼ��
}

/*
  0 1 2 3 4
0|
1|1
2|2 3
3|4 5 6
4|7 8 9 10
*/
void MultiUniset::caculateUnion(unsigned long long l_start,
    unsigned long long l_end, UnionFind& finder)
{
    unsigned long long cal_cnt = l_end - l_start;
    std::pair<int, int> loc = getLoc(l_start);
    int cal_real_cnt = 0;
    int m = loc.first, n = loc.second;
    //std::cout << "\nloc :" << m << "#" << n << "#" << cal_cnt << "//" << std::endl;
    for (int i = m; i < buf_.size(); ++i)//
    {
        for (int j = 0; j < m; ++j)
        {
            j = j + n;//��һ�ν���ѭ��ʱ����ʼ��j��λ�ã�������n����
            n = 0;
            // std::cout << "cal:" << i << " and " << j << std::endl;

            if (!buf_[i].isOut(buf_[j]))
            {
                finder.merge(i, j);
            }

            cal_real_cnt++;
            if (cal_cnt == cal_real_cnt)
            {
                //std::cout << "cal_real_cnt: " << cal_real_cnt << std::endl;
                return;
            }
        };
        m++;
    }
    //std::cout << "cal_real_cnt: " << cal_real_cnt << std::endl;
    assert(cal_cnt == cal_real_cnt);

}

template <typename Iterator>
int MultiUniset::handleUnionSetResult(Iterator first, Iterator last, const int index)
{
    int cnt = 0;
    unsigned long const length = std::distance(first, last);
    if (!length)
    {
        std::string info = std::to_string(index) + " CHECK: dis is 0\n";
        return 0;
    }

    Iterator block_start = first;

    while (block_start != last)
    {
        if ((*block_start).second.size() == 1)
        {
            ++block_start;
            continue;
        }
        const std::unordered_set<int>& cur = ((*block_start).second);
        std::vector<KBox> curset;

        //std::string info = std::to_string(block_start - first) + ":{";
        for (const auto& num : cur)
        {
            //info = info + ' ' + std::to_string(num) + ' ';

            curset.emplace_back(buf_[num]);
        }
        // info += "}\n";
        //std::cout << info;

        curset[0].mergeTest(curset);
        ++cnt;
        ++block_start;
    }

    return cnt;
};

int MultiUniset::handleUnionFinder(const UnionFind& finder, bool use_multi)
{
    //int cnt = 0;
    if (!use_multi)
    {
        return handleUnionSetResult(finder.final_set_.begin(), finder.final_set_.end(), 0);
    }
    else
    {
        const std::vector<std::pair<int, std::unordered_set<int>>>& final_set
            = finder.final_set_;

        unsigned long long caculate_cnt = final_set.size();
        std::cout << "\nfinal set size: " << final_set.size() << std::endl;
        unsigned long long num_of_thread = getThreadCount(caculate_cnt, 0);//TODO:int user_set_num
        std::cout << "num of thread: " << num_of_thread << std::endl;
        unsigned long long block_size = caculate_cnt / num_of_thread;
        std::vector<std::thread> threads(num_of_thread - 1);
        std::vector<int> threads_res(num_of_thread - 1);

        auto l_start = final_set.begin();
        int cnt = 0;
        for (unsigned long long thread_index = 0; thread_index < num_of_thread - 1; ++thread_index)
        {
            auto l_end = l_start + block_size;

            threads[thread_index] = std::thread([=, &cnt]//�漰��������ò����Ҫʹ�ø�ֵ
                {
                    cnt += handleUnionSetResult(l_start, l_end, thread_index);
                });

            l_start = l_end;
        }
        cnt += handleUnionSetResult(l_start, finder.final_set_.end(), num_of_thread);

        std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

        return cnt;
    }
}


void MultiUniset::generateTestData(std::vector<KBox>& buffer,
    int _row_size, int _col_size, double distance)
{
    if (_col_size == 0)_col_size = _row_size;


    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1);
    std::uniform_real_distribution<double> v(0.6, 0.95);
    e.seed(1);
    for (int i = 0; i < _col_size; ++i)
    {
        for (int j = 0; j < _row_size; ++j)
        {
            double loc_random = v(e);
            double stx = i * distance * loc_random;
            double sty = j * distance * loc_random;
            double x_size = sizeu(e);
            double y_size = x_size;
            KBox l_box(stx, sty, x_size, y_size);
            buffer.emplace_back(l_box);
        }
    }
    ConsoleLog("Size: " + QString::number(_row_size * distance) + " * " +
        QString::number(_col_size * distance));
}