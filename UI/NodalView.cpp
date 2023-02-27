/*
 * Copyright 2019-2020 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "NodalView.h"
#include <Utils.h>
#include "NodeItem.h"
#include "NodalScene.h"
#include "LinkItem.h"
#include "PinItem.h"

const int NodalView::gridUnit = 20;

NodalView::NodalView(QWidget* _parent)
    : QGraphicsView (_parent)
    , m_dragging(false)
    , m_rightClickPressed(false)
    , m_zoom(1.0)
    , m_scene(nullptr)
{
    setAcceptDrops(true);
#if defined(Q_OS_WIN32)
    setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showCustomContextMenu(const QPoint&)));
#else
    setContextMenuPolicy(Qt::NoContextMenu);
#endif

    // fix all graphic issue, but is less performant
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QPushButton* btn_resetZoom = new QPushButton("reset", this);
    btn_resetZoom->move(5, 5);
    connect(btn_resetZoom, &QPushButton::clicked, [this]() { setZoom(1.0); });

    QLabel* lbl_zoom = new QLabel("Label", this);
    lbl_zoom->move(65, 5);
    lbl_zoom->setProperty("class", "standard");
    connect(this, &NodalView::zoomChanged, [lbl_zoom](qreal zoom){lbl_zoom->setText(QString::number(100.0 * zoom, 'g', zoom < 1 ? 2 : 3) + "%");});

    setZoom(1.0);

    m_menuAdd = nullptr;
}

void NodalView::init()
{
    connect(scene(), &QGraphicsScene::changed, this, &NodalView::autocomputeSceneSize);
}

void NodalView::setZoomLimits(qreal _min, qreal _max)
{
    m_zoomMin = _min;
    m_zoomMax = _max;
}

void NodalView::setZoom(qreal _zoom)
{
    m_zoom = _zoom;
    updateZoomView();
}

void NodalView::keyPressEvent(QKeyEvent* _event)
{
    QGraphicsView::keyPressEvent(_event);
    if(_event->key() == Qt::Key::Key_Delete)
    {
        getScene()->deleteSelection();
    }
}

void NodalView::wheelEvent(QWheelEvent* _event)
{
    //QPointF pos = mapToScene(event->pos());
    //qreal oldZoom = m_zoom;
    // zoom
    //const ViewportAnchor anchor = transformationAnchor();
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = _event->angleDelta().y();
    qreal factor = 1.0 + (angle > 0.0 ? -0.05 : 0.05);
    m_zoom *= factor;
    m_zoom = Utils::Clamp(m_zoom, m_zoomMin, m_zoomMax);

    updateZoomView();

    _event->accept();
}


void NodalView::mousePressEvent(QMouseEvent* _event)
{
    switch(_event->button())
    {
    case Qt::RightButton:
        // the scrollhanddrag mode doesn't work ????
        setDragMode(QGraphicsView::NoDrag);

        m_lastMousePos = _event->pos();
        m_startScenePos = mapToScene(_event->pos());
        m_startMousePos = _event->pos();
        m_rightClickPressed = true;
        break;
    case Qt::LeftButton:
        setDragMode(QGraphicsView::RubberBandDrag);
        QGraphicsView::mousePressEvent(_event);
        break;
    default:
        setDragMode(QGraphicsView::NoDrag);
        QGraphicsView::mousePressEvent(_event);
    }
}

void NodalView::mouseReleaseEvent(QMouseEvent* _event)
{
    if(_event->button() == Qt::RightButton)
    {
        m_rightClickPressed = false;
        if(m_dragging)
        {
#if defined(Q_OS_WIN32)
            setContextMenuPolicy(Qt::NoContextMenu);
#endif
            m_dragging = false;
        }
        else
        {
#if defined(Q_OS_WIN32)
            setContextMenuPolicy(Qt::CustomContextMenu);
#else
            showCustomContextMenu(_event->pos());
#endif
        }
    }

    QGraphicsView::mouseReleaseEvent(_event);
    setDragMode(QGraphicsView::NoDrag);
}

void NodalView::mouseMoveEvent(QMouseEvent* _event)
{

    if(m_rightClickPressed)
    {
        m_dragging = true;
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = _event->pos() - m_lastMousePos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());

        scene()->update(mapToScene(rect()).boundingRect());
    }
    else
    {
        QGraphicsView::mouseMoveEvent(_event);
    }

    m_lastMousePos = _event->pos();
}

void NodalView::dragEnterEvent(QDragEnterEvent* _event)
{
    if(_event->mimeData()->hasFormat("add/component"))
    {
        _event->acceptProposedAction();
    }
}

void NodalView::dropEvent(QDropEvent* _event)
{
    _event->acceptProposedAction();

    QString componentName = QString::fromUtf8(_event->mimeData()->data("add/component"));

    getScene()->setCreationPosition(mapToScene(_event->pos()));
    getScene()->createComponent(componentName);
}

// must be implemented
void NodalView::dragMoveEvent(QDragMoveEvent* _event)
{
    Q_UNUSED(_event)
}

// must be implemented
void NodalView::dragLeaveEvent(QDragLeaveEvent* _event)
{
    Q_UNUSED(_event)
}

void NodalView::drawBackground(QPainter* _painter, const QRectF& _rect)
{
    QVector<QLine> lines;
    QVector<QLine> mainLines;

    int nbLineWidth = qCeil(_rect.width() / gridUnit);
    int nbLineHeight = qCeil(_rect.height() / gridUnit);
    int startX = qCeil(_rect.x() / gridUnit) * gridUnit;
    int startY = qCeil(_rect.y() / gridUnit) * gridUnit;

    for(int i = 0; i < nbLineWidth; i++)
    {
        int x = startX + i * gridUnit;
        QLine line = QLine(x, qFloor(_rect.top()), x, qCeil(_rect.bottom()));
        if((x % (5 * gridUnit)) == 0)
        {
            mainLines.push_front(line);
        }
        else
        {
            lines.push_back(line);
        }
    }
    for(int i = 0; i < nbLineHeight; i++)
    {
        int y = startY + i * gridUnit;
        QLine line = QLine(qFloor(_rect.left()), y, qCeil(_rect.right()), y);
        if((y % (5 * gridUnit)) == 0)
        {
            mainLines.push_front(line);
        }
        else
        {
            lines.push_back(line);
        }
    }

    QBrush brush(QColor(50, 50, 50), Qt::BrushStyle::SolidPattern);
    _painter->fillRect(_rect, brush);

    QPen pen(QColor(120, 120, 120));
    pen.setCosmetic(true);
    double opacity = 1.0;

    qreal mainGridFrom = 0.5;
    qreal mainGridTo = 3.0;
    qreal mainGridAmp = 1.0;

    qreal gridFrom = 0.5;
    qreal gridTo = 0.9;
    qreal gridAmp = 0.5;

    _painter->setPen(pen);
    if(m_zoom < gridTo)
    {
        opacity = (m_zoom < gridFrom) ?
                    gridAmp
                  : gridAmp * (m_zoom - gridTo)/(gridFrom - gridTo);
        _painter->setOpacity(opacity);
        _painter->drawLines(lines);
    }

    if(m_zoom < mainGridTo)
    {
        opacity = (m_zoom < mainGridFrom) ?
                    mainGridAmp
                  : mainGridAmp * (m_zoom - mainGridTo)/(mainGridFrom - mainGridTo);
        _painter->setOpacity(opacity);
        _painter->drawLines(mainLines);
    }
}

NodalScene *NodalView::getScene()
{
    if(m_scene == nullptr)
    {
        m_scene = dynamic_cast<NodalScene*>(scene());
        Q_ASSERT(m_scene != nullptr);
    }
    return m_scene;
}

void NodalView::updateZoomView()
{
    //qreal delta = m_zoom / oldZoom;
    //this->scale(delta, delta);
    /*QRectF oldView = mapToScene(rect()).boundingRect();//(mapToScene(0, 0), mapToScene(rect().bottomRight()));
    qDebug() << "oldView: " << oldView;
    QRectF newView(
                oldView.left(),
                oldView.top(),
                //pos.x() + (oldView.left() - pos.x()) * delta,
                //pos.y() + (oldView.top() - pos.y()) * delta,
                oldView.width() * delta,
                oldView.height() * delta);
    qDebug() << "newView: " << newView;*/

    //scene()->setSceneRect(newView);


//    ensureVisible(newView, 0, 0);


//    QPointF newPos = mapToScene(event->pos());
//    this->translate(50, 50);//newPos.x() - pos.x(), newPos.y() - pos.y());


    ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    //autocomputeSceneSize();
    setTransform(QTransform::fromScale(1./m_zoom, 1./m_zoom));
    setTransformationAnchor(anchor);

//    QPoint  pos  = event->pos();
//    QPointF posf = this->mapToScene(pos);

//    int w = this->viewport()->width();
//    int h = this->viewport()->height();

//    qreal wf = mapToScene(QPoint(w-1, 0)).x() - mapToScene(QPoint(0,0)).x();
//    qreal hf = mapToScene(QPoint(0, h-1)).y() - mapToScene(QPoint(0,0)).y();

//    qreal lf = posf.x() - pos.x() * wf / w;
//    qreal tf = posf.y() - pos.y() * hf / h;

//    /* try to set viewport properly */
//    this->ensureVisible(lf, tf, wf, hf, 0, 0);

//    QPointF newPos = this->mapToScene(pos);

//    /* readjust according to the still remaining offset/drift
//     * I don't know how to do this any other way */
//    this->ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf,
//                    QSizeF(wf, hf)), 0, 0);
    update();
    autocomputeSceneSize();
    //scene()->update();//mapToScene(rect()).boundingRect());

    emit zoomChanged(m_zoom);
}

void NodalView::showCustomContextMenu(const QPoint& _pos)
{
    getScene()->isContextMenu(true);

    // for most widgets
    QPoint globalPos = mapToGlobal(_pos);
    getScene()->setContextPosition(mapToScene(_pos));
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QAction act_breakAllLinks("Break all link(s)");
    QAction act_breakLink("Break link");
    QAction act_removeComponents("Remove Component(s)");
    QAction act_createPassThrough("Add passthrough");

    QMenu myMenu;

    if(m_menuAdd != nullptr)
    {
        myMenu.addMenu(m_menuAdd);
        qDebug() << "Add menu in context";
    }

    QGraphicsItem* item = scene()->itemAt(mapToScene(_pos), QTransform());
    PinItem* pin = dynamic_cast<PinItem*>(item);
    LinkItem* link = dynamic_cast<LinkItem*>(item);
    if(link != nullptr)
    {
        myMenu.addAction(&act_breakLink);
        myMenu.addAction(&act_createPassThrough);
    }
    else if(pin != nullptr && pin->isLinked())
    {
        myMenu.addAction(&act_breakAllLinks);
    }
    if(scene()->selectedItems().size() > 0)
    {
        myMenu.addAction(&act_removeComponents);
    }

    // don't show menu if no action inside
    if(myMenu.isEmpty())
        return;

    QAction* selectedAction = myMenu.exec(globalPos);
    if (selectedAction == &act_breakLink)
    {
        link->firstPin()->unlink(link->secondPin());
    }
    if (selectedAction == &act_breakAllLinks)
    {
        qDebug() << "break links";
        pin->unlinkAll();
    }
    else if (selectedAction == &act_removeComponents)
    {
        getScene()->deleteSelection();
    }
    else if (selectedAction == &act_createPassThrough)
    {
        getScene()->createPassThrough(link);
    }
    else
    {
        // nothing was chosen
    }
    getScene()->isContextMenu(false);
}


void NodalView::autocomputeSceneSize()
{
    int margin = 100;//qRound(100.0 * m_zoom);

    // Widget viewport recangle
    QRectF widget_rect_in_scene(mapToScene(-margin, -margin), mapToScene(rect().bottomRight() + QPoint(margin, margin)));

    setSceneRect(widget_rect_in_scene);
    //qDebug() << "Autocompute: " << widget_rect_in_scene.topLeft() << " | " << widget_rect_in_scene.bottomRight();

    /*if(scene() != nullptr)
    {
        scene()->setSceneRect(widget_rect_in_scene);
    }*/
}

