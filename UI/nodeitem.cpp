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

#include "nodeitem.h"
#include "nodalview.h"
#include "pinitem.h"
#include "pininputitem.h"
#include "pinoutputitem.h"
#include <QWidget>
#include <QtWidgets>
#include <QPainter>

NodeItem::NodeItem(QGraphicsItem *parent)
    : QGraphicsItem(parent), m_width(200), m_height(100), m_component(nullptr)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

//    m_inputPin = new PinInputItem(this);
//    m_inputPin->setX(10);
//    m_inputPin->setY(50);
    m_outputPin = new PinOutputItem(this);
    m_outputPin->setX(190);
    m_outputPin->setY(50);
    connect(m_outputPin, SIGNAL(dirtyChanged()), this, SLOT(setDirty()));

    m_label = new QGraphicsTextItem(this);
    m_label->setPlainText("Component Title");
    m_label->setFont(QFont("consolas", 10));
    m_label->setDefaultTextColor(QColor(200, 200, 200));
}

NodeItem::~NodeItem()
{
    m_outputPin->unlinkAll();
    for(PinInputItem* input : m_inputPins)
    {
        input->unlinkAll();
    }

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

PinInputItem *NodeItem::getInput(QString name)
{
    PinInputItem* input = nullptr;

    for (int i = 0; i < m_inputPins.size() && input == nullptr; i++)
    {
        if (m_inputPins[i]->input()->getName() == name)
        {
            input = m_inputPins[i];
        }
    }

    return input;
}

PinInputItem *NodeItem::getInput(int index)
{
    PinInputItem* input = nullptr;
    if(index >= 0 && index < m_inputPins.size())
    {
        input = m_inputPins[index];
    }
    return input;
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

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    QVariant toReturn = value;

    if (change == ItemPositionChange) {
        QPointF newPos = value.toPointF();
        if(QGuiApplication::mouseButtons() == Qt::LeftButton
          && QGuiApplication::keyboardModifiers() == Qt::ShiftModifier/* && nullptr != view*/){

            int gridSize = NodalView::gridUnit;
            qreal xV = round(newPos.x()/gridSize)*gridSize;
            qreal yV = round(newPos.y()/gridSize)*gridSize;
            toReturn = QPointF(xV, yV);
        }
        else
            toReturn = newPos;

//        for(QGraphicsItem* item : childItems())
//        {
//            PinItem* pin = qgraphicsitem_cast<PinItem*>(item);
//            if(pin != nullptr)
//            {
//                pin->updateLinks();
//            }
//        }
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
        delete pin;
    }
    m_inputPins.clear();
}


QJsonArray NodeItem::NodeArrayToJson(const QVector<NodeItem*> &nodeArray)
{
    // create a json array with all selected components
    QJsonArray jsonArray;
    for(int i = 0; i < nodeArray.size(); i++)
    {
        QJsonArray inputArray;
        for(int k = 0; k < nodeArray[i]->getInputCount(); k++)
        {
            PinInputItem* pin = nodeArray[i]->getInput(k);

            Component* link = pin->input()->getComponent();
            int linkIndex = -1;
            if(link != nullptr)
            {
                for(int p = 0; p < nodeArray.size() && linkIndex < 0; p++)
                {
                    if(nodeArray[p]->component() == link)
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
        component["x"] = nodeArray[i]->x();
        component["y"] = nodeArray[i]->y();
        component["name"] = nodeArray[i]->component()->getName();
        component["inputs"] = inputArray;

        jsonArray.append(component);
    }

    return jsonArray;
}

QVector<NodeItem*> NodeItem::JsonToNodeArray(const QJsonArray &jsonArray)
{
    Q_UNUSED(jsonArray);
    QVector<NodeItem*> nodeArray;
    /*
    // first create all components
    for(int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject component = jsonArray[i].toObject();

        qDebug() << "Node at: " << i;

        // ============== COMPONENT NAME =============
        if(Utils::CheckJsonValue(component, "name", QJsonValue::String, 130))
        {
            qDebug() << "Node name: " << component["name"].toString();
            if(component["name"].toString() != "Output")
            {
                NodeItem* node = createNode(component["name"].toString());
                node->setSelected(true);
                nodeArray.append(node);

                // ============== COMPONENT COORDINATES =============

                if(Utils::CheckJsonValue(component, "x", QJsonValue::Double, 140))
                    node->setX(component["x"].toDouble() + m_nextPastePosition.x());

                if(Utils::CheckJsonValue(component, "y", QJsonValue::Double, 150))
                    node->setY(component["y"].toDouble() + m_nextPastePosition.y());

                qDebug() << "Append node: " << component["name"].toString();

            }
        }
    }

    qDebug() << "Nb pasted node: " << nodeArray.size();

    // then set values and connect pins of each component
    for(int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject component = jsonArray[i].toObject();
        if(!Utils::CheckJsonValue(component, "inputs", QJsonValue::Array, 160))
            continue;

        QJsonArray inputs = component["inputs"].toArray();

        if(i >= nodeArray.size())
        {
            Utils::ErrorMsg(1000, "ERROR index #" + QString::number(i) + " doesn't correspond to pasted node");
            continue;
        }

        for(int k = 0; k < inputs.size(); k++)
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
                LinkItem* link = new LinkItem();
                scene()->addItem(link);
                link->setFirstPin(pin);
                pin->addLink(link);

                PinOutputItem* otherPin = nodeArray[linkIndex]->getOutput();

                if(!pin->connect(otherPin, link))
                {
                    delete link;
                }
            }
        }
    }*/
    return nodeArray;
}
