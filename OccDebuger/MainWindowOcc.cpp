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

MainWindowOcc::MainWindowOcc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowOccClass())
{
    ui->setupUi(this);
    mview = new OccView(this);

    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();
    auto scwidth = width * 4 / 5;
    auto scheight = height * 4 / 5;
    this->setGeometry(width * 1 / 10, height / 10, scwidth, scheight);

    setCentralWidget(mview);

}

MainWindowOcc::~MainWindowOcc()
{
    delete mview;
    mview = nullptr;
    delete ui;

}
void MainWindowOcc::on_actionGenerate_triggered()
{
    if (buf_.size() > 0)return;
    qDebug() << "generating...";
    int rowcnt = 4;
    int colcnt = 4;
    DataUtil::generateTestData(buf_, rowcnt, colcnt,2);
    if (rowcnt * colcnt < 1000)
    {
        on_actionview_triggered();
    }
    qDebug() << "generate done!";
}

void MainWindowOcc::on_actionOri_triggered()
{

    int n = buf_.size();
    std::cout << "data size: " <<
        n << "\ncaculating..." << std::endl;
    K_Timer ticker;
    int cnt = 0;
    int cntisout = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i+1; j < n; ++j)
        {
            buf_[i].mergeTest(buf_[j]);
            ++cnt;
            if (!buf_[i].isOut(buf_[j]))
            {
                qDebug() << i << " " << j;
            }
        }
    }
    std::cout << "caculate cnt: " << cnt << std::endl;
    ticker.timeFromBegin();
}

void MainWindowOcc::on_actionopt1_triggered()
{

}

void MainWindowOcc::on_actionopt2_triggered()
{

}
void MainWindowOcc::on_actionFitAll_triggered()
{

}
void MainWindowOcc::on_actionview_triggered()
{
    std::vector<TopoDS_Face> vecset;
    std::vector<Handle(AIS_TextLabel)> labs;
    DataUtil::drawData(buf_, vecset, labs);
    mview->drawtestdata(vecset, labs);
}
