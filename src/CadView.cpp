#include "CadView.h"

#include <AIS_InteractiveContext.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_Handle.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include <QAction>
#include <QApplication>
#include <QGuiApplication>
#include <QMenuBar>
#include <QTimer>
#include <qclipboard.h>
#include <qcursor.h>
#include <qdebug.h>
#include <qinputdialog.h>
#include <qmutex.h>
#include <qpointer.h>
#include <qscreen.h>

#include "KLogger.h"
#include "shapetools.h"
#include "stadfx.h"

namespace KDebugger
{
CadView::CadView(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // 窗口将通过鼠标点击、按tab键等方式获取焦点
    setBackgroundRole(QPalette::NoRole);
    setMouseTracking(true);
    // set focus policy to threat QContextMenuEvent from keyboard
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    initContext();
}

CadView::~CadView()
{
}

QCursor getCursor(CadView::CursorType type)
{
    switch (type)
    {
    case CadView::CursorType::def: return QCursor(Qt::ArrowCursor);
    case CadView::CursorType::hand: return QCursor(Qt::PointingHandCursor);
    case CadView::CursorType::pan: return QCursor(Qt::SizeAllCursor);
    case CadView::CursorType::glob: return QCursor(Qt::CrossCursor);
    case CadView::CursorType::zoom:
        return QCursor(Qt::SizeHorCursor /*QPixmap(":/cursor_zoom.png").scaled(40, 40)*/);
    case CadView::CursorType::rot: return QCursor(Qt::SizeFDiagCursor);
    case CadView::CursorType::wait: return QCursor(Qt::CursorShape::ForbiddenCursor);
    default: return QCursor(Qt::ArrowCursor);
    }
}

void CadView::setUserCursor(CursorType type)
{
    auto cursor = getCursor(type);
    switch (type)
    {
    case CursorType::def: this->setCursor(cursor); break;
    case CursorType::hand: this->setCursor(cursor); break;
    case CursorType::pan: this->setCursor(cursor); break;
    case CursorType::glob: this->setCursor(cursor); break;
    case CursorType::zoom: this->setCursor(cursor); break;
    case CursorType::rot: this->setCursor(cursor); break;
    case CursorType::wait: this->setCursor(cursor); break;
    default: break;
    }
}

void CadView::initContext()
{
    // 获取主屏幕分辨率
    auto screenRect = QGuiApplication::screens();
    auto width = screenRect[0]->geometry().width();
    auto height = screenRect[0]->geometry().height();

    this->setGeometry(width / 5, height / 5, width * 3 / 5, height * 3 / 5); // 设置窗口默认大小

    if (context_.IsNull())
    {
        Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();
        // 创建OpenGL图形驱动
        if (graphic_driver_.IsNull())
        {
            graphic_driver_ = new OpenGl_GraphicDriver(m_display_donnection);
        }
        // 获取QWidget的窗口系统标识符
        WId window_handle = (WId)winId();

#ifdef _WIN32
        // 创建Windows NT 窗口
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
#else
        Handle(Xw_Window) wind = new Xw_Window(m_display_donnection, (Aspect_Handle)window_handle);
#endif

        // 创建3D查看器
        v_viewer_ = new V3d_Viewer(graphic_driver_);
        // 创建视图
        view_ = v_viewer_->CreateView();
        view_->SetWindow(wind);
        // 打开窗口
        if (!wind->IsMapped())
        {
            wind->Map();
        }
        // 配置查看器的光照
        v_viewer_->SetDefaultLights();
        v_viewer_->SetLightOn();

        // 显示直角坐标系，可以配置在窗口显示位置、文字颜色、大小、样式
        view_->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

        // 渐变色背景
        Quantity_Color top_color =
            Quantity_Color(101 / 255.0, 101 / 255.0, 122 / 255.0, Quantity_TOC_RGB);
        Quantity_Color bottom_color =
            Quantity_Color(191 / 255.0, 193 / 255.0, 204 / 255.0, Quantity_TOC_RGB);
        view_->SetBgGradientColors(top_color, bottom_color, Aspect_GFM_VER);

        context_ = new KContext(v_viewer_); // 创建交互式上下文
        // 设置显示模式
        initHilightAttributes();
        initContextAttributes();

        setBackgroundRole(QPalette::NoRole); // 无背景

        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_NoSystemBackground);
        setMouseTracking(true); // 开启鼠标位置追踪
    }
    setViewCube();

    view_->Redraw();
    view_->MustBeResized();
};

void CadView::setViewCube()
{
    auto ais_viewcube = new AIS_ViewCube();
    double transp_num = 0.55; // 透明程度，1为完全透明
    ais_viewcube->SetBoxColor(Quantity_NOC_GRAY2);
    // ais_viewcube->SetFixedAnimationLoop(false);
    ais_viewcube->SetSize(62);
    ais_viewcube->SetFontHeight(14);
    ais_viewcube->SetBoxTransparency(transp_num);

    ais_viewcube->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers, Aspect_TOTP_LEFT_UPPER, Graphic3d_Vec2i(85, 85)));

    const Handle_Prs3d_DatumAspect datum_color = new Prs3d_DatumAspect();
    datum_color->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_GREEN2);
    datum_color->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_RED2);
    datum_color->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    datum_color->ShadingAspect(Prs3d_DP_XAxis)->SetTransparency(transp_num);
    datum_color->ShadingAspect(Prs3d_DP_YAxis)->SetTransparency(transp_num);
    datum_color->ShadingAspect(Prs3d_DP_ZAxis)->SetTransparency(transp_num);
    ais_viewcube->Attributes()->SetDatumAspect(datum_color);

    viewcube_ = ais_viewcube;

    context_->Display(viewcube_, false); // 显示模型
}

QPaintEngine *CadView::paintEngine() const
{
    return 0;
}

void CadView::initContextAttributes()
{
    context_->SetDisplayMode(AIS_Shaded, Standard_True);

    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)
        ->SetColor(Quantity_NameOfColor::Quantity_NOC_RED);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetMethod(Aspect_TOHM_COLOR);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetDisplayMode(1);
    context_->HighlightStyle(Prs3d_TypeOfHighlight_LocalDynamic)->SetTransparency(0.5f);
    context_->UpdateCurrentViewer();
}

void CadView::initHilightAttributes()
{
    Quantity_Color L_YELLOW =
        Quantity_Color(245 / 255.0, 180 / 255.0, 77 / 255.0, Quantity_TOC_RGB); // ownYellow
    Quantity_Color L_BLUE = Quantity_Color(29 / 255.0, 166 / 255.0, 251 / 255.0, Quantity_TOC_RGB);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_Dynamic, 2, L_BLUE);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalDynamic, 2, L_YELLOW);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_Selected, 2, Quantity_NOC_RED);
    setHilightAttribute(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalSelected, 2,
                        Quantity_NOC_RED);
}

void CadView::setHilightAttribute(Prs3d_TypeOfHighlight idx, double lineWidth_aspect,
                                  Quantity_Color theColor)
{
    //        Prs3d_TypeOfHighlight_Selected,       //!< entire object is selected
    //        Prs3d_TypeOfHighlight_Dynamic,        //!< entire object is dynamically highlighted
    //        Prs3d_TypeOfHighlight_LocalSelected,  //!< part of the object is selected
    //        Prs3d_TypeOfHighlight_LocalDynamic,   //!< part of the object is dynamically
    //        highlighted
    const Handle(Prs3d_Drawer) &drawer = context_->HighlightStyle(idx);
    double lineWidth = 5.0;
    // Quantity_NameOfColor::Quantity_NOC_BROWN;//此处查看所有颜色

    drawer->SetMethod(Aspect_TOHM_COLOR);
    drawer->SetDisplayMode(0);
    drawer->SetColor(theColor);

    drawer->SetupOwnShadingAspect();
    drawer->SetupOwnPointAspect();

    drawer->SetLineAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->LineAspect()->Aspect() = *drawer->Link()->LineAspect()->Aspect();

    drawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->WireAspect()->Aspect() = *drawer->Link()->WireAspect()->Aspect();

    drawer->SetPlaneAspect(new Prs3d_PlaneAspect());
    *drawer->PlaneAspect()->EdgesAspect() = *drawer->Link()->PlaneAspect()->EdgesAspect();

    drawer->SetFreeBoundaryAspect(
        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->FreeBoundaryAspect()->Aspect() = *drawer->Link()->FreeBoundaryAspect()->Aspect();

    drawer->SetUnFreeBoundaryAspect(
        new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, lineWidth));
    *drawer->UnFreeBoundaryAspect()->Aspect() = *drawer->Link()->UnFreeBoundaryAspect()->Aspect();
    drawer->SetDatumAspect(new Prs3d_DatumAspect());

    drawer->ShadingAspect()->SetColor(theColor);
    drawer->WireAspect()->SetColor(theColor);
    drawer->LineAspect()->SetColor(theColor);
    drawer->PlaneAspect()->ArrowAspect()->SetColor(theColor);
    drawer->PlaneAspect()->IsoAspect()->SetColor(theColor);
    drawer->PlaneAspect()->EdgesAspect()->SetColor(theColor);
    drawer->FreeBoundaryAspect()->SetColor(theColor);
    drawer->UnFreeBoundaryAspect()->SetColor(theColor);
    drawer->PointAspect()->SetColor(theColor);
    for (Standard_Integer it = 0; it < Prs3d_DP_None; ++it)
    {
        if (Handle(Prs3d_LineAspect) aLineAsp =
                drawer->DatumAspect()->LineAspect((Prs3d_DatumParts)it))
        {
            aLineAsp->SetColor(theColor);
        }
    }

    drawer->WireAspect()->SetWidth(lineWidth_aspect);
    drawer->LineAspect()->SetWidth(lineWidth_aspect);
    drawer->PlaneAspect()->EdgesAspect()->SetWidth(lineWidth_aspect);
    drawer->FreeBoundaryAspect()->SetWidth(lineWidth_aspect);
    drawer->UnFreeBoundaryAspect()->SetWidth(lineWidth_aspect); // 辅助显示线框
    drawer->PointAspect()->SetTypeOfMarker(Aspect_TOM_O_POINT);
    drawer->PointAspect()->SetScale(lineWidth_aspect);

    // the triangulation should be computed using main presentation attributes,
    // and should not be overridden by highlighting
    drawer->SetAutoTriangulation(Standard_False);
}

void CadView::paintEvent(QPaintEvent *)
{
    view_->InvalidateImmediate();
    FlushViewEvents(context_, view_, true);
};

void CadView::resizeEvent(QResizeEvent *)
{
    view_->MustBeResized();
};

bool CadView::checkDetectedValid()
{
    if (!context_->HasDetected())
    {
        context_->ClearSelected(true);
        return false;
    }
    else if (context_->DetectedInteractive()->IsKind(STANDARD_TYPE(AIS_ViewCube)))
    {
        context_->SelectDetected();
        return false;
    }
    else
        return true;
}

//! 覆写鼠标按键按下事件
void CadView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::MiddleButton) // 平移
    {
        context_action_mode_ = CurrentAction3dEnum::CurAction3d_DynamicPanning;
        mouse_x_record_ = event->pos().x();
        mouse_y_record_ = event->pos().y();
        this->setCursor(getCursor(CursorType::pan));
    }
    else if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton)) // 旋转
    {
        context_action_mode_ = CurrentAction3dEnum::CurAction3d_DynamicRotation;
        view_->StartRotation(event->pos().x(), event->pos().y());
        this->setCursor(getCursor(CursorType::rot));
    }
    else if (event->buttons() == Qt::RightButton)
    {
        if (rightClickCb)
            rightClickCb(event);
        if (!checkDetectedValid())
            return;
    }
    else if (event->buttons() & Qt::LeftButton) // 鼠标左键选择模型
    {
        if (leftClickCb)
        {
            double x, y, z;
            view_->Convert(event->pos().x(), event->pos().y(), x, y, z);
            leftClickCb(x, y);
        }

        if (!checkDetectedValid())
            return;

        if (qApp->keyboardModifiers() == Qt::ControlModifier)
        {
            context_->SelectDetected(AIS_SelectionScheme_XOR); // 按下Shift键点击鼠标左键实现多选
        }
        else
        {
            context_->SelectDetected(AIS_SelectionScheme::AIS_SelectionScheme_Replace); // 单选模型
        }
        if (processClickCb)
        {
            context_->clearSelectedVec();
            context_->addToSelected(context_->DetectedOwner());
            processClickCb(context_->DetectedOwner());
        }
        view_->Update();
    }
};

Handle(SelectMgr_EntityOwner) CadView::getDetectedObj()
{
    if (detected_obj_.empty())
        return nullptr;
    else
        return detected_obj_.front();
}

//! 覆写鼠标按键释放事件
void CadView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    context_action_mode_ = CurrentAction3dEnum::CurAction3d_Nothing;
    this->setCursor(getCursor(CursorType::def));
};

//! Map Qt buttons bitmask to virtual keys.
static Aspect_VKeyMouse qtMouseButtons2VKeys(Qt::MouseButtons theButtons)
{
    Aspect_VKeyMouse mouse_button = Aspect_VKeyMouse_NONE;
    if ((theButtons & Qt::LeftButton) != 0)
    {
        mouse_button |= Aspect_VKeyMouse_LeftButton;
    }
    if ((theButtons & Qt::MiddleButton) != 0)
    {
        mouse_button |= Aspect_VKeyMouse_MiddleButton;
    }
    if ((theButtons & Qt::RightButton) != 0)
    {
        mouse_button |= Aspect_VKeyMouse_RightButton;
    }
    return mouse_button;
}

//! 覆写鼠标移动事件
void CadView::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标按下时，依然会做highlight检测
    // context_->MoveTo(event->pos().x(), event->pos().y(), view_, true);

    // 鼠标按下时，不做highlight检测
    const Graphic3d_Vec2i new_pos(event->pos().x(), event->pos().y());
    if (!view_.IsNull()
        && UpdateMousePosition(new_pos, qtMouseButtons2VKeys(event->buttons()),
                               Aspect_VKeyFlags_NONE, false))
    {
        update();
    }

    // 鼠标移动到模型时，模型高亮显示
    double x, y, z;
    view_->Convert(event->pos().x(), event->pos().y(), x, y, z);
    if (event->buttons() & Qt::LeftButton)
    {
        if (leftClickCb)
        {
            double x, y, z;
            view_->Convert(event->pos().x(), event->pos().y(), x, y, z);
            leftClickCb(x, y);
        }
    }
    else
    {
        if (moveInfoCb)
            moveInfoCb(x, y, z);
    }
    if (!busy_)
    {
        switch (context_action_mode_)
        {
        case CadView::CurrentAction3dEnum::CurAction3d_Nothing:
        {
            this->setCursor(getCursor(CursorType::def));
            break;
        }
        case CadView::CurrentAction3dEnum::CurAction3d_DynamicPanning:
        {
            view_->Pan(event->pos().x() - mouse_x_record_, mouse_y_record_ - event->pos().y());
            mouse_x_record_ = event->pos().x();
            mouse_y_record_ = event->pos().y();
            break;
        }
        case CadView::CurrentAction3dEnum::CurAction3d_DynamicZooming: break;
        case CadView::CurrentAction3dEnum::CurAction3d_DynamicRotation:
        {
            view_->Rotation(event->pos().x(), event->pos().y());
            break;
        }
        default: break;
        }
        //......
    }
};

//! 覆写鼠标滚轮事件
void CadView::wheelEvent(QWheelEvent *event)
{
    this->setCursor(getCursor(CursorType::zoom));
    view_->StartZoomAtPoint(event->position().x(), event->position().y());
    view_->ZoomAtPoint(0, 0, event->angleDelta().y() / zoom_scale_control_, 0);
};

void CadView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        std::cout << "press delete";
    }
    else if (event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Delete) {}
    return QWidget::keyPressEvent(event);
};

void CadView::removeAll(bool redraw)
{
    context_->RemoveAll(false);
    context_->Display(viewcube_, false); // 显示模型
    if (redraw)
        view_->Redraw();
}

void CadView::updateView(KUpdate cmd)
{
    switch (cmd)
    {
    case CadView::KUpdate::Redraw:
    {
        view_->Redraw();
        break;
    }
    case CadView::KUpdate::ZFitAll:
    {
        view_->ZFitAll();
        break;
    }
    case CadView::KUpdate::Fitall:
    {
        view_->FitAll();
        break;
    }
    case CadView::KUpdate::MustResized:
    {
        view_->MustBeResized();
        break;
    }
    default: break;
    }
}

} // namespace KDebugger
