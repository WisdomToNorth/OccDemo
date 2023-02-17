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

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>


#ifdef _WIN32
#include <WNT_Window.hxx>
#include <QtOpenGL\QGLWidget>
#else
#undef None
#include <Xw_Window.hxx>
#include <QGLWidget>
#endif

//#include <Standard_Handle.hxx>
//#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ViewController.hxx>
#include <Prs3d_TypeOfHighlight.hxx>
#include <TopoDS_Face.hxx>
#include <V3d_Viewer.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <AIS_TextLabel.hxx>
// class AIS_InteractiveContext;
// class AIS_InteractiveObject;
#include <V3d_View.hxx>


class QMenu;


class ViewModel;

class CadView : public QWidget, protected AIS_ViewController
{
    Q_OBJECT
private:
    enum class CurrentAction3dEnum
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning,
        CurAction3d_DynamicZooming,
        CurAction3d_DynamicRotation
    };


    typedef std::function<void(const Handle(SelectMgr_EntityOwner)&)> leftButtonClickCallBack;
    typedef std::function<void(QMouseEvent* event)> rightButtonClickCallBack;

public:
    enum class KUpdate
    {
        Redraw, ZFitAll, Fitall, MustResized
    };

    CadView(QWidget* parent = Q_NULLPTR);
    ~CadView();
    Handle(AIS_InteractiveContext) context_;
    Handle(SelectMgr_EntityOwner) getDetectedObj();
    void drawTestData(const std::vector<TopoDS_Face>& all_face_);
    void drawTestLabelData(const std::vector<Handle(AIS_TextLabel)>& all_labels);
    leftButtonClickCallBack processClickCb = NULL;
    rightButtonClickCallBack rightClickCb = NULL;
    void fitAll();

public slots:
    void updateView(KUpdate);
    //void execCmd(CmdEnum _cmd);
    void resetAll(bool);

protected:
    void paintEvent(QPaintEvent*)override;
    void resizeEvent(QResizeEvent*)override;
    void mousePressEvent(QMouseEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* event)override;
    void wheelEvent(QWheelEvent* event)override;
    void keyPressEvent(QKeyEvent* event)override;
    QPaintEngine* paintEngine() const;//初始化界面相关

private:
    void initContext();
    void initDefaultHilightAttributes(Prs3d_TypeOfHighlight idx,
        Standard_Real lineWidth_aspect = 5.0, Quantity_Color theColor = Quantity_NOC_LIGHTBLUE2);
    void initCursors();
    void setViewCube();
    bool checkDetectedValid();

signals:
    void cmdSignal(QMouseEvent* event);

private:

    Handle(AIS_InteractiveObject) viewcube_ = nullptr;
    Handle(V3d_Viewer) v_viewer_ = nullptr;
    Handle(Graphic3d_GraphicDriver) graphic_driver_ = nullptr;
    Handle(V3d_View) view_ = nullptr;

    CurrentAction3dEnum context_action_mode_;//!三维场景转换模式
    std::vector<Handle(SelectMgr_EntityOwner)>detected_obj_;//存储点击过的对象，可能是点线面

    int mouse_x_record_;    //!记录鼠标平移坐标X
    int mouse_y_record_;    //!记录鼠标平移坐标Y
    int zoom_scale_control_ = 4;
};

#endif