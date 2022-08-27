#pragma once

#include <vector>
#include <QMainWindow>
#include "data.h"

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
    MainWindowOcc(QWidget *parent = nullptr);
    ~MainWindowOcc();

private slots:
    void on_actionGenerate_triggered();
    void on_actionOri_triggered();
    void on_actionopt1_triggered();
    void on_actionopt2_triggered();
    void on_actionFitAll_triggered();
    void on_actionview_triggered();
private:
    Ui::MainWindowOccClass *ui;
    OccView* mview;

    std::vector<KBox> buf_;
};
