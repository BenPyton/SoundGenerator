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

#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>
#include "types.h"

class PinInputItem;
class PinOutputItem;
class Component;

class NodeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum { Type = UserType + NODE_TYPE };
    int type() const override { return Type; }

    NodeItem(QGraphicsItem *parent = nullptr);
    virtual ~NodeItem() override;

    void setComponent(Component* comp);
    Component* component() { return m_component; }

    PinOutputItem* getOutput() { return m_outputPin; }
    PinInputItem* getInput(QString name);
    PinInputItem *getInput(int index);
    int getInputCount() { return m_inputPins.size(); }

    qreal width() { return m_width; }
    void setWidth(qreal width) { m_width = width; }

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget) override;
    virtual QRectF boundingRect() const override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void clearInputs();

protected slots:
    void setDirty() { emit dirtyChanged(); }

signals:
    void dirtyChanged();

private:
    qreal m_width;
    qreal m_height;
    Component* m_component;

    QGraphicsTextItem* m_label;
    QVector<PinInputItem*> m_inputPins;
    PinOutputItem* m_outputPin;
};

#endif // NODEITEM_H
