/****************************************************************************
** Copyright 2022 by KangYucheng.
** All Rights Reserved.
**
** This file is part of RobotConfig software. No part of this file may be
** reproduced in any form or means, without the prior written consent of KangYucheng.
****************************************************************************/

#pragma once
#ifndef _CADVIEW_H_
#define _CADVIEW_H_

#include <functional>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QWidget>

#ifdef _WIN32
#include <QtOpenGL\QGLWidget>
#include <WNT_Window.hxx>
#else
#undef None
#include <QGLWidget>
#include <Xw_Window.hxx>
#endif

#include <AIS_InteractiveObject.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_ViewController.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Prs3d_TypeOfHighlight.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include "KContext.h"

class QMenu;
class QRubberBand;
class AIS_Shape;
class AIS_InteractiveContext;
class QMouseEvent;
class QWheelEvent;
class V3d_View;
class V3d_Viewer;
class AIS_InteractiveObject;
class Graphic3d_GraphicDriver;
class KContext;
enum Prs3d_TypeOfHighlight;

namespace KDebugger
{
//! Adapted a QWidget for OpenCASCADE viewer.
class CadView : public QWidget, protected AIS_ViewController
{
private:
    enum class CurrentAction3dEnum
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning,
        CurAction3d_DynamicZooming,
        CurAction3d_DynamicRotation
    };

    typedef std::function<void(const Handle(SelectMgr_EntityOwner) &)> processClickCallBack;
    typedef std::function<void(QMouseEvent *event)> rightButtonClickCallBack;
    typedef std::function<void(const double &_1, const double &_2)> leftButtonClickCallBack;
    typedef std::function<void(const double &_1, const double &_2, const double &_3)>
        moveInfoCallBack;

public:
    enum class KUpdate
    {
        Redraw,
        ZFitAll,
        Fitall,
        MustResized
    };
    enum class CursorType
    {
        def,
        hand,
        pan,
        glob,
        zoom,
        rot,
        wait
    };
    void setUserCursor(CursorType type);
    CadView(QWidget *parent = Q_NULLPTR);
    ~CadView();
    bool busy_ = false;
    KContext *getContext()
    {
        return context_.get();
    };
    Handle(SelectMgr_EntityOwner) getDetectedObj();
    void updateView(KUpdate);
    void removeAll(bool redraw = true);

    processClickCallBack processClickCb = NULL;
    moveInfoCallBack moveInfoCb = NULL;
    leftButtonClickCallBack leftClickCb = NULL;
    rightButtonClickCallBack rightClickCb = NULL;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QPaintEngine *paintEngine() const; // 初始化界面相关

private:
    void initContext();
    void initHilightAttributes();
    void initContextAttributes();
    void setHilightAttribute(Prs3d_TypeOfHighlight idx, double lineWidth_aspect = 5.0,
                             Quantity_Color theColor = Quantity_NOC_LIGHTBLUE2);
    void setViewCube();
    bool checkDetectedValid();

private:
    friend class CursorHelper;

    Handle(KContext) context_ = nullptr;
    Handle(AIS_InteractiveObject) viewcube_ = nullptr;
    Handle(V3d_Viewer) v_viewer_ = nullptr;
    Handle(Graphic3d_GraphicDriver) graphic_driver_ = nullptr;
    Handle(V3d_View) view_ = nullptr;

    CurrentAction3dEnum context_action_mode_;                 //! 三维场景转换模式
    std::vector<Handle(SelectMgr_EntityOwner)> detected_obj_; // 存储点击过的对象，可能是点线面

    int mouse_x_record_;                                      //! 记录鼠标平移坐标X
    int mouse_y_record_;                                      //! 记录鼠标平移坐标Y
    int zoom_scale_control_ = 4;
    CursorType cursor_type_ = CursorType::def;
};

} // namespace KDebugger
#endif // _OCCVIEW_H_
