#include "CADView.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include <qdebug.h>
#include <Prs3d_DatumAspect.hxx>
#include <V3d_View.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAlgoAPI_Algo.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_Shape.hxx>
#include <Bnd_Box2d.hxx>
#include <AIS_TextLabel.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Bnd_OBB.hxx>
#include <BRepBndLib.hxx>
#include <gp_Trsf.hxx>
#include <unordered_set>

#include "public.h"

//#ifdef WNT
#include <WNT_Window.hxx>

#include <Xw_Window.hxx>
//#endif
using std::vector;

static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}

OccView::OccView(QWidget* parent)
    : QWidget(parent),
    myXmin(0),
    myYmin(0),
    myXmax(0),
    myYmax(0),
    cur_mode_(CurAction3d_DynamicPanning),
    rectband_(NULL)
{
    // No Background
    setBackgroundRole(QPalette::NoRole);

    // set focus policy to threat QContextMenuEvent from keyboard  
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking(true);

    init();
}

void OccView::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection =
        new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId)winId();

    // Create appropriate window for platform
//#ifdef WNT
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
    //#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    //    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView*)window_handle);
    //#else
       // Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window)window_handle);
    //#endif

        // Create V3dViewer and V3d_View
    viewer_ = new V3d_Viewer(GetGraphicDriver());

    view_ = viewer_->CreateView();

    view_->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    // Create AISInteractiveContext
    context_ = new AIS_InteractiveContext(viewer_);

    // Set up lights etc
    viewer_->SetDefaultLights();
    viewer_->SetLightOn();

    //渐变色背景
    Quantity_Color blue = Quantity_Color(30 / 255.0, 30 / 255.0, 30 / 255.0, Quantity_TOC_RGB);
    view_->SetBackgroundColor(blue);
    //设置显示模式
    context_->SetDisplayMode(AIS_Shaded, Standard_True);
    view_->MustBeResized();
    view_->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    context_->SetDisplayMode(AIS_Shaded, Standard_True);
    setViewCube();
}

void OccView::setViewCube()
{
    auto aisViewCube = new AIS_ViewCube;
    double transp_num = 0.75;//透明程度，1为完全透明
    aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
    //aisViewCube->SetFixedAnimationLoop(false);
    aisViewCube->SetSize(62);
    aisViewCube->SetFontHeight(14);
    aisViewCube->SetBoxTransparency(transp_num);

    aisViewCube->SetTransformPersistence(
        new Graphic3d_TransformPers(
            Graphic3d_TMF_TriedronPers,
            Aspect_TOTP_LEFT_UPPER,
            Graphic3d_Vec2i(85, 85)));
    //m_gfxScene.addObject(aisViewCube);

    const Handle_Prs3d_DatumAspect myDatumColor = new Prs3d_DatumAspect();
    myDatumColor->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_GREEN2);
    myDatumColor->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_RED2);
    myDatumColor->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
    myDatumColor->ShadingAspect(Prs3d_DP_XAxis)->SetTransparency(transp_num);
    myDatumColor->ShadingAspect(Prs3d_DP_YAxis)->SetTransparency(transp_num);
    myDatumColor->ShadingAspect(Prs3d_DP_ZAxis)->SetTransparency(transp_num);
    aisViewCube->Attributes()->SetDatumAspect(myDatumColor);

    viewcube_ = aisViewCube;

    context_->Display(viewcube_, false);  // 显示模型

}

const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return context_;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* OccView::paintEngine() const
{
    return 0;
}

void OccView::paintEvent(QPaintEvent* /*theEvent*/)
{
    view_->Redraw();
}

void OccView::resizeEvent(QResizeEvent* /*theEvent*/)
{
    if (!view_.IsNull())
    {
        view_->MustBeResized();
    }
}

void OccView::fitAll(void)
{
    view_->FitAll();
    view_->ZFitAll();
    view_->Redraw();
}

void OccView::reset(void)
{
    view_->Reset();
}

void OccView::removeAll(void)
{
    context_->RemoveAll(true);
    setViewCube();
}

void OccView::mousePressEvent(QMouseEvent* theEvent)
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        cur_mode_ = CurAction3d_DynamicPanning;
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}

void OccView::mouseReleaseEvent(QMouseEvent* theEvent)
{
    cur_mode_ = CurAction3d_Nothing;
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }

}

void OccView::mouseMoveEvent(QMouseEvent* theEvent)
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
}

void OccView::wheelEvent(QWheelEvent* theEvent)
{
    onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
}

void OccView::onLButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

}

void OccView::onMButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

    if (cur_mode_ == CurAction3d_DynamicRotation)
    {
        view_->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

    if (cur_mode_ == CurAction3d_DynamicRotation)
    {
        view_->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onMouseWheel(const int /*theFlags*/, const int theDelta, const QPoint thePoint)
{
    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    view_->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup(QMenu* /*theMenu*/)
{}

void OccView::popup(const int /*x*/, const int /*y*/)
{}

void OccView::onLButtonUp(const int theFlags, const QPoint thePoint)
{
    // Hide the QRubberBand
    if (rectband_)
    {
        rectband_->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void OccView::onMButtonUp(const int /*theFlags*/, const QPoint thePoint)
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void OccView::onRButtonUp(const int /*theFlags*/, const QPoint thePoint)
{
    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove(const int theFlags, const QPoint thePoint)
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MidButton)
    {
        switch (cur_mode_)
        {
        case CurAction3d_DynamicRotation:
            view_->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            view_->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            view_->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;

        default:
            break;
        }
    }

}

void OccView::dragEvent(const int x, const int y)
{
    context_->Select(myXmin, myYmin, x, y, view_, Standard_True);

    emit selectionChanged();
}

void OccView::multiDragEvent(const int x, const int y)
{
    context_->ShiftSelect(myXmin, myYmin, x, y, view_, Standard_True);

    emit selectionChanged();

}

void OccView::inputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    context_->Select(Standard_True);

    emit selectionChanged();
}

void OccView::multiInputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    context_->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void OccView::moveEvent(const int x, const int y)
{
    context_->MoveTo(x, y, view_, Standard_True);
}

void OccView::multiMoveEvent(const int x, const int y)
{
    context_->MoveTo(x, y, view_, Standard_True);
}

void OccView::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY)
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!rectband_)
    {
        rectband_ = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        rectband_->setStyle(QStyleFactory::create("windows"));
    }

    rectband_->setGeometry(aRect);
    rectband_->show();
}

void OccView::panByMiddleButton(const QPoint& thePoint)
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    view_->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void OccView::drawTestData(const std::vector<TopoDS_Face>& all_face_)
{
    int cnt = all_face_.size();
    for (int i = 0; i < cnt; ++i)
    {
        Handle(AIS_Shape) shape = new AIS_Shape(all_face_[i]);
        context_->Display(shape, false);
    }
}

void OccView::drawTestLabelData(const std::vector<Handle(AIS_TextLabel)>& all_labels)
{
    int textcnt = all_labels.size();
    for (int j = 0; j < textcnt; ++j)
    {
        context_->Display(all_labels[j], false);
    }
}

void OccView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_1)
    {
    }
    else if (event->key() == Qt::Key_2)//opt
    {
    }
    else if (event->key() == Qt::Key_0)
    {
        context_->RemoveAll(true);
    }
}