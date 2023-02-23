#pragma once
#ifndef _IWLOG_H_
#define _IWLOG_H_
//system
#include <stdio.h>
#include <string>
#include <iostream>
//Qt
#include <QString>

class iwLog
{
public:

    //! 析构函数
    virtual ~iwLog() {}

    //! 返回唯一静态实例
    static iwLog* instance();

    //! 注册唯一实例
    static void registerInstance(iwLog* logInstance);

    //! 开启消息备份系统
    /** 存储消息，直到注册了有效的日志记录实例
    **/
    static void enableMessageBackup(bool state);

    //! 消息等级
    enum MessageLevelFlags
    {
        LOG_STANDARD = 0, /**< Standard message (Print) **/
        LOG_WARNING = 1, /**< Warning message (Warning) **/
        LOG_ERROR = 2, /**< Error message (Error) **/
    };

    //! iwLog::logMessage 的静态快捷方式
    static void LogMessage(const QString& message, int level);

    //! 通用消息记录方法
    /** 由子类实现
        \warning 必须是线程安全的！
        \param message 消息
        \param level 消息等级（请参阅 MessageLevelFlags）
    **/
    virtual void logMessage(const QString& message, int level) = 0;

    //! 在控制台打印出格式化的消息
    /** 与 printf 命令的效果相同
        \return 总是返回 true
    **/
    static bool print(const char* format, ...);

    //! QString版本的 iwLog::Print
    inline static bool print(const QString& message)
    {
        LogMessage(message, LOG_STANDARD);
        return true;
    }

    //! 在控制台中打印出格式化的警告消息
    /** 与 printf 命令的效果相同
        \return 总是返回 false
    **/
    static bool warning(const char* format, ...);

    //! QString版本的 iwLog::Warning
    inline static bool warning(const QString& message)
    {
        LogMessage(message, LOG_WARNING); return false;
    }

    //! 显示带有格式化消息的错误对话框
    /** 与 printf 命令的效果相同
        \return 总是返回 false
    **/
    static bool error(const char* format, ...);

    //! QString版本的 iwLog::Error
    inline static bool error(const QString& message)
    {
        LogMessage(message, LOG_ERROR); return false;
    }
};

#endif