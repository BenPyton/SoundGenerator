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

#include "pinitem.h"
#include "nodalview.h"
#include "linkitem.h"
#include <QPainter>
#include <QWidget>
#include <QtWidgets>

PinItem::PinItem(QGraphicsItem *parent)
    : QGraphicsItem(parent), m_pinVisible(true), m_currentLink(nullptr)
{
    m_radius = 5;
    m_maxLink = -1;
    m_links.clear();
    m_hovered = false;

    //setAcceptHoverEvents(true);
}

void PinItem::addLink(LinkItem *link)
{
    if(m_links.indexOf(link) == -1)
    {
        m_links.append(link);
        emit connected();
    }
}

void PinItem::removeLink(LinkItem *link)
{
    int index = m_links.indexOf(link);
    if(index >= 0)
    {
        m_links.removeAt(index);
        if(m_links.size() <= 0)
        {
            emit disconnected();
        }
    }
}

void PinItem::updateLinks()
{
    for(LinkItem* link : m_links)
    {
        link->update();
    }
}

bool PinItem::connect(PinItem *pin, LinkItem* link)
{
    bool connected = false;
    if(pin != nullptr && pin != this && pin->_canConnect())
    {
        // search for an existing link with this pin
        LinkItem* existingLink = nullptr;
        for(LinkItem* l : m_links)
        {
            if(l->firstPin() == pin || l->secondPin() == pin)
            {
                existingLink = l;
            }
        }

        // no existing pin, so connect to it
        if(existingLink == nullptr && _tryConnect(pin))
        {
            connected = true;
            link->setSecondPin(pin);
            pin->addLink(link);
            setDirty();
        }
    }

    return connected;
}

void PinItem::disconnectAll()
{
    _disconnect();
    for(LinkItem* link : m_links)
    {
        if(link != nullptr)
        {
            PinItem* pin = link->getPinThatIsNot(this);
            if(pin != nullptr)
            {
                pin->_disconnect();
            }
            link->unlink();
            delete link;
        }
    }
    m_links.clear();
    setDirty();
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

        if(isConnected())
        {
            brush.setColor(QColor(200, 200, 200));
        }

        painter->setBrush(brush);
        painter->setPen(pen);
        painter->drawEllipse(boundingRect());
    }
}

QRectF PinItem::boundingRect() const
{
    return QRectF(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
}

void PinItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_pinVisible)
    {
        if(event->button() == Qt::MouseButton::LeftButton && _canConnect())
        {
            qDebug() << "Pin Clicked!";

            m_currentLink = new LinkItem();
            scene()->addItem(m_currentLink);
            m_currentLink->setFirstPin(this);
            m_currentLink->setMousePos(event->scenePos());
            addLink(m_currentLink);

            update();
        }
        else if(event->button() == Qt::MouseButton::MiddleButton)
        {
            qDebug() << "Pin Reset!";
            disconnectAll();
            update();
        }
    }
}

void PinItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_pinVisible && event->button() == Qt::MouseButton::LeftButton)
    {
        if(m_currentLink != nullptr)
        {
            qDebug() << "Pin Release!";

            QList<QGraphicsItem*> items = scene()->items(event->scenePos());

            PinItem* pin = nullptr;
            for(int i = 0; i < items.size() && pin == nullptr; i++)
            {
                pin = dynamic_cast<PinItem*>(items[i]);
            }

            if(connect(pin, m_currentLink))
            {
                qDebug() << "Pin connected!";
                pin->update();
            }
            else
            {
                qDebug() << "link destroyed...";
                m_currentLink->unlink();
                delete m_currentLink;
            }

            m_currentLink = nullptr;
            update();
            scene()->update();
        }
    }
}

void PinItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_currentLink != nullptr)
    {
        m_currentLink->setMousePos(event->scenePos());
        update();
        qDebug() << "Mouse move!";
    }
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

bool PinItem::_canConnect()
{
    return m_maxLink < 0 || m_maxLink > m_links.size();
}

