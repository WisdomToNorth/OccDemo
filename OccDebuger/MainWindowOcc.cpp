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
using std::vector;

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
    ui->gridLayout->addWidget(mview);
    

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
    K_Timer ti;
    qDebug() << "[generating...]";
    int rowcnt = 200;
    int colcnt = rowcnt;
    DataUtil::generateTestData(buf_, rowcnt, colcnt,2);
    if (rowcnt * colcnt < 1000)
    {
        on_actionview_triggered();
    }
    std::cout<<"generate time: "<<ti.timeFromBegin(false)<<"ms"<<std::endl;
}

void MainWindowOcc::on_actionOri_triggered()
{
    int n = buf_.size();
    std::cout << "\n##########worst##########\n" << "data size: " <<
        n << "\n\n[caculating...]" << std::endl;
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
                //qDebug() << i << " " << j;
            }
        }
    }
    std::cout << "caculate cnt: " << cnt << std::endl;
    ticker.timeFromBegin();
}

void MainWindowOcc::on_actionopt1_triggered()
{
    int n = buf_.size();
    std::cout << "\n##########worst##########\n" << "data size: " <<
        n << "\n\n[caculating..]" << std::endl;
    K_Timer ticker;
    int cnt = 0;
    int cntisout = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (!buf_[i].isOut(buf_[j]))
            {
                ++cnt;
                buf_[i].mergeTest(buf_[j]);
            }
        }
    }
    std::cout << "caculate cnt: " << cnt << std::endl;
    ticker.timeFromBegin();
}

void MainWindowOcc::on_actionopt2_triggered()
{
    int cnt = buf_.size();
    UnionFind finder;
    finder.init(cnt);
    std::cout << "\n##########opt2##########\n" << "data size: " <<
        cnt << "\n\n[caculating...]" << std::endl;
    K_Timer ticker;
    for (int i = 0; i < cnt; ++i)
    {
        for (int j = i + 1; j < cnt; ++j)
        {
            if (!buf_[i].isOut(buf_[j]))
            {
                finder.merge(i,j);
            }
        }
    }
    std::cout << "union init done: " << ticker.timeFromLastSee(false)<<std::endl;

    std::unordered_map<int, int> aux;
    int unionsize = 0;
    for (int i = 0; i < cnt; ++i)
    {
        if (!aux.count(finder.find(i)))
        {
            unionsize++;
            aux[finder.find(i)] = unionsize;
        }
    }
    
    vector<vector<KBox>> l_merge_mark_vec(unionsize + 1, vector<KBox>());
    for (int i = 0; i < cnt; ++i)
    {
        l_merge_mark_vec[aux[finder.find(i)]].emplace_back(buf_[i]);
    }
    if (cnt < 1000)
    {
        vector<vector<int>> checkvec(unionsize + 1, vector<int>());
        for (int i = 0; i < cnt; ++i)
        {
            checkvec[aux[finder.find(i)]].emplace_back(i);
        }
        DataUtil::printvecvec(checkvec);
    }
    std::cout << "build map done: " << ticker.timeFromLastSee(false) << std::endl;
    std::cout << "classfied size: " << unionsize <<"\n\n[Start merge...]"<< std::endl;

    int merge_cnt = 0;
    for (auto& vec : l_merge_mark_vec)
    {
        int cursize = vec.size();
        for (int i = 0; i < cursize; ++i)
        {
            for (int j = i + 1; j < cursize; ++j)
            {
                ++merge_cnt;
                vec[i].mergeTest(vec[j]);
                //std::cout << "merge " << i << " and " << j << std::endl;
            }
        }
        //std::cout << std::endl;
    }
    std::cout << "caculate cnt: " << merge_cnt << std::endl;
    std::cout << "merge time: " << ticker.timeFromLastSee(false) << std::endl;
    ticker.timeFromBegin();
   
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
