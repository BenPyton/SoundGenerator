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

#include "PinItem.h"
#include "NodalView.h"
#include "LinkItem.h"
#include <QPainter>
#include <QWidget>
#include <QtWidgets>
#include "UndoCommands/LinkPinCommand.h"
#include "UndoCommands/UnlinkPinCommand.h"
#include "NodeItem.h"
#include "Components/Component.h"

LinkItem* PinItem::s_linkPreview = new LinkItem();

PinItem::PinItem(QGraphicsItem* _parent)
    : QGraphicsItem(_parent)
    , m_pinVisible(true)
{
    m_radius = 5;
    m_maxLink = -1;
    m_linkedPins.clear();
    m_hovered = false;
    m_dragging = false;
}

bool PinItem::link(PinItem* _pin)
{
    bool linked = false;
    if(_pin != nullptr && _pin != this
        && !isLinkedWith(_pin)
        && _pin->_canConnect(this)
        && this->_canConnect(_pin))
    {
        linked = true;
        undoStack()->push(new LinkPinCommand(scene(), this, _pin));
        setDirty();
    }
    return linked;
}

bool PinItem::unlink(PinItem* _pin)
{
    bool unlinked = false;
    if(_pin != nullptr && _pin != this
       && isLinkedWith(_pin))
    {
        unlinked = true;
        undoStack()->push(new UnlinkPinCommand(scene(), this, _pin));
        setDirty();
    }
    return unlinked;
}

void PinItem::unlinkAll()
{
    undoStack()->beginMacro("Break All Pin Links");
    for(PinItem* linkedPin : m_linkedPins)
    {
        undoStack()->push(new UnlinkPinCommand(scene(), this, linkedPin));
    }
    m_linkedPins.clear();
    setDirty();
    undoStack()->endMacro();
}

bool PinItem::isLinkedWith(PinItem* _pin)
{
    return m_linkedPins.indexOf(_pin) >= 0;
}

void PinItem::check()
{
    qDebug() << "Check Pin Item";
    Q_ASSERT(m_linkedPins.count() <= 0);
    Q_ASSERT(scene() == nullptr);
}

NodeItem* PinItem::parentNode()
{
    return qgraphicsitem_cast<NodeItem*>(parentItem());
}

void PinItem::paint(QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget)
{
    Q_UNUSED(_option)
    Q_UNUSED(_widget)

    if(m_pinVisible)
    {
        bool isSelected = false;
        if (parentNode() != nullptr
            && parentNode()->component() != nullptr
            && parentNode()->isSelected()
            && !parentNode()->component()->showBackground())
        {
            isSelected = true;
        }

        QBrush brush(QColor(80, 80, 80), Qt::BrushStyle::SolidPattern);
        QPen pen(isSelected ? QColor(200, 180, 0) : QColor(200, 200, 200));
        pen.setWidth(m_hovered || isSelected ? 3 : 1);
        pen.setCosmetic(true);

        if(isLinked())
        {
            brush.setColor(QColor(200, 200, 200));
        }

        _painter->setBrush(brush);
        _painter->setPen(pen);
        _painter->drawEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
    }
}

QRectF PinItem::boundingRect() const
{
    return QRectF(-2.0 * m_radius, -2.0 * m_radius, 4.0 * m_radius, 4.0 * m_radius);
}

void PinItem::mousePressEvent(QGraphicsSceneMouseEvent* _event)
{
    if(m_pinVisible)
    {
        if(_event->button() == Qt::MouseButton::LeftButton && !hasMaxLink())
        {
            if (parentNode() == nullptr
                || (parentNode()->component() != nullptr
                && parentNode()->component()->canDragPins()))
            {
                qDebug() << "Pin left click: accept event";
                m_dragging = true;
                s_linkPreview->setMousePos(_event->scenePos());
                _showLinkPreview(true);
                update();
                _event->accept();
            }
            else
            {
                qDebug() << "Pin left click: ignore event (pin drag disabled)";
                _event->ignore();
            }
        }
        else if(_event->button() == Qt::MouseButton::MiddleButton)
        {
            qDebug() << "Pin middle click: accept event";
            unlinkAll();
            update();
        }
        else
        {
            qDebug() << "Pin click: Unhandled event";
            _event->ignore();
        }
    }
    else
    {
        qDebug() << "Pin not visible: ignore mouse events";
        _event->ignore();
    }
}

void PinItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* _event)
{
    if(m_pinVisible && _event->button() == Qt::MouseButton::LeftButton)
    {
        if(m_dragging)
        {
            qDebug() << "Pin Release!";

            QList<QGraphicsItem*> items = scene()->items(_event->scenePos());

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

void PinItem::mouseMoveEvent(QGraphicsSceneMouseEvent* _event)
{
    if(m_dragging)
    {
        s_linkPreview->setMousePos(_event->scenePos());
        scene()->update();
        //qDebug() << "Mouse move!";
    }
}

QPainterPath PinItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

bool PinItem::_canConnect(PinItem* _other)
{
    return nullptr != _other && !hasMaxLink();
}

void PinItem::_connect(PinItem* _other)
{
    if(m_linkedPins.size() == 0)
    {
        emit onLink();
    }
    m_linkedPins.append(_other);
}

void PinItem::_disconnect(PinItem* _other)
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

void PinItem::_showLinkPreview(bool _show)
{
    if(_show)
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

