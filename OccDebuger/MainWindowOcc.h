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

class MainWindowOcc : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowOcc(QWidget* parent = nullptr);
    ~MainWindowOcc();

private:

private slots:
    //data
    void on_actionGenerate_triggered();
    void on_actionFitAll_triggered();
    void on_actionview_triggered();

    //unionfind
    void on_actionOri_triggered();
    void on_actionopt1_triggered();
    void on_actionopt2_triggered();

    //kdtree
    void on_actionkd_find1D_triggered();

private:
    Ui::MainWindowOccClass* ui;
    CadView* viewer_;
    QSpinBox* row_spin_;
    QSpinBox* col_spin_;
    QSpinBox* thread_spin_;
    QDoubleSpinBox* distance_spin_;

    DataGenerator* datar_;
    MultiUniset* unionset_;
};
}