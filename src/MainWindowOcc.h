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
class TwoDSearch;
class StatusInfoWidget;
class LineDrawer;
class PrePline;
class MainWindowOcc : public QMainWindow
{
    Q_OBJECT
private:
    enum class CmdEnum;
    enum class AppModeEnum;
public:
    MainWindowOcc(QWidget* parent = nullptr);
    ~MainWindowOcc();

private:
    void setUpUI();
    void setStatusBar(const double& _1, const double& _2, const double& _3);
    void handleMouseMove(const double& _1, const double& _2, const double& _3);
    void handleLeftPress(const double& _1, const double& _2);
    void handleRightPress(QMouseEvent* event);
    QMenu* getRightMenu();
    void execCmd(CmdEnum _cmd);

private slots:
    //data

    void on_pb_generate_pressed();
    void on_pb_valueMax_pressed();
    void on_pb_valueMax_unsafe_pressed();
    void on_pb_RandRange_pressed();
    void on_pb_Randvalue_pressed();
    void on_pb_appenddata_pressed();

    void on_actionFitAll_triggered();
    void on_actionview_triggered();
    void on_actionendCPU_triggered();
    //unionfind
    void on_act_unionfind_ori_triggered();
    void on_act_unionfind_opt1_triggered();
    void on_act_unionfind_opt2_triggered();

    //kdtree
    void on_pb_Test1DFind_pressed();
    void on_pb_Test2DFind_pressed();
    void on_pb_Simple1DFind_pressed();
    void on_pb_Simple2DFind_pressed();

    void on_actionkd_find1D_triggered();
    void on_actionori_find1D_triggered();
    void on_actionkd_find2D_triggered();
    void on_actionori_find2D_triggered();
    void on_actionran_find2D_triggered();

    //viewdate
    void on_action_drawline_triggered();
    void on_action_cpuline_triggered();
    void on_action_normline_triggered();

private:
    int kd_find1D(bool _debug = false);
    int ori_find1D(bool _debug = false);
    int kd_find2D(bool _debug = false);
    int ori_find2D(bool _debug = false);
    int ran_find2D(bool _debug = false);

private:
    Ui::MainWindowOccClass* ui;

    CadView* cadview_;

    DataGenerator* data_generator_;
    MultiUniset* unionset_;
    TwoDSearch* kdtree_;
    StatusInfoWidget* info_widget_;
    LineDrawer* line_drawer_ = nullptr;

    AppModeEnum curmode_ = AppModeEnum::none;

    std::vector<PrePline*> preline_vec_;
    enum class CmdEnum
    {
        commit_draw,
        cancel_draw
    };
    enum class AppModeEnum
    {
        none,
        draw_line
    };
};
}