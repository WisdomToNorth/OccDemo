#include "iwConsole.h"

//Local
#include "MainwindowOcc.h"

//LAB_Camera
#include "iwSingleton.h"

//Qt

#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QThread>
#include <QTime>


//! 唯一的控制台句柄
static iwSingleton<iwConsole>* s_console = new iwSingleton<iwConsole>();

bool iwConsole::s_showQtMessagesInConsole = true;
bool iwConsole::s_redirectToStdOut = true;

// iwConsole
iwConsole* iwConsole::handle(bool autoInit/*=true*/)
{
    if (!s_console->handle && autoInit)
    {
        s_console->handle = new iwConsole();
        iwLog::registerInstance(s_console->handle);
    }

    return s_console->handle;
}

void iwConsole::releaseInstance(bool flush/*=true*/)
{
    if (flush && s_console->handle)
    {
        //DGM: just in case some messages are still in the queue
        s_console->handle->refresh();
    }
    iwLog::registerInstance(nullptr);
    s_console->release();
}

iwConsole::iwConsole()
    : text_display_(nullptr)
    , parent_widget_(nullptr)
    , parent_window_(nullptr)
    , log_stream_(nullptr)
{}

iwConsole::~iwConsole()
{
    setLogFile(QString()); //to close/delete any active stream
}

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
#ifndef QT_DEBUG
    if (!iwConsole::QtMessagesEnabled())
    {
        return;
    }

    if (type == QtDebugMsg)
    {
        return;
    }
#endif

    QString message = QString("[%1] ").arg(context.function) + msg; // QString("%1 (%1:%1, %1)").arg(msg).arg(context.file).arg(context.line).arg(context.function);

    //in this function, you can write the message to any stream!
    switch (type)
    {
    case QtDebugMsg:
        //iwLog::PrintDebug(msg);
        break;
    case QtWarningMsg:
        message.prepend("[Qt WARNING] ");
        iwLog::warning(message);
        break;
    case QtCriticalMsg:
        message.prepend("[Qt CRITICAL] ");
        iwLog::warning(message);
        break;
    case QtFatalMsg:
        message.prepend("[Qt FATAL] ");
        iwLog::warning(message);
        break;
    case QtInfoMsg:
        message.prepend("[Qt INFO] ");
        iwLog::warning(message);
        break;
    }
}

void iwConsole::EnableQtMessages(bool state)
{
    s_showQtMessagesInConsole = state;

    //save to persistent settings
    //QSettings settings;
    //settings.beginGroup(QStringLiteral("Console"));
    //settings.setValue("QtMessagesEnabled", s_showQtMessagesInConsole);
    //settings.endGroup();
}

void iwConsole::init(QListWidget* textDisplay/*=0*/,
    QWidget* parentWidget/*=0*/,
    QMainWindow* parentWindow/*=0*/,
    bool redirectToStdOut/*=false*/)
{
    // 只能调用一次
    if (s_console->handle)
    {
        return;
    }

    s_console->handle = new iwConsole();
    s_console->handle->text_display_ = textDisplay;
    s_console->handle->parent_widget_ = parentWidget;
    s_console->handle->parent_window_ = parentWindow;
    s_redirectToStdOut = redirectToStdOut;
    //auto-start
    if (textDisplay)
    {
        //load from persistent settings
        QSettings settings;
        //settings.beginGroup(ccPS::Console());
        //s_showQtMessagesInConsole = settings.value("QtMessagesEnabled", false).toBool();
        //settings.endGroup();

        //install : set the callback for Qt messages
        qInstallMessageHandler(myMessageOutput);

        s_console->handle->setAutoRefresh(true);
    }
    iwLog::registerInstance(s_console->handle);
}

void iwConsole::setAutoRefresh(bool state)
{
    if (state)
    {
        connect(&timer_, &QTimer::timeout, this, &iwConsole::refresh);
        timer_.start(1000);
    }
    else
    {
        timer_.stop();
        disconnect(&timer_, &QTimer::timeout, this, &iwConsole::refresh);
    }
}

void iwConsole::refresh()
{
    mutex_.lock();

    if ((text_display_ || log_stream_) && !queue_.isEmpty())
    {
        for (QVector<ConsoleItemType>::const_iterator it = queue_.constBegin();
            it != queue_.constEnd(); ++it)
        {
            //destination: log file
            if (log_stream_)
            {
                *log_stream_ << it->first << Qt::endl;
            }

            //destination: console widget
            if (text_display_)
            {
                //it->first = message text
                QListWidgetItem* item = new QListWidgetItem(it->first);

                // 根据消息严重性设置颜色
                // Error
                if (it->second & LOG_ERROR)
                {
                    item->setForeground(Qt::red);
                }
                // Warning
                else if (it->second & LOG_WARNING)
                {
                    item->setForeground(Qt::darkRed);
                    // 若收到警告消息，强制控制台可见性！
                    if (parent_window_)
                    {
                        //parent_window_->forceConsoleDisplay();
                    }
                }

                text_display_->addItem(item);
            }
        }

        if (log_stream_)
            log_file_.flush();

        if (text_display_)
            text_display_->scrollToBottom();
    }

    queue_.clear();

    mutex_.unlock();
}

void iwConsole::logMessage(const QString& message, int level)
{
    QString formatedMessage = QStringLiteral("[")
        + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        + QStringLiteral("] ")
        + message;
    if (s_redirectToStdOut)
    {
        printf("%s\n", qPrintable(message));
    }
    if (text_display_ || log_stream_)
    {
        mutex_.lock();
        queue_.push_back(ConsoleItemType(formatedMessage, level));
        mutex_.unlock();
    }

    // 弹出对话框显示错误消息
    if ((level & LOG_ERROR)
        && qApp
        && parent_widget_
        && QThread::currentThread() == qApp->thread()
        )
    {
        QMessageBox::warning(parent_widget_, "Error", message);
    }
}

bool iwConsole::setLogFile(const QString& filename)
{
    // 关闭之前的文件流
    if (log_stream_)
    {
        mutex_.lock();
        delete log_stream_;
        log_stream_ = nullptr;
        mutex_.unlock();

        if (log_file_.isOpen())
        {
            log_file_.close();
        }
    }

    if (!filename.isEmpty())
    {
        log_file_.setFileName(filename);
        if (!log_file_.open(QFile::Text | QFile::WriteOnly))
        {
            return error(QString("[Console] Failed to open/create log file '%1'").arg(filename));
        }

        mutex_.lock();
        log_stream_ = new QTextStream(&log_file_);
        mutex_.unlock();
        setAutoRefresh(true);
    }

    return true;
}
