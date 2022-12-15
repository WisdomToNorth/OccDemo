#include "MainWindowOcc.h"

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
#include "data.h"
#include "global.h"

MainWindowOcc::MainWindowOcc(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowOccClass())
{
    ui->setupUi(this);
    auto con = getUIConfig();
    this->setStyleSheet(G_GetUiStyleSheet(con));


    viewer_ = new OccView(this);

    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();
    auto scwidth = width * 4 / 5;
    auto scheight = height * 4 / 5;
    this->setGeometry(width * 1 / 10, height / 10, scwidth, scheight);

    row_spin_ = new QSpinBox();
    col_spin_ = new QSpinBox();
    thread_spin_ = new QSpinBox();
    distance_spin_ = new QDoubleSpinBox();
    QLabel* label_row = new QLabel(" row");
    QLabel* label_col = new QLabel(" col");
    QLabel* label_thread = new QLabel(" thread");
    QLabel* label_dis = new QLabel(" distance");
    ui->mainToolBar->addWidget(label_row);
    ui->mainToolBar->addWidget(row_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_col);
    ui->mainToolBar->addWidget(col_spin_);
    ui->mainToolBar->addWidget(label_thread);
    ui->mainToolBar->addWidget(thread_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_dis);
    ui->mainToolBar->addWidget(distance_spin_);
    row_spin_->setAlignment(Qt::AlignRight);
    col_spin_->setAlignment(Qt::AlignRight);
    thread_spin_->setAlignment(Qt::AlignRight);
    distance_spin_->setAlignment(Qt::AlignRight);

    row_spin_->setMaximum(300);
    col_spin_->setMaximum(300);
    thread_spin_->setMinimum(0);
    thread_spin_->setMaximum(64);
    thread_spin_->setValue(0);
    row_spin_->setValue(300);
    col_spin_->setValue(300);
    distance_spin_->setValue(1.5);
    ui->gridLayout->addWidget(viewer_);
    on_actionGenerate_triggered();
}

MainWindowOcc::~MainWindowOcc()
{
    delete ui;
}

void MainWindowOcc::on_actionGenerate_triggered()
{
    if (buf_.size() > 0)
    {
        std::vector<KBox> newbuf;
        buf_.swap(newbuf);
    }
    std::cout << "\n\n-------------\n-------------\ngenerating...";
    K_Timer timer;
    int rowcnt = row_spin_->value();
    int colcnt = col_spin_->value();
    double dis = distance_spin_->value();
    generateTestData(buf_, rowcnt, colcnt, dis);
    if (rowcnt * colcnt < 1000)
    {
        on_actionview_triggered();
    }
    timer.timeFromBegin("generate data");
}

void MainWindowOcc::on_actionOri_triggered()
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
                //std::cout << "{ " << thread_index << "  " << j << " }\caculate_cnt";
                ++cnt;
            }
        }
    }
    timer.timeFromBegin("Origin method");
    std::cout << "merge cnt : " << cnt << std::endl;
}

void MainWindowOcc::on_actionopt1_triggered()
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
    int cnt = handleUnionFinder(unionfinder);

    timer.timeFromBegin("union single all");
    std::cout << "merge count: " << cnt << std::endl;
}

unsigned long long MainWindowOcc::getThreadCount(unsigned long long datasize)
{
    if (thread_spin_->value() > 0)
    {
        return thread_spin_->value();
    }
    unsigned long long const min_per_thread = 25;
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
std::pair<int, int> MainWindowOcc::getLoc(unsigned long long num)
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
    return { a,b };//m,n从0开始数
}

/*
  0 1 2 3 4
0|
1|1
2|2 3
3|4 5 6
4|7 8 9 10
*/
void MainWindowOcc::caculateUnion(unsigned long long l_start,
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
            j = j + n;//第一次进入循环时，初始化j的位置，后续将n置零
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

void MainWindowOcc::on_actionopt2_triggered()
{
    std::cout << "\n\n-----------unionset multi thread------------" << std::endl;
    unsigned long long data_count = buf_.size();
    unsigned long long caculate_cnt = (data_count) * (data_count - 1) / 2;
    std::cout << "data size: " << data_count << "\ncaculating..." << std::endl;
    std::cout << "caculate cnt:" << caculate_cnt << std::endl;

    unsigned long long num_of_thread = getThreadCount(caculate_cnt);
    std::cout << "num of thread: " << num_of_thread << std::endl;
    unsigned long long block_size = caculate_cnt / num_of_thread;


    K_Timer timer;
    //这里多线程的划分也可以优化，按平面区域分块划分，使各子并查集的重合性尽可能小
    //此处，假设x、y都为偶数，这样恰好可以被四等分。分四线程计算
    std::vector<std::thread> threads(num_of_thread - 1);
    std::vector<UnionFind> unionfinders(num_of_thread, UnionFind(data_count));

    unsigned long long l_start = 1;//代表任务数，从1到n，使用尾后index，所以n+1
    for (unsigned long long thread_index = 0; thread_index < num_of_thread - 1; ++thread_index)
    {
        unsigned long long l_end = l_start + block_size;
        //threads[thread_index] = std::thread(std::mem_fn(&MainWindowOcc::caculateUnion), this,
        //    l_start, l_end, std::ref(unionfinders[thread_index + 1]));  
        threads[thread_index] = std::thread(std::mem_fn(&MainWindowOcc::caculateUnion), this,
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

    int cnt = handleUnionFinder(unionfinders[0]);

    timer.timeFromBegin("union multi all");
    std::cout << "merge count: " << cnt << std::endl;

}

int MainWindowOcc::handleUnionFinder(const UnionFind& finder)
{
    int cnt = 0;
    for (auto it = finder.final_set_.begin();
        it != finder.final_set_.end(); ++it)
    {
        if ((*it).second.size() == 1)continue;
        std::unordered_set<int> cur((*it).second);
        std::vector<KBox> curset;
        // std::cout << '{';
        for (const auto& num : cur)
        {
            //  std::cout << ' ' << num << ' ';
            curset.emplace_back(buf_[num]);
        }
        //std::cout << "}\caculate_cnt";
        curset[0].mergeTest(curset);
        ++cnt;
    }
    return cnt;
}

void MainWindowOcc::on_actionFitAll_triggered()
{
    viewer_->fitAll();
}

void MainWindowOcc::on_actionview_triggered()
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

void MainWindowOcc::generateTestData(std::vector<KBox>& buffer,
    int testrow, int testcol, double distance)
{
    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1);
    std::uniform_real_distribution<double> v(0.6, 0.95);
    e.seed(1);
    for (int i = 0; i < testcol; ++i)
    {
        for (int j = 0; j < testrow; ++j)
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
}