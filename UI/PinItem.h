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

#ifndef PINITEM_H
#define PINITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include "Types.h"
#include <QUndoStack>

class LinkPinCommand;
class UnlinkPinCommand;
class LinkItem;
class NodeItem;

class PinItem : public QObject, public QGraphicsItem
{
    friend LinkPinCommand;
    friend UnlinkPinCommand;

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum { Type = UserType + PIN_TYPE };
    int type() const override { return Type; }

    PinItem(QGraphicsItem* parent = nullptr);

    bool isPinVisible() { return m_pinVisible; }
    void setPinVisible(bool _visible) { m_pinVisible = _visible; }

    inline bool isLinked() { return m_linkedPins.size() > 0; }
    void setMaxLink(int _maxLink) { m_maxLink = _maxLink; }
    inline int maxLink() { return m_maxLink; }
    bool hasMaxLink() { return m_maxLink >= 0 && m_linkedPins.size() >= m_maxLink; }
    bool link(PinItem* _pin);
    bool unlink(PinItem* _pin);
    void unlinkAll();
    bool isLinkedWith(PinItem* _pin);
    void check();

    void setUndoStack(QUndoStack* _undoStack) { m_undoStack = _undoStack; }

    NodeItem* parentNode();

protected slots:
    void setDirty() { emit dirtyChanged(); }

signals:
    void onLink();
    void onUnlink();
    void dirtyChanged();

protected:
    virtual void paint(QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget) override;
    virtual QRectF boundingRect() const override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* _event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* _event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* _event) override;

    virtual bool _canConnect(PinItem* _other);
    virtual void _connect(PinItem* _other);
    virtual void _disconnect(PinItem* _other);
    virtual QPainterPath shape() const override;

    inline QUndoStack* undoStack() { return m_undoStack; }

private:
    void _showLinkPreview(bool _show);

protected:
    int m_maxLink;

private:
    QUndoStack* m_undoStack;
    bool m_pinVisible;
    bool m_hovered;
    int m_radius;
    QList<PinItem*> m_linkedPins;
    bool m_dragging;

    static LinkItem* s_linkPreview;
};

#endif // PINITEM_H
