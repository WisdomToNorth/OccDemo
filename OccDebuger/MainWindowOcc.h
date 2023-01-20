#pragma once

#include <vector>
#include <QMainWindow>
#include <qspinbox.h>
#include <qlabel.h>

#include "data.h"
#include "unionset.h"

#include "ui_MainWindowOcc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowOccClass; };
QT_END_NAMESPACE

class OccView;
class MyThread;
class MainWindowOcc : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowOcc(QWidget* parent = nullptr);
    ~MainWindowOcc();

private:
    void generateTestData(std::vector<KBox>& buffer,
        int testrow, int testcol, double distance);

    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    static std::pair<int, int> getLoc(unsigned long long num);
    unsigned long long getThreadCount(unsigned long long datasize);
    int handleUnionFinder(const UnionFind& finder, bool use_multi);


    template <typename Iterator>
    int cal_set_fuc(Iterator first, Iterator last, const int index)
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

private slots:
    void on_actionGenerate_triggered();
    void on_actionOri_triggered();
    void on_actionopt1_triggered();
    void on_actionopt2_triggered();
    void on_actionFitAll_triggered();
    void on_actionview_triggered();

private:
    Ui::MainWindowOccClass* ui;
    OccView* viewer_;
    QSpinBox* row_spin_;
    QSpinBox* col_spin_;
    QSpinBox* thread_spin_;
    QDoubleSpinBox* distance_spin_;

    std::vector<KBox> buf_;
};
