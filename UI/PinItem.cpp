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

#include "PinItem.h"
#include "NodalView.h"
#include "LinkItem.h"
#include <QPainter>
#include <QWidget>
#include <QtWidgets>
#include "UndoCommands/LinkPinCommand.h"
#include "UndoCommands/UnlinkPinCommand.h"

LinkItem* PinItem::s_linkPreview = new LinkItem();

PinItem::PinItem(QGraphicsItem *parent)
    : QGraphicsItem(parent), m_pinVisible(true)
{
    m_radius = 5;
    m_maxLink = -1;
    m_linkedPins.clear();
    m_hovered = false;
    m_dragging = false;

    //setAcceptHoverEvents(true);
}

bool PinItem::link(PinItem *pin)
{
    bool linked = false;
    if(pin != nullptr && pin != this
        && !isLinkedWith(pin)
        && pin->_canConnect(this)
        && this->_canConnect(pin))
    {
        linked = true;
        undoStack()->push(new LinkPinCommand(scene(), this, pin));
        setDirty();
    }
    return linked;
}

void PinItem::unlinkAll()
{
    undoStack()->beginMacro("Unlink pin");
    for(PinItem* linkedPin : m_linkedPins)
    {
        undoStack()->push(new UnlinkPinCommand(scene(), this, linkedPin));
    }
    m_linkedPins.clear();
    setDirty();
    undoStack()->endMacro();
}

bool PinItem::isLinkedWith(PinItem *_pin)
{
    return m_linkedPins.indexOf(_pin) >= 0;
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(m_pinVisible)
    {
        QBrush brush(QColor(80, 80, 80), Qt::BrushStyle::SolidPattern);
        QPen pen(QColor(200, 200, 200));
        pen.setWidth(m_hovered ? 3 : 1);
        pen.setCosmetic(true);

        if(isLinked())
        {
            brush.setColor(QColor(200, 200, 200));
        }

        painter->setBrush(brush);
        painter->setPen(pen);
        painter->drawEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    }
}

QRectF PinItem::boundingRect() const
{
    return QRectF(-2.*m_radius, -2.*m_radius, 4.*m_radius, 4.*m_radius);
}

void PinItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_pinVisible)
    {
        if(event->button() == Qt::MouseButton::LeftButton && !hasMaxLink())
        {
            qDebug() << "Pin Clicked!";
            m_dragging = true;
            s_linkPreview->setMousePos(event->scenePos());
            _showLinkPreview(true);
            update();
        }
        else if(event->button() == Qt::MouseButton::MiddleButton)
        {
            qDebug() << "Pin Reset!";
            unlinkAll();
            update();
        }
    }
}

void PinItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_pinVisible && event->button() == Qt::MouseButton::LeftButton)
    {
        if(m_dragging)
        {
            qDebug() << "Pin Release!";

            QList<QGraphicsItem*> items = scene()->items(event->scenePos());

            PinItem* pin = nullptr;
            for(int i = 0; i < items.size() && pin == nullptr; i++)
            {
                pin = dynamic_cast<PinItem*>(items[i]);
            }

            if(link(pin))
            {
                qDebug() << "Pin connected!";
                pin->update();
            }

            m_dragging = false;
            _showLinkPreview(false);
            update();
        }
    }
}

void PinItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_dragging)
    {
        s_linkPreview->setMousePos(event->scenePos());
        scene()->update();
        qDebug() << "Mouse move!";
    }
}

QPainterPath PinItem::shape() const
{
    QPainterPath path;
    path.addEllipse(-2.*m_radius, -2.*m_radius, 4.*m_radius, 4.*m_radius);
    return path;
}

//void PinItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    Q_UNUSED(event);
//    if(boundingRect().contains(event->scenePos()))
//    {
//        m_hovered = true;
//        //event->ignore(); // block children to reveive this event
//        update();
//    }
//}

//void PinItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    Q_UNUSED(event);
//    if(boundingRect().contains(event->lastScenePos()))
//    {
//        m_hovered = false;
//        //event->ignore(); // block children to reveive this event
//        update();
//    }
//}

bool PinItem::_canConnect(PinItem* _other)
{
    return nullptr != _other && !hasMaxLink();
}

void PinItem::_connect(PinItem *_other)
{
    if(m_linkedPins.size() == 0)
    {
        emit onLink();
    }
    m_linkedPins.append(_other);
}

void PinItem::_disconnect(PinItem *_other)
{
    int index = m_linkedPins.indexOf(_other);
    if(index >= 0)
    {
        m_linkedPins.removeAt(index);

        if(m_linkedPins.size() == 0)
        {
            emit onUnlink();
        }
    }
}

void PinItem::_showLinkPreview(bool show)
{
    if(show)
    {
        s_linkPreview->setPins(this, nullptr);
        scene()->addItem(s_linkPreview);
    }
    else
    {
        s_linkPreview->setPins(nullptr, nullptr);
        scene()->removeItem(s_linkPreview);
    }
    scene()->update();
}

