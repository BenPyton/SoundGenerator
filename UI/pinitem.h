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

#ifndef PINITEM_H
#define PINITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include "types.h"

class LinkItem;
class PinItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum { Type = UserType + PIN_TYPE };
    int type() const override { return Type; }

    PinItem(QGraphicsItem *parent = nullptr);

    bool isPinVisible() { return m_pinVisible; }
    void setPinVisible(bool visible) { m_pinVisible = visible; }

    bool isConnected() { return m_links.size() > 0; }
    void setMaxConnection(int maxConnection) { m_maxLink = maxConnection; }
    int maxConnection() { return m_maxLink; }

    void addLink(LinkItem* link);
    void removeLink(LinkItem* link);

    void updateLinks();

    bool connect(PinItem* pin, LinkItem* link);
    void disconnectAll();

protected slots:
    void setDirty() { emit dirtyChanged(); }

signals:
    void connected();
    void disconnected();
    void dirtyChanged();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget) override;
    virtual QRectF boundingRect() const override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    //virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    //virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    virtual bool _tryConnect(PinItem* other) { Q_UNUSED(other); return true; }
    virtual void _disconnect() {}

private:
    bool _canConnect();


protected:
    int m_maxLink;

private:
    bool m_pinVisible;
    bool m_hovered;
    int m_radius;
    LinkItem* m_currentLink;
    QList<LinkItem*> m_links;


};

#endif // PINITEM_H
