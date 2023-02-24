#pragma once

#include <vector>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowOccClass; };
QT_END_NAMESPACE

class QSpinBox;
class QDoubleSpinBox;

namespace KDebugger
{
class CadView;
class MultiUniset;
class DataGenerator;
class KDTree;
class MainWindowOcc : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowOcc(QWidget* parent = nullptr);
    ~MainWindowOcc();

private:
    void setUpUI();

private slots:
    //data
    void on_pb_generate_pressed();
    void on_pb_valueMax_pressed();
    void on_pb_valueSmall_pressed();

    void on_actionFitAll_triggered();
    void on_actionview_triggered();

    //unionfind
    void on_act_unionfind_ori_triggered();
    void on_act_unionfind_opt1_triggered();
    void on_act_unionfind_opt2_triggered();

    //kdtree
    void on_actionkd_find1D_triggered();
    void on_actionori_find1D_triggered();
    void on_actionkd_find2D_triggered();
    void on_actionori_find2D_triggered();
private:
    Ui::MainWindowOccClass* ui;

    CadView* viewer_;

    DataGenerator* data_generator_;
    MultiUniset* unionset_;
    KDTree* kdtree_;

};
}