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


MainWindowOcc::MainWindowOcc(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowOccClass())
{
    ui->setupUi(this);
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

    row_spin_->setMaximum(1000);
    col_spin_->setMaximum(1000);
    thread_spin_->setMinimum(0);
    thread_spin_->setMaximum(64);
    thread_spin_->setValue(0);
    row_spin_->setValue(30);
    col_spin_->setValue(30);

    distance_spin_->setValue(1.5);
    ui->gridLayout->addWidget(viewer_);

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
    qDebug() << "\n\n-------------\n\
-------------\ngenerating...";
    K_Timer timer;
    int rowcnt = row_spin_->value();
    int colcnt = col_spin_->value();
    int dis = distance_spin_->value();
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
                //std::cout << "{ " << thread_index << "  " << j << " }\n";
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
    unsigned long n = buf_.size();
    std::cout << "data size: " << n << "\ncaculating..." << std::endl;
    K_Timer timer;
    UnionFind unionfinder(n);
    unsigned long caculatecnt= (n) * (n - 1) / 2;
    caculateUnion(1, caculatecnt+1, unionfinder);
    unionfinder.update();

    timer.timeFromBegin("union single thread build");
    int cnt = 0;
    for (auto it = unionfinder.final_set_.begin(); it != unionfinder.final_set_.end(); ++it)
    {
        if ((*it).second.size() == 1)continue;
        std::unordered_set<int> cur((*it).second);
        std::vector<KBox> curset;
        //std::cout << '{';
        for (auto& num : cur)
        {
            // std::cout << ' ' << num << ' ';
            curset.push_back(buf_[num]);
        }
        //std::cout << "}\n";
        curset[0].mergeTest(curset);
        ++cnt;
    }
    timer.timeFromBegin("union single all");
    std::cout << "merge count: " << cnt << std::endl;
}

unsigned long MainWindowOcc::getThreadCount(unsigned long datasize)
{
    if (thread_spin_->value() > 0)
    {
        return thread_spin_->value();
    }
    unsigned long const min_per_thread = 25;
    unsigned long const max_thread = (datasize + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_thread = std::thread::hardware_concurrency();
    unsigned long const temp = hardware_thread != 0 ? hardware_thread : 2;
    unsigned long const num_thrads = temp < max_thread ? temp : max_thread;

    return num_thrads;
}

// 1,
// 2, 3
// 4, 5, 6
// 7, 8, 9, 10
std::pair<int, int> MainWindowOcc::getLoc(unsigned long num)
{
    int m = 0, n = 0;
    unsigned long testnum1 = std::floor(pow(2 * num, 0.5));//6
    if (!((testnum1 * (testnum1 - 1) < 2 * num) && (testnum1 * (testnum1 + 1) >= 2 * num)))
    {
        testnum1 = std::ceil(pow(2 * num, 0.5));
        //assert((testnum1 * (testnum1 - 1) < 2 * num) && (testnum1 * (testnum1 + 1) > 2 * num));
    }
    m = testnum1 - 1;
    n = num - testnum1 * (testnum1 - 1) / 2 - 1;
    return { m,n };//m,n从0开始数
}

/*
  0 1 2 3 4
0|
1|1
2|2 3
3|4 5 6
4|7 8 9 10
*/
void MainWindowOcc::caculateUnion(unsigned long l_start, unsigned long l_end, UnionFind& finder)
{
    unsigned long cal_cnt = l_end - l_start;
    std::pair<int, int> loc = getLoc(l_start);

    int m = loc.first, n = loc.second;
    //std::cout << "\n\n\nloc :" << m << "#" << n << "#" << cal_cnt << ' ' << std::endl;
    for (int i = m + 1; i < buf_.size(); ++i)//
    {
        for (int j = 0; j < m + 1; ++j)
        {
            j = j + n;//第一次进入循环时，初始化j的位置，后续将n置零
            n = 0;
            // std::cout << "cal:" << i << " and " << j << std::endl;
            if (!buf_[i].isOut(buf_[j]))
            {
                finder.merge(i, j);
            }
            cal_cnt -= 1;
            if (cal_cnt == 0)
            {
                return;
            }
        };
        m++;
    }
    assert(cal_cnt == 0);

}

void MainWindowOcc::on_actionopt2_triggered()
{
    std::cout << "\n\n-----------unionset multi thread------------" << std::endl;
    unsigned long temp = buf_.size();
    unsigned long n = (temp) * (temp - 1) / 2;
    std::cout << "data size: " << temp << "\ncaculating..." << std::endl;

    unsigned long num_of_thread = getThreadCount(n);
    unsigned long block_size = n / num_of_thread;
    std::cout << "num of thread: " << num_of_thread << std::endl;
    K_Timer timer;
    //这里多线程的划分也可以优化，按平面区域分块划分，使各子并查集的重合性尽可能小
    //此处，假设x、y都为偶数，这样恰好可以被四等分。分四线程计算
    std::vector<std::thread> threads(num_of_thread - 1);
    std::vector<UnionFind> unionfinders(num_of_thread, UnionFind(temp));

    int l_start = 1;
    for (int thread_index = 0; thread_index < num_of_thread - 1; ++thread_index)
    {
        int l_end = l_start + block_size;
        threads[thread_index] = std::thread(std::mem_fn(&MainWindowOcc::caculateUnion), this,
            l_start, l_end, std::ref(unionfinders[thread_index + 1]));
        l_start = l_end;
    }
    this->caculateUnion(l_start, n + 1, std::ref(unionfinders[0]));
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    std::cout << "All thread join, begin to combine!" << std::endl;
    for (int numofunion = 1; numofunion < num_of_thread; ++numofunion)
    {
        unionfinders[0].merge(unionfinders[numofunion]);
    }
    unionfinders[0].update();

    timer.timeFromBegin("union mutil thread build");
    int cnt = 0;
    for (auto it = unionfinders[0].final_set_.begin(); it != unionfinders[0].final_set_.end(); ++it)
    {
        if ((*it).second.size() == 1)continue;
        std::unordered_set<int> cur((*it).second);
        std::vector<KBox> curset;
        // std::cout << '{';
        for (auto& num : cur)
        {
            //  std::cout << ' ' << num << ' ';
            curset.push_back(buf_[num]);
        }
        //std::cout << "}\n";
        curset[0].mergeTest(curset);
        ++cnt;
    }
    timer.timeFromBegin("union multi all");
    std::cout << "merge count: " << cnt << std::endl;

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
    viewer_->drawtestdata(vecset, labs);
}

void MainWindowOcc::generateTestData(std::vector<KBox>& buffer,
    int testrow, int testcol, int distance)
{
    if (testcol == 0)testcol = testrow;

    std::default_random_engine e;
    std::uniform_real_distribution<double> sizeu(0.8, 1);
    std::uniform_real_distribution<double> v(0.5, 1);
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