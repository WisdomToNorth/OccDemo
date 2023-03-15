#pragma once
#ifndef _IWCONSOLE_H_
#define _IWCONSOLE_H_
//Local
#include "iwLog.h"

//Qt
#include <QFile>
#include <QListWidget>
#include <QMutex>
#include <QTimer>

class QTextStream;

class QMainWindow;

//! Console
class iwConsole : public QObject, public iwLog
{
    Q_OBJECT

public:

    //! Destructor
    ~iwConsole() override;

    //! 初始化控制台（并可选将其与文本输出部件相关联）
    /** 警告: 警告：在发布模式下，如果没有定义 textDisplay 控件，则不会输出任何消息。
        此外，如果定义了 parentWidget 控件，错误消息只会出现在（阻塞）QMessageBox 中。
        \param textDisplay 文本输出控件（可选）
        \param parentWidget 父控件（可选）
        \param parentWindow 父窗口（可选）
        \param silentCommandLineMode 将日志消息转到 printf（可选）
    **/
    static void init(QListWidget* textDisplay = nullptr,
        QWidget* parentWidget = nullptr,
        QMainWindow* parentWindow = nullptr,
        bool redirectToStdOut = false);

    //! 返回唯一静态句柄
    /** \param autoInit 自动初始化控制台实例（没有小部件！） 如果尚未完成
    **/
    static iwConsole* handle(bool autoInit = true);

    //! 释放唯一句柄
    static void releaseInstance(bool flush = true);

    //! 设置自动刷新
    void setAutoRefresh(bool state);

    //! 设置日志文件
    bool setLogFile(const QString& filename);

    //! 是否使用在控制台使用 Qt 信息 (qDebug / qWarning / etc.)
    static void EnableQtMessages(bool state);

    //! 返回是否在控制台中显示 Qt 消息
    static bool QtMessagesEnabled() { return s_showQtMessagesInConsole; }

    //! 返回父控件
    inline QWidget* parentWidget() { return parent_widget_; }

public:

    //! 刷新控制台（显示仍在队列中的所有消息）
    void refresh();

protected:

    //! 默认构造
    /** 构造函数受到保护，以避免将此对象用作非静态类
    **/
    iwConsole();

    //! 继承自 iwLog
    void logMessage(const QString& message, int level) override;

    //! 关联的文本显示控件
    QListWidget* text_display_;

    //! 父控件
    QWidget* parent_widget_;

    //! 父窗口
    QMainWindow* parent_window_;

    //! 控制台的并发线程访问的互斥体
    QMutex mutex_;

    //! 队列元素类型（消息 + 颜色）
    using ConsoleItemType = QPair<QString, int>;

    //! 消息的队列
    QVector<ConsoleItemType> queue_;

    //! 自动刷新时间
    QTimer timer_;

    //! 日志文件
    QFile log_file_;

    //! 日志文件流
    QTextStream* log_stream_;

    //! 是否在控制台中显示 Qt 消息
    static bool s_showQtMessagesInConsole;
    static bool s_redirectToStdOut;
};

#endif