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

#include "linkitem.h"
#include "nodalview.h"
#include "pinitem.h"
#include <QtMath>

LinkItem::LinkItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_size = 1;
    m_penStyle = Qt::PenStyle::SolidLine;
    m_pinA = nullptr;
    m_pinB = nullptr;

    m_mousePos = QPointF(0, 0);

    setZValue(10000);
}

void LinkItem::setMousePos(QPointF mousePos)
{
    m_mousePos = mousePos;
    prepareGeometryChange();
}

void LinkItem::setFirstPin(PinItem *pin)
{
    m_pinA = pin;
}

void LinkItem::setSecondPin(PinItem *pin)
{
    m_pinB = pin;
}

void LinkItem::unlink()
{
    if(m_pinA != nullptr)
    {
        m_pinA->removeLink(this);
    }
    if(m_pinB != nullptr)
    {
        m_pinB->removeLink(this);
    }
}

void LinkItem::prepareChange()
{
    prepareGeometryChange();
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

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(QColor(200, 200, 200));
    pen.setWidth(1);
    pen.setCosmetic(true);

    QPointF firstPoint = (m_pinA != nullptr) ? m_pinA->scenePos() : m_mousePos;
    QPointF secondPoint = (m_pinB != nullptr) ? m_pinB->scenePos() : m_mousePos;

    painter->setPen(pen);
    painter->drawLine(firstPoint, secondPoint);
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
    qreal width = 1.;
    qreal pinRadius = 10.;
    QPointF firstPoint = (m_pinA != nullptr) ? m_pinA->scenePos() : m_mousePos;
    QPointF secondPoint = (m_pinB != nullptr) ? m_pinB->scenePos() : m_mousePos;

    QPointF delta = secondPoint - firstPoint;
    qreal length = qSqrt(delta.x() * delta.x() + delta.y() * delta.y());
    QPointF dir = delta / length;
    QPointF ortho(-dir.y(), dir.x());

    QPainterPath path;
    path.moveTo(firstPoint + dir * pinRadius + ortho * width);
    path.lineTo(secondPoint - dir * pinRadius + ortho * width);
    path.lineTo(secondPoint - dir * pinRadius - ortho * width);
    path.lineTo(firstPoint + dir * pinRadius - ortho * width);
    path.closeSubpath();
    return path;
}
