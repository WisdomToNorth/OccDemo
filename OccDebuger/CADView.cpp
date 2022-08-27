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
#include "data.h"
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
    myCurrentMode(CurAction3d_DynamicRotation),
    myDegenerateModeIsOn(Standard_True),
    myRectBand(NULL)
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
    myViewer = new V3d_Viewer(GetGraphicDriver());

    myView = myViewer->CreateView();

    myView->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    // Set up lights etc
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    //渐变色背景
    Quantity_Color blue = Quantity_Color(30 / 255.0, 30 / 255.0, 30 / 255.0, Quantity_TOC_RGB);
    myView->SetBackgroundColor(blue);
    //设置显示模式
    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
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

    ////下面几句当上边存在时是生效的，暂时不知道为啥不能直接用。
    //const Handle_Prs3d_DatumAspect& datumAspect = aisViewCube->Attributes()->DatumAspect();
    //datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
    //datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
    //datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_GREEN2);

    m_aViewCube = aisViewCube;


    myContext->Display(m_aViewCube, false);  // 显示模型

}

const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return myContext;
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
    myView->Redraw();
}

void OccView::resizeEvent(QResizeEvent* /*theEvent*/)
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}

void OccView::fitAll(void)
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccView::reset(void)
{
    myView->Reset();
}

void OccView::pan(void)
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::zoom(void)
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate(void)
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void OccView::mousePressEvent(QMouseEvent* theEvent)
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}

void OccView::mouseReleaseEvent(QMouseEvent* theEvent)
{
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

    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown(const int /*theFlags*/, const QPoint /*thePoint*/)
{

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

    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup(QMenu* /*theMenu*/)
{
}

void OccView::popup(const int /*x*/, const int /*y*/)
{
}

void OccView::onLButtonUp(const int theFlags, const QPoint thePoint)
{
    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
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
        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
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
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void OccView::multiDragEvent(const int x, const int y)
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();

}

void OccView::inputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(Standard_True);

    emit selectionChanged();
}

void OccView::multiInputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void OccView::moveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::multiMoveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY)
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}

void OccView::panByMiddleButton(const QPoint& thePoint)
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void OccView::drawtestdata(const std::vector<TopoDS_Face>& all_face_,
    const std::vector<Handle(AIS_TextLabel)>& all_labels)
{
    int cnt = all_face_.size();
    for (int i = 0; i < cnt; ++i)
    {
        Handle(AIS_Shape) shape = new AIS_Shape(all_face_[i]);
        myContext->Display(shape, false);
        if (!all_labels[i].IsNull())myContext->Display(all_labels[i], false);
    }
    myView->Update();
    myView->FitAll();
}

void OccView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_1)
    {
        myView->FitAll();
        myView->ZFitAll();
    }
    else if (event->key() == Qt::Key_2)
    {

    }
    else if (event->key() == Qt::Key_3)
    {

    }
    else if (event->key() == Qt::Key_4)
    {

    }
    else if (event->key() == Qt::Key_5)
    {
    }
    else if (event->key() == Qt::Key_6)//origin
    {
       
    }
    else if (event->key() == Qt::Key_7)//opt
    {

        //int cnt = all_face_.size();
        //UnionFind finder;
        //finder.init(cnt);
        //qDebug() << "###########opt##########\ncalculate start!";
        //auto t0 = std::chrono::steady_clock::now();
        //vector<Bnd_OBB> faces_obb;//初始化obb vector
        //for (int i = 0; i < cnt; ++i)
        //{
        //    Bnd_OBB box_obb;
        //    BRepBndLib ret;

        //    ret.AddOBB(all_face_[i], box_obb, true, true, true);
        //    //box_obb.Enlarge(EnlargeGap);
        //    faces_obb.emplace_back(box_obb);
        //}
        //qDebug() << "convert done!";
        //for (int i = 0; i < cnt; ++i)
        //{
        //    for (int j = i + 1; j < cnt; ++j)
        //    {
        //        if (!faces_obb[i].IsOut(faces_obb[j]))
        //        {
        //            finder.merge(i, j);
        //        }
        //    }
        //}
        //qDebug() << "merge done!";
        //std::unordered_map<int, int> aux;
        //int unionsize = 0;
        //for (int i = 0; i < cnt; ++i)
        //{
        //    if (!aux.count(finder.find(i)))
        //    {
        //        unionsize++;
        //        aux[finder.find(i)] = unionsize;
        //    }
        //}
        //qDebug() << "classfied size: " << unionsize;
        //vector<vector<TopoDS_Face>> l_merge_mark_vec(unionsize + 1, vector<TopoDS_Face>());
        //for (int i = 0; i < cnt; ++i)
        //{
        //    l_merge_mark_vec[aux[finder.find(i)]].emplace_back(all_face_[i]);
        //}
        //if (cnt < 1000)
        //{
        //    vector<vector<int>> checkvec(unionsize + 1, vector<int>());
        //    for (int i = 0; i < cnt; ++i)
        //    {
        //        checkvec[aux[finder.find(i)]].emplace_back(i);
        //    }
        //    printvecvec(checkvec);
        //}

        //qDebug() << "vec init done!";
        //auto t1 = std::chrono::steady_clock::now();
        //for (auto& vec : l_merge_mark_vec)
        //{
        //    int cursize = vec.size();
        //    for (int i = 0; i < cursize; ++i)
        //    {
        //        for (int j = i + 1; j < cursize; ++j)
        //        {
        //            auto fused = BRepAlgoAPI_Fuse(all_face_[i], all_face_[j]);
        //        }
        //    }
        //}
        //qDebug() << "all done!";
        //auto t2 = std::chrono::steady_clock::now();
        //double dr_us = std::chrono::duration<double, std::micro>(t2 - t0).count();
        //double dr_us_merge = std::chrono::duration<double, std::micro>(t2 - t1).count();
        //qDebug() << "all time:" << dr_us / 1000;
        //qDebug() << "merge time:" << dr_us_merge / 1000;
    }
    else if (event->key() == Qt::Key_0)
    {
        myContext->RemoveAll(true);
    }
}