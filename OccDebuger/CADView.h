#pragma once
#ifndef _CADVIEW_H_
#define _CADVIEW_H_

#include <functional>

#include <QWidget>

#ifdef _WIN32
#include <WNT_Window.hxx>
#include <QtOpenGL\QGLWidget>
#else
#undef None
#include <Xw_Window.hxx>
#include <QGLWidget>
#endif

#include <AIS_ViewController.hxx>

class QMenu;
class QRubberBand;
class AIS_Shape;
class AIS_InteractiveContext;
class QMouseEvent;
class QWheelEvent;
class AIS_TextLabel;
class V3d_View;
class V3d_Viewer;
class AIS_InteractiveObject;
class Graphic3d_GraphicDriver;
class AIS_TextLabel;
enum Prs3d_TypeOfHighlight;

namespace KDebugger
{
//! Adapted a QWidget for OpenCASCADE viewer.

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


    //typedef std::function<void(const Handle(SelectMgr_EntityOwner)&)> leftButtonClickCallBack;
    typedef std::function<void(QMouseEvent* event)> rightButtonClickCallBack;
    typedef std::function<void(const double& _1, const double& _2)> leftButtonClickCallBack;
    typedef std::function<void(const double& _1, const double& _2, const double& _3)> moveInfoCallBack;


public:
    enum class KUpdate
    {
        Redraw, ZFitAll, Fitall, MustResized
    };

    CadView(QWidget* parent = Q_NULLPTR);
    ~CadView();

    Handle(SelectMgr_EntityOwner) getDetectedObj();
    void drawTestData(const std::vector<Handle(AIS_Shape)>& all_face_);
    void drawTestLabelData(const std::vector<Handle(AIS_TextLabel)>& all_labels);
    void fitAll();
    void removeAll();
    Handle(AIS_InteractiveContext) getContext() { return context_; }
    leftButtonClickCallBack leftClickCb = NULL;
    rightButtonClickCallBack rightClickCb = NULL;
    moveInfoCallBack moveInfoCb = NULL;

public slots:
    void updateView(KUpdate);
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
    void setOriginTrihedron();
    bool checkDetectedValid();

signals:
    void cmdSignal(QMouseEvent* event);

private:
    friend class DataGenerator;
    Handle(AIS_InteractiveContext) context_;
    Handle(AIS_InteractiveObject) viewcube_ = nullptr;
    Handle(AIS_InteractiveObject) aisOriginTrihedron_ = nullptr;
    Handle(V3d_Viewer) viewer_ = nullptr;
    Handle(Graphic3d_GraphicDriver) graphic_driver_ = nullptr;
    Handle(V3d_View) view_ = nullptr;
    Handle(AIS_TextLabel) text_ = nullptr;

    CurrentAction3dEnum context_action_mode_;//!三维场景转换模式
    std::vector<Handle(SelectMgr_EntityOwner)>detected_obj_;//存储点击过的对象，可能是点线面

    int mouse_x_record_;    //!记录鼠标平移坐标X
    int mouse_y_record_;    //!记录鼠标平移坐标Y
    int zoom_scale_control_ = 4;
};
}
#endif // _OCCVIEW_H_
