﻿/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : OccView.h
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-09 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Adapte OpenCASCADE view for Qt.
*/

#ifndef _OCCVIEW_H_
#define _OCCVIEW_H_

#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewCube.hxx>
#include <TopoDS_Face.hxx>
#include <AIS_TextLabel.hxx>
class QMenu;
class QRubberBand;

//! Adapted a QWidget for OpenCASCADE viewer.
class OccView : public QWidget
{
    Q_OBJECT

public:
    //! mouse actions.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    //! constructor.
    OccView(QWidget* parent);

    const Handle(AIS_InteractiveContext)& getContext() const;
    void drawtestdata(const std::vector<TopoDS_Face>& all_face_,
        const std::vector<Handle(AIS_TextLabel)>& all_labels);
signals:
    void selectionChanged(void);

public slots:
    //! operations for the view.
    void fitAll(void);
    void reset(void);
    void removeAll();
protected:
    virtual QPaintEngine* paintEngine() const;

    // Paint events.
    virtual void paintEvent(QPaintEvent* theEvent);
    virtual void resizeEvent(QResizeEvent* theEvent);

    // Mouse events.
    virtual void mousePressEvent(QMouseEvent* theEvent);
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent* theEvent);
    virtual void wheelEvent(QWheelEvent* theEvent);
    virtual void keyPressEvent(QKeyEvent* event);
    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Popup menu.
    virtual void addItemInPopup(QMenu* theMenu);

protected:
    void init(void);
    void setViewCube();
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);

private:

    //! the occ viewer.
    Handle(V3d_Viewer) viewer_;
    Handle(AIS_ViewCube) viewcube_;
    //! the occ view.
    Handle(V3d_View) view_;

    //! the occ context.
    Handle(AIS_InteractiveContext) context_;

    //! save the mouse position.
    Standard_Integer myXmin;
    Standard_Integer myYmin;
    Standard_Integer myXmax;
    Standard_Integer myYmax;

    //! the mouse current mode.
    CurrentAction3d cur_mode_;

    //! rubber rectangle for the mouse selection.
    QRubberBand* rectband_;

};

#endif // _OCCVIEW_H_
