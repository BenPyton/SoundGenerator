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

#include "NodeItem.h"
#include "NodalView.h"
#include "NodalScene.h"
#include "PinItem.h"
#include "PinInputItem.h"
#include "PinOutputItem.h"
#include <QWidget>
#include <QtWidgets>
#include <QPainter>
#include "Utils.h"
#include "LinkItem.h"
#include "UndoCommands/CreateComponentCommand.h"
#include "UndoCommands/MoveComponentCommand.h"


NodeItem::NodeItem(QGraphicsItem* _parent)
    : QGraphicsItem(_parent)
    , m_width(200)
    , m_height(100)
    , m_component(nullptr)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    m_outputPin = new PinOutputItem(this);
    m_outputPin->setX(190);
    m_outputPin->setY(50);
    m_outputPin->setUndoStack(undoStack());
    connect(m_outputPin, SIGNAL(dirtyChanged()), this, SLOT(setDirty()));

    m_label = new QGraphicsTextItem(this);
    m_label->setPlainText("Component Title");
    m_label->setFont(QFont("consolas", 10));
    m_label->setDefaultTextColor(QColor(200, 200, 200));
}

NodeItem::~NodeItem()
{
    if(m_component != nullptr)
        delete m_component;
}

void NodeItem::setComponent(Component *comp)
{
    if(m_component != nullptr)
        delete m_component;

    m_component = comp;
    m_outputPin->setComponent(comp);

    clearInputs();

    qreal heightSum = 35;
    if (comp != nullptr)
    {
        m_label->setPlainText(comp->getName());

        for (int i = 0; i < comp->getInputCount(); i++)
        {
            PinInputItem* pin = new PinInputItem(this);

            pin->setInput(comp->getInput(i));
            pin->setX(10);
            pin->setY(heightSum);
            connect(pin, SIGNAL(dirtyChanged()), this, SLOT(setDirty()));

            m_inputPins.append(pin);
            heightSum += (comp->getInput(i)->getName().isEmpty() ? 20 : 40);

            connect(pin, &PinInputItem::outputChanged, this, &NodeItem::setOutputChanged);
        }

        m_outputPin->setVisible(comp->hasOutput());
    }
    else
    {
        m_label->setPlainText("NULL");
    }
    m_height = heightSum;
    // recenter the output pin
    m_outputPin->setY(0.5*(m_height-20)+20);
}

PinInputItem *NodeItem::getInput(QString _name)
{
    PinInputItem* input = nullptr;

    for (int i = 0; i < m_inputPins.size() && input == nullptr; i++)
    {
        if (m_inputPins[i]->input()->getName() == _name)
        {
            input = m_inputPins[i];
        }
    }

    return input;
}

PinInputItem *NodeItem::getInput(int _index)
{
    PinInputItem* input = nullptr;
    if(_index >= 0 && _index < m_inputPins.size())
    {
        input = m_inputPins[_index];
    }
    return input;
}

void NodeItem::unlink()
{
    undoStack()->beginMacro("Unlink Component");
    for(PinInputItem* input : m_inputPins)
    {
        input->unlinkAll();
    }
    m_outputPin->unlinkAll();
    undoStack()->endMacro();
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget)

    QBrush brush(QColor(80, 80, 80), Qt::BrushStyle::SolidPattern);
    QPen pen(QColor(200, 200, 200, 0));

    int radius = 5;

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRoundedRect(0, 0, qRound(m_width), qRound(m_height), 5, 5);

    brush.setColor(QColor(60, 60, 60));
    painter->setBrush(brush);

    int titleHeight = 20;

    QPainterPath path;
    path.moveTo(0, 2*radius);
    path.arcTo(0, 0, 2*radius, 2*radius, 180, -90);
    path.lineTo(qRound(m_width) - 2*radius, 0);
    path.arcTo(qRound(m_width) - 2*radius, 0, 2*radius, 2*radius, 90, -90);
    path.lineTo(qRound(m_width), titleHeight);
    path.lineTo(0, titleHeight);
    path.closeSubpath();

    painter->drawPath(path);

    pen.setColor(QColor(200, 200, 200));
    brush.setColor(QColor(0, 0, 0, 0));
    // Draw outline
    pen.setWidth(1);
    pen.setCosmetic(true);

    if(isSelected())
    {
        pen.setColor(QColor(200, 180, 0));
        pen.setWidth(3);
    }

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRoundedRect(0, 0, qRound(m_width), qRound(m_height), 5, 5);
}

QRectF NodeItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QVariant NodeItem::itemChange(GraphicsItemChange _change, const QVariant& _value)
{
    QVariant toReturn = _value;

    if (_change == ItemPositionChange)
    {
        QPointF newPos = _value.toPointF();
        if(QGuiApplication::mouseButtons() == Qt::LeftButton
          && QGuiApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            int gridSize = NodalView::gridUnit;
            qreal correctedX = round(newPos.x() / gridSize) * gridSize;
            qreal correctedY = round(newPos.y() / gridSize) * gridSize;
            toReturn = QPointF(correctedX, correctedY);
        }
        else
        {
            toReturn = newPos;
        }

        // not performant, but work well enough
        if(scene() != nullptr)
            scene()->update();

        setDirty();
    }

    return toReturn;
}

void NodeItem::clearInputs()
{
    for(PinInputItem* pin : m_inputPins)
    {
        disconnect(pin, &PinInputItem::outputChanged, this, &NodeItem::setOutputChanged);
        delete pin;
    }
    m_inputPins.clear();
}


QJsonArray NodeItem::NodeArrayToJson(const QVector<NodeItem*>& _nodeArray)
{
    // create a json array with all selected components
    QJsonArray jsonArray;
    for(int i = 0; i < _nodeArray.size(); i++)
    {
        QJsonArray inputArray;
        for(int k = 0; k < _nodeArray[i]->getInputCount(); k++)
        {
            PinInputItem* pin = _nodeArray[i]->getInput(k);

            Component* link = pin->input()->getComponent();
            int linkIndex = -1;
            if(link != nullptr)
            {
                for(int p = 0; p < _nodeArray.size() && linkIndex < 0; p++)
                {
                    if(_nodeArray[p]->component() == link)
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
        component["x"] = _nodeArray[i]->x();
        component["y"] = _nodeArray[i]->y();
        component["name"] = _nodeArray[i]->component()->getName();
        component["inputs"] = inputArray;

        jsonArray.append(component);
    }

    return jsonArray;
}

QVector<NodeItem*> NodeItem::JsonToNodeArray(
        const QJsonArray& _jsonArray,
        NodalScene* _scene,
        QPointF _positionOffset,
        QUndoStack* _commandStack)
{
    Q_ASSERT(nullptr != _commandStack);
    QVector<NodeItem*> nodeArray;

    _commandStack->beginMacro("Create Components");

    // first create all components
    for(int i = 0; i < _jsonArray.size(); i++)
    {
        QJsonObject component = _jsonArray[i].toObject();
        NodeItem* node = nullptr;

        // ============== COMPONENT NAME =============
        if(Utils::CheckJsonValue(component, "name", QJsonValue::String, 130))
        {
            QPointF position;
            // ============== COMPONENT COORDINATES =============
            if(Utils::CheckJsonValue(component, "x", QJsonValue::Double, 140))
                position.setX(component["x"].toDouble() + _positionOffset.x());
            if(Utils::CheckJsonValue(component, "y", QJsonValue::Double, 150))
                position.setY(component["y"].toDouble() + _positionOffset.y());

            if(component["name"].toString() == "Output")
            {
                node = _scene->getOutput();
                node->setPos(position);
            }
            else
            {
                CreateComponentCommand *command = new CreateComponentCommand(_scene, component["name"].toString(), position, 200.f);
                _commandStack->push(command);
                node = command->getItem();
                node->setUndoStack(_commandStack);
            }

            nodeArray.append(node);
        }
    }

    if(nodeArray.size() > 0)
    {
        // then set values and connect pins of each component
        for(int i = 0; i < _jsonArray.size(); i++)
        {
            if(i >= nodeArray.size())
            {
                Utils::ErrorMsg(1000, "ERROR index #" + QString::number(i) + " doesn't correspond to pasted node");
                continue;
            }
            nodeArray[i]->setSelected(true);

            QJsonObject component = _jsonArray[i].toObject();
            if(!Utils::CheckJsonValue(component, "inputs", QJsonValue::Array, 160))
                continue;
            QJsonArray inputs = component["inputs"].toArray();

            for(int k = 0; k < inputs.size(); ++k)
            {
                QJsonObject input = inputs[k].toObject();
                if(!Utils::CheckJsonValue(input, "value", QJsonValue::Double, 170))
                    continue;
                PinInputItem* pin = nodeArray[i]->getInput(k);
                pin->setDefaultValue(input["value"].toDouble());

                if(!Utils::CheckJsonValue(input, "link", QJsonValue::Double, 180))
                    continue;
                int linkIndex = input["link"].toInt();
                if(linkIndex >= 0 && linkIndex < nodeArray.size())
                {
                    PinOutputItem* otherPin = nodeArray[linkIndex]->getOutput();
                    pin->link(otherPin);
                }
            }
        }
    }

    _commandStack->endMacro();

    return nodeArray;
}

void NodeItem::setUndoStack(QUndoStack *_undoStack)
{
    m_undoStack = _undoStack;
    m_outputPin->setUndoStack(m_undoStack);
    for(PinInputItem* input : m_inputPins)
    {
        input->setUndoStack(m_undoStack);
    }
}
