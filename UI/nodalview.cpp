/*
 * Copyright 2019 Benoit Pelletier
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

#include "nodalview.h"
#include <QtWidgets>
#include <QWidget>
#include <utils.h>
#include "nodeitem.h"
#include "Components.h"
#include "pininputitem.h"
#include "linkitem.h"
#include "pinoutputitem.h"

const int NodalView::gridUnit = 20;

NodalView::NodalView(QWidget* parent)
    : QGraphicsView (parent),
      m_dragging(false),
      m_rightClickPressed(false),
      m_zoom(1.0),
      m_nextCreationPosition(0, 0)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showCustomContextMenu(const QPoint&)));

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
}

void NodalView::init()
{
    connect(scene(), &QGraphicsScene::changed, this, &NodalView::autocomputeSceneSize);
    createNode("Output", 100.0);
}

void NodalView::setZoomLimits(qreal min, qreal max)
{
    m_zoomMin = min;
    m_zoomMax = max;
}

void NodalView::setZoom(qreal zoom)
{
    m_zoom = zoom;
    updateZoomView();
}

void NodalView::save(QString fileName, const qreal duration)
{
    QJsonArray componentArray;
    // save data
    for(int i = 0; i < m_nodeList.size(); i++)
    {
        QJsonArray inputArray;
        for(int k = 0; k < m_nodeList[i]->getInputCount(); k++)
        {
            PinInputItem* pin = m_nodeList[i]->getInput(k);

            Component* link = pin->input()->getComponent();
            int linkIndex = -1;
            if(link != nullptr)
            {
                for(int p = 0; p < m_nodeList.size() && linkIndex < 0; p++)
                {
                    if(m_nodeList[p]->component() == link)
                    {
                        linkIndex = p;
                    }
                }
            }

            QJsonObject input;
            input["name"] = pin->input()->getName();
            input["value"] = pin->input()->getDefaultValue();
            input["link"] = linkIndex;

            inputArray.append(input);
        }

        QJsonObject component;
        component["id"] = i;
        component["x"] = m_nodeList[i]->x();
        component["y"] = m_nodeList[i]->y();
        component["name"] = m_nodeList[i]->component()->getName();
        component["inputs"] = inputArray;

        componentArray.append(component);
    }


    QJsonObject root;
    root["duration"] = duration;
    root["version"] = 1;
    root["components"] = componentArray;


    QJsonDocument json(root);
    QByteArray data = json.toJson();

    QFile file(fileName);
    file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(data);
    file.close();
}

void NodalView::load(QString fileName, qreal& duration)
{
    QFile file(fileName);
    file.open(QIODevice::Text | QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject root = json.object();

    if(root["version"].toInt() != 1)
        return;

    duration = root["duration"].toDouble();

    clearItems(1);

    QJsonArray componentArray = root["components"].toArray();
    // first create all components
    for(int i = 0; i < componentArray.size(); i++)
    {
        QJsonObject component = componentArray[i].toObject();
        NodeItem* node = nullptr;
        if(component["name"] == "Output")
        {
            node = m_nodeList[0];
        }
        else
        {
            node = createNode(component["name"].toString());
        }

        node->setX(component["x"].toDouble());
        node->setY(component["y"].toDouble());
    }

    // the set values and connect pins of each component
    for(int i = 0; i < root.size(); i++)
    {
        //QJsonObject component = root[i].toObject();
        QJsonArray inputs = componentArray[i].toObject()["inputs"].toArray();

        for(int k = 0; k < inputs.size(); k++)
        {
            PinInputItem* pin = m_nodeList[i]->getInput(k);
            pin->setDefaultValue(inputs[k].toObject()["value"].toDouble());

            int linkIndex = inputs[k].toObject()["link"].toInt();
            if(linkIndex >= 0 && linkIndex < m_nodeList.size())
            {
                LinkItem* link = new LinkItem();
                scene()->addItem(link);
                link->setFirstPin(pin);
                pin->addLink(link);

                PinOutputItem* otherPin = m_nodeList[linkIndex]->getOutput();

                if(!pin->connect(otherPin, link))
                {
                    delete link;
                }
            }
        }
    }
}

NodeItem* NodalView::createComponent(QString componentName, qreal width)
{
    setDirty();
    return createNode(componentName, width);
}

void NodalView::addNodeItem(NodeItem *item)
{
    scene()->addItem(item);
    m_nodeList.append(item);
}

int NodalView::clearItems(int from)
{
    int nbRemoved = 0;
    for(int i = m_nodeList.size()-1; i >= from; i--)
    {
        delete m_nodeList[i];
        m_nodeList.remove(i);
        nbRemoved++;
    }

//    if(nbRemoved > 0)
//    {
//        setDirty();
//    }

    return nbRemoved;
}

void NodalView::reset()
{
    clearItems(1);
    m_nodeList[0]->setX(0);
    m_nodeList[0]->setY(0);
}

void NodalView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if(event->key() == Qt::Key::Key_Delete)
    {
        deleteSelection();
    }
}

void NodalView::wheelEvent(QWheelEvent* event)
{
    //QPointF pos = mapToScene(event->pos());
    //qreal oldZoom = m_zoom;
    // zoom
    //const ViewportAnchor anchor = transformationAnchor();
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = event->angleDelta().y();
    qreal factor = 1.0 + (angle > 0.0 ? -0.05 : 0.05);
    m_zoom *= factor;
    m_zoom = Utils::Clamp(m_zoom, m_zoomMin, m_zoomMax);

    updateZoomView();

    event->accept();
}


void NodalView::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
    case Qt::RightButton:
        // the scrollhanddrag mode doesn't work ????
        setDragMode(QGraphicsView::NoDrag);

        m_lastMousePos = event->pos();
        m_startScenePos = mapToScene(event->pos());
        m_startMousePos = event->pos();
        m_rightClickPressed = true;
        break;
    case Qt::LeftButton:
        setDragMode(QGraphicsView::RubberBandDrag);
        QGraphicsView::mousePressEvent(event);
        break;
    default:
        setDragMode(QGraphicsView::NoDrag);
        QGraphicsView::mousePressEvent(event);
    }

    /*if(event->button() == Qt::MidButton)
    {
        m_lastMousePos = event->pos();
        m_dragging = true;
        m_startScenePos = mapToScene(event->pos());
        m_startMousePos = event->pos();
    }
    else if(event->button() == Qt::LeftButton)
    {
        m_rubberDrag = true;
        m_startMousePos = event->pos();
        m_rubberBand.move(event->pos());
        m_rubberBand.resize(0, 0);
        m_rubberBand.show();
    }*/
}

void NodalView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        m_rightClickPressed = false;
        if(m_dragging)
        {
            setContextMenuPolicy(Qt::NoContextMenu);
            m_dragging = false;
        }
        else
        {
            setContextMenuPolicy(Qt::CustomContextMenu);
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
    setDragMode(QGraphicsView::NoDrag);
    /*if(event->button() == Qt::MidButton)
    {
        m_dragging = false;
    }
    else if(event->button() == Qt::LeftButton)
    {
        m_rubberDrag = false;
        m_rubberBand.hide();
        //scene()->clearSelection();


        QPointF topLeft = mapToScene(qMin(event->pos().x(), m_startMousePos.x()), qMin(event->pos().y(), m_startMousePos.y()));
        QPointF bottomRight = mapToScene(qMax(event->pos().x(), m_startMousePos.x()), qMax(event->pos().y(), m_startMousePos.y()));
        QPainterPath shape;
        shape.addRect(topLeft.x(), topLeft.y(), bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
        scene()->setSelectionArea(shape);
    }*/
}

void NodalView::mouseMoveEvent(QMouseEvent* event)
{

    if(m_rightClickPressed)
    {
        m_dragging = true;
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = event->pos() - m_lastMousePos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());

        scene()->update(mapToScene(rect()).boundingRect());
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }

    /*if(m_rubberDrag)
    {
        QPoint topLeft(qMin(event->pos().x(), m_startMousePos.x()), qMin(event->pos().y(), m_startMousePos.y()));
        QPoint bottomRight(qMax(event->pos().x(), m_startMousePos.x()), qMax(event->pos().y(), m_startMousePos.y()));

        m_rubberBand.move(topLeft);
        m_rubberBand.resize(bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
    }*/

    m_lastMousePos = event->pos();
}

void NodalView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QVector<QLine> lines;
    QVector<QLine> mainLines;

    //int unit = 10;
    int nbLineWidth = qCeil(rect.width() / gridUnit);
    int nbLineHeight = qCeil(rect.height() / gridUnit);
    int startX = qCeil(rect.x() / gridUnit) * gridUnit;
    int startY = qCeil(rect.y() / gridUnit) * gridUnit;

    for(int i = 0; i < nbLineWidth; i++)
    {
        int x = startX + i * gridUnit;
        QLine line = QLine(x, qFloor(rect.top()), x, qCeil(rect.bottom()));
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
        QLine line = QLine(qFloor(rect.left()), y, qCeil(rect.right()), y);
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
    painter->fillRect(rect, brush);

    QPen pen(QColor(120, 120, 120));
    pen.setCosmetic(true);
    double opacity = 1.0;

    qreal mainGridFrom = 0.5;
    qreal mainGridTo = 3.0;
    qreal mainGridAmp = 1.0;

    qreal gridFrom = 0.5;
    qreal gridTo = 0.9;
    qreal gridAmp = 0.5;

    painter->setPen(pen);
    if(m_zoom < gridTo)
    {
        opacity = (m_zoom < gridFrom) ?
                    gridAmp
                  : gridAmp * (m_zoom - gridTo)/(gridFrom - gridTo);
        painter->setOpacity(opacity);
        painter->drawLines(lines);
    }

    if(m_zoom < mainGridTo)
    {
        opacity = (m_zoom < mainGridFrom) ?
                    mainGridAmp
                  : mainGridAmp * (m_zoom - mainGridTo)/(mainGridFrom - mainGridTo);
        painter->setOpacity(opacity);
        painter->drawLines(mainLines);
    }
}

int NodalView::deleteSelection()
{
    int nbRemoved = 0;

    for(QGraphicsItem* item : scene()->selectedItems())
    {
        NodeItem* node = qgraphicsitem_cast<NodeItem*>(item);
        if(node != nullptr && (node->component() == nullptr || node->component()->isRemovable()))
        {
            int index = m_nodeList.indexOf(node);
            if(index >= 0)
            {
                m_nodeList.remove(index);
            }

            delete item;
            nbRemoved++;
        }
    }

    if(nbRemoved > 0)
    {
        setDirty();
    }

    return nbRemoved;
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

void NodalView::showCustomContextMenu(const QPoint& pos)
{
    // for most widgets
    QPoint globalPos = mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    if(scene()->selectedItems().size() > 0)
    {
        QMenu myMenu;
        myMenu.addAction("Remove Component(s)");

        QAction* selectedAction = myMenu.exec(globalPos);
        if (selectedAction)
        {
            // something was chosen, do stuff
            deleteSelection();
        }
        else
        {
            // nothing was chosen
        }
    }
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

NodeItem *NodalView::createNode(QString componentName, qreal width)
{
    Component* component = ComponentFactory::CreateComponent(componentName);
    NodeItem* item = new NodeItem();
    item->setComponent(component);
    item->setPos(m_nextCreationPosition);
    item->setWidth(width);

    addNodeItem(item);
    m_nextCreationPosition += QPoint(20, 20);

    connect(item, SIGNAL(dirtyChanged()), this, SLOT(setDirty()));

    return item;
}
