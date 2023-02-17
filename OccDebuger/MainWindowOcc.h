#pragma once

#include <vector>
#include <QMainWindow>
#include <qspinbox.h>
#include <qlabel.h>

#include "MultiUniset.h"

#include "ui_MainWindowOcc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowOccClass; };
QT_END_NAMESPACE


class MyThread;
class MultiUniset;
class MainWindowOcc : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowOcc(QWidget* parent = nullptr);
    ~MainWindowOcc();

private:

private slots:
    void on_actionGenerate_triggered();
    void on_actionOri_triggered();
    void on_actionopt1_triggered();
    void on_actionopt2_triggered();
    void on_actionFitAll_triggered();
    void on_actionview_triggered();

private:
    Ui::MainWindowOccClass* ui;
    CadView* viewer_;
    QSpinBox* row_spin_;
    QSpinBox* col_spin_;
    QSpinBox* thread_spin_;
    QDoubleSpinBox* distance_spin_;

    MultiUniset* unionset_;
};
