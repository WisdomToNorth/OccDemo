#include "global.h"
#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <thread>
#include <algorithm>
#include <functional>
#include <QFile>
#include <QJsonObject>
#include <QDir>
#include <QJsonParseError>
#include <QApplication>

namespace KDebugger
{
Handle(AIS_InteractiveContext) G_Context = nullptr;

unsigned long long G_CoreCount(unsigned long long datasize)
{
    unsigned long long const min_per_thread = 25;
    unsigned long long const max_thread = (datasize + min_per_thread - 1) / min_per_thread;
    unsigned long long const hardware_thread = std::thread::hardware_concurrency();
    unsigned long long const temp = hardware_thread != 0 ? hardware_thread : 2;
    unsigned long long const num_thrads = temp < max_thread ? temp : max_thread;

    return num_thrads;
}

bool makeDirAndFile(const QString path, bool isFile)
{
    QDir dir(path);
    if (dir.exists(path)) return true;
    bool succ = makeDirAndFile(path.mid(0, path.lastIndexOf('/')), false);
    if (!succ) return false;

    if (isFile)
    {
        QFile fp(path);
        succ = fp.open(QIODevice::ReadWrite | QIODevice::Text);
    }
    else
    {
        succ = dir.mkpath(path);
    }

    if (!succ) return false;
    return true;
}

QString loadQString(const std::string& path)
{
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly))
    {
        if (!file.exists())
        {
            std::cout << path << " is not exist";
            return QString();
        }
    }

    QByteArray allData = file.readAll();
    file.close();
    return QString(allData);// .toStdString();
}

QString G_GetAppPath()
{
    return QApplication::applicationDirPath();
}

QString G_GetDocumentsPath()
{
    QString _path = G_GetAppPath() + "/Files/";
    return _path;
}

QJsonObject loadJson(const std::string& path)
{
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly))
    {
        if (!file.exists())
        {
            return QJsonObject();
        }
    }

    QByteArray allData = file.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));

    if (jsonError.error == QJsonParseError::NoError)
    {
        return jsonDoc.object();
    }
    else
    {
        std::cout << "json error!" << jsonError.errorString().toStdString();
        return QJsonObject();
    }
}

Appearance G_UI_Appearance = Appearance::Dark;

Appearance getUIConfig()
{
    QString ui_json_path = G_GetDocumentsPath() + "/uiconfig.json";

    QJsonObject appinfo = loadJson(ui_json_path.toStdString());
    if (appinfo.contains("ui"))
    {
        QString ui_str = appinfo["ui"].toString();

        if (ui_str == "day")
            G_UI_Appearance = Appearance::Day;
        else if (ui_str == "system")//todo
            G_UI_Appearance = Appearance::System;
        else //if (ui_str == "dark")
            G_UI_Appearance = Appearance::Dark;
    }

    return G_UI_Appearance;
}

QString G_GetUiStyleSheet(Appearance preset)
{

    QString ui_day_qss_path = G_GetDocumentsPath() + "/daymode.qss";
    QString dayset = loadQString(ui_day_qss_path.toStdString());

    switch (preset)
    {
    case Appearance::Dark:
    {
        std::cout << "Use Dark appearance." << std::endl;
        QString ui_dark_qss_path = G_GetDocumentsPath() + "/darkmode.qss";
        QString darkset = loadQString(ui_dark_qss_path.toStdString());
        return darkset;
    }
    case Appearance::Day:
    {
        std::cout << "Use Day appearance." << std::endl;
        return dayset;
    }
    case Appearance::System:
    {
        std::cout << "Use System appearance." << std::endl;//todo
        return dayset;
    }
    default:
    {
        std::cout << "Invalid setting. Use Day appearance." << std::endl;
        return dayset;
    }
    }
}
}