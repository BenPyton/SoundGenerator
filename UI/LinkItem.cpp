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

#include "LinkItem.h"
#include "NodalView.h"
#include "PinItem.h"
#include "NodeItem.h"
#include "PinOutputItem.h"
#include <QtMath>

QVector<LinkItem*> LinkItem::s_linkList;

LinkItem::LinkItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_size = 1;
    m_penStyle = Qt::PenStyle::SolidLine;
    m_pinA = nullptr;
    m_pinB = nullptr;

    m_mousePos = QPointF(0, 0);

    setZValue(-10000);

    qDebug() << "Create LinkItem";
}

LinkItem::~LinkItem()
{
    qDebug() << "Destroy LinkItem";
    if (isRegistered(this))
    {
        unregisterLink(this);
    }
}

void LinkItem::setMousePos(QPointF mousePos)
{
    m_mousePos = mousePos;
    prepareGeometryChange();
}

void LinkItem::setPins(PinItem *_pinA, PinItem* _pinB)
{
    m_pinA = _pinA;
    m_pinB = _pinB;
}

void LinkItem::prepareChange()
{
    prepareGeometryChange();
}

void LinkItem::check()
{
    qDebug() << "Check Link Item";
    Q_ASSERT(scene() == nullptr);
    Q_ASSERT(m_pinA == nullptr && m_pinB == nullptr);
}

void LinkItem::registerLink(LinkItem*_link)
{
    Q_ASSERT(_link != nullptr);
    if(!isRegistered(_link))
    {
        s_linkList.append(_link);
    }
}

void LinkItem::unregisterLink(LinkItem*_link)
{
    Q_ASSERT(_link != nullptr);
    int index = s_linkList.indexOf(_link);
    Q_ASSERT(index >= 0);
    s_linkList.removeAt(index);
}

bool LinkItem::isRegistered(LinkItem *_link)
{
    Q_ASSERT(_link != nullptr);
    return s_linkList.indexOf(_link) >= 0;
}

LinkItem *LinkItem::getLinkBetween(PinItem *_pinA, PinItem *_pinB)
{
    LinkItem* link = nullptr;
    for(int i = 0; link == nullptr && i < s_linkList.size(); ++i)
    {
        if(s_linkList[i]->hasPins(_pinA, _pinB))
        {
            link = s_linkList[i];
        }
    }
    return link;
}

QList<LinkItem*> LinkItem::getLinksWithPin(PinItem* _pin)
{
    QList<LinkItem*> links;
    for(int i = 0; i < s_linkList.size(); ++i)
    {
        if(s_linkList[i]->m_pinA == _pin || s_linkList[i]->m_pinB == _pin)
        {
            links.append(s_linkList[i]);
        }
    }
    return links;
}

PinItem *LinkItem::getPinThatIsNot(PinItem *pin)
{
    PinItem* toReturn = nullptr;
    if(m_pinA != pin)
        toReturn = m_pinA;
    else if(m_pinB != pin)
        toReturn = m_pinB;
    return toReturn;
}

bool LinkItem::hasPins(PinItem *_pinA, PinItem *_pinB)
{
    return (m_pinA == _pinA && m_pinB == _pinB) || (m_pinA == _pinB && m_pinB == _pinA);
}

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(m_pinA != nullptr || m_pinB != nullptr)
    {
        bool isCompSelected = (m_pinA != nullptr && m_pinA->parentNode()->isSelected())
            || (m_pinB != nullptr && m_pinB->parentNode()->isSelected());

        QPen pen(QColor(200, 200, 200));
        pen.setWidth(isCompSelected ? 3 : 1);
        pen.setCosmetic(true);

        painter->setPen(pen);
        painter->drawPath(getLinePath());
    }
}

QRectF LinkItem::boundingRect() const
{
    QPointF firstPoint = (m_pinA != nullptr) ? m_pinA->scenePos() : m_mousePos;
    QPointF secondPoint = (m_pinB != nullptr) ? m_pinB->scenePos() : m_mousePos;
    QRectF bounds;

    bounds.setLeft(qMin(firstPoint.x(), secondPoint.x()) - 20);
    bounds.setRight(qMax(firstPoint.x(), secondPoint.x()) + 20);
    bounds.setTop(qMin(firstPoint.y(), secondPoint.y()) - 20);
    bounds.setBottom(qMax(firstPoint.y(), secondPoint.y()) + 20);

    return bounds;
}

QPainterPath LinkItem::shape() const
{
    qreal width = 10.;
    QPainterPathStroker pathStroker;
    pathStroker.setWidth(width);
    return pathStroker.createStroke(getLinePath());
}

QPainterPath LinkItem::getLinePath() const
{
    QPointF firstPoint = (m_pinA != nullptr) ? m_pinA->scenePos() : m_mousePos;
    QPointF secondPoint = (m_pinB != nullptr) ? m_pinB->scenePos() : m_mousePos;

    qreal firstPointSign = 1.0;
    if(m_pinA != nullptr)
    {
        firstPointSign = qgraphicsitem_cast<PinOutputItem*>(m_pinA) != nullptr ? 1.0 : -1.0 ;
    }
    qreal secondPointSign = -firstPointSign;

    qreal alpha = 0.6;
    qreal dist = qMax(abs(firstPoint.x() - secondPoint.x()), abs(firstPoint.y() - secondPoint.y()));
    QPointF ctrlPoint1(firstPoint.x() + firstPointSign * dist * alpha, firstPoint.y());
    QPointF ctrlPoint2(secondPoint.x() + secondPointSign * dist * alpha, secondPoint.y());

    QPainterPath path;
    path.moveTo(firstPoint);
    path.cubicTo(ctrlPoint1, ctrlPoint2, secondPoint);
    return path;
}
