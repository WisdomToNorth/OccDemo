#include "iwLog.h"

// System
#include <assert.h>
#include <vector>

//! 缓冲区 用于格式化字符串生成

static const size_t s_bufferMaxSize = 4096;
static char s_buffer[s_bufferMaxSize];

//! Message
struct Message
{
    Message(const QString &t, int f) :
        text(t), flags(f)
    {
    }

    QString text;
    int flags;
};

//! message backup system
static bool s_backupEnabled;
//! backed up messages
static std::vector<Message> s_backupMessages;

//! 唯一控制台实例
static iwLog *s_instance = nullptr;

iwLog *iwLog::instance()
{
    return s_instance;
}

void iwLog::enableMessageBackup(bool state)
{
    s_backupEnabled = state;
}
void iwLog::LogMessage(const std::string &message, int level)
{
    LogMessage(QString::fromStdString(message), level);
}

void iwLog::LogMessage(const QString &message, int level)
{
    if (s_instance)
    {
        s_instance->logMessage(message, level);
    }
    else if (s_backupEnabled)
    {
        try
        {
            s_backupMessages.emplace_back(message, level);
        } catch (const std::bad_alloc &)
        {
            // 消息丢失
        }
    }
}

void iwLog::registerInstance(iwLog *logInstance)
{
    s_instance = logInstance;
    if (s_instance)
    {
        // 如果有一个有效的实例，我们现在可以刷新备份的消息
        for (const Message &message : s_backupMessages)
        {
            s_instance->logMessage(message.text, message.flags);
        }
        s_backupMessages.clear();
    }
}

// 将格式化数据转换为 QString 以便调用 iwLog::logMessage
// 支持 printf 方式输出
#define LOG_ARGS(flags)                                     \
    if (s_instance || s_backupEnabled)                      \
    {                                                       \
        va_list args;                                       \
        va_start(args, format);                             \
        vsnprintf(s_buffer, s_bufferMaxSize, format, args); \
        va_end(args);                                       \
        LogMessage(QString(s_buffer), flags);               \
    }

bool iwLog::print(const char *format, ...)
{
    LOG_ARGS(LOG_STANDARD)
    return true;
}

bool iwLog::warning(const char *format, ...)
{
    LOG_ARGS(LOG_WARNING)
    return false;
}

bool iwLog::error(const char *format, ...)
{
    LOG_ARGS(LOG_ERROR)
    return false;
}
