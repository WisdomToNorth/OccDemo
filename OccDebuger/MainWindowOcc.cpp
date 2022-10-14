#include "MainWindowOcc.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>

#include <qguiapplication.h>
#include <qscreen.h>
#include <qdebug.h>

#include "CADView.h"
#include "Ktimer.h"
#include "public.h"
#include "data.h"
#include "unionset.h"

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
    distance_spin_ = new QSpinBox();
    QLabel* label_row = new QLabel("row");
    QLabel* label_col = new QLabel("col");
    QLabel* label_dis = new QLabel("distance");
    ui->mainToolBar->addWidget(label_row);
    ui->mainToolBar->addWidget(row_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_col);
    ui->mainToolBar->addWidget(col_spin_);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(label_dis);
    ui->mainToolBar->addWidget(distance_spin_);
    row_spin_->setAlignment(Qt::AlignRight);
    col_spin_->setAlignment(Qt::AlignRight);
    distance_spin_->setAlignment(Qt::AlignRight);
    row_spin_->setValue(20);
    col_spin_->setValue(20);

    distance_spin_->setValue(2);
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
                //std::cout << "{ " << i << "  " << j << " }\n";
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
    int n = buf_.size();
    std::cout << "data size: " <<
        n << "\ncaculating..." << std::endl;
    K_Timer timer;
    UnionFind unionfinder(n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (!buf_[i].isOut(buf_[j]))
            {
                unionfinder.merge(i, j);
            }
        }
    }
    unionfinder.update();

    int cnt = 0;
    for (auto it = unionfinder.final_set_.begin(); it != unionfinder.final_set_.end(); ++it)
    {
        if ((*it).second.size() == 1)continue;
        std::unordered_set<int> cur((*it).second);
        std::vector<KBox> curset;
        std::cout << '{';
        for (auto& num : cur)
        {
            std::cout << ' ' << num << ' ';
            curset.push_back(buf_[num]);
        }
        std::cout << "}\n";
        curset[0].mergeTest(curset);
        ++cnt;
    }
    timer.timeFromBegin("union single thread");
    std::cout << "merge count: " << cnt << std::endl;
}

void MainWindowOcc::on_actionopt2_triggered()
{

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