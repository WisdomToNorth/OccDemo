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
        int testrow, int testcol = 0, int distance = 3);
    void caculateUnion(unsigned long long l_start, unsigned long long l_end, UnionFind& finder);
    static std::pair<int, int> getLoc(unsigned long long num);
    unsigned long long getThreadCount(unsigned long long datasize);

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
