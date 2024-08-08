#pragma once
#ifndef _CUSTOM_LISTVIEW_H_
#define _CUSTOM_LISTVIEW_H_
#include <QListWidget>

//! 自定义 QListWidget 以允许使用 CTRL+C 复制所有选定元素
class iwCustomQListWidget : public QListWidget
{
    Q_OBJECT

public:
    iwCustomQListWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif