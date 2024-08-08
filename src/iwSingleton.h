/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#pragma once
#ifndef _IWSINGLETON_H_
#define _IWSINGLETON_H_

// 泛型单例封装结构体
template <class T>
struct iwSingleton
{
    // 构造函数
    iwSingleton() :
        handle(nullptr)
    {}
    // 析构函数
    ~iwSingleton()
    {
        release();
    }
    // 释放当前实例
    inline void release()
    {
        if (handle)
        {
            delete handle;
            handle = nullptr;
        }
    }

    // 当前句柄
    T *handle;
};

#endif