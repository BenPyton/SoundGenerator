/*
 * Copyright 2020 Benoit Pelletier
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

#include <QtWidgets>
#include <QUndoStack>
#include <Utils.h>
#include "NodalScene.h"
#include "NodeItem.h"
#include "Components.h"
#include "PinInputItem.h"
#include "LinkItem.h"
#include "PinOutputItem.h"
#include "UndoCommands/CreateComponentCommand.h"
#include "UndoCommands/DeleteComponentCommand.h"
#include "UndoCommands/MoveComponentCommand.h"
#include "UndoCommands/SelectComponentCommand.h"

NodalScene::NodalScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_creationPosition(0, 0)
    , m_isContextMenu(false)
    , m_undoStack(nullptr)
{
    m_undoStack = new QUndoStack(this);
    // Create the output, that is a unique and non-removable component
    createNode("Output", 100.0);
    m_undoStack->clear();
}

NodalScene::~NodalScene()
{
}

NodeItem* NodalScene::createComponent(QString _componentName, qreal _width)
{
    if(m_isContextMenu)
    {
        m_creationPosition = m_contextPosition;
    }
    qDebug() << "Create Component";
    NodeItem* item = createNode(_componentName, _width);
    m_creationPosition += QPoint(20, 20);
    setDirty();
    return item;
}

void NodalScene::addNodeItem(NodeItem *_item)
{
    addItem(_item);
    m_nodeList.append(_item);
}

void NodalScene::removeNodeItem(NodeItem *_item)
{
    removeItem(_item);
    int itemIndex = m_nodeList.indexOf(_item);
    Q_ASSERT(itemIndex >= 0);
    m_nodeList.remove(itemIndex);
}

void NodalScene::save(QString fileName, const qreal duration)
{
    QJsonArray componentArray = NodeItem::NodeArrayToJson(m_nodeList);

    QJsonObject root;
    root["duration"] = duration;
    root["version"] = 1;
    root["components"] = componentArray;

    QJsonDocument json(root);
    QByteArray data = json.toJson();

    QFile file(fileName);
    file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(data);
    file.close();
}

void NodalScene::load(QString fileName, qreal& duration)
{
    QFile file(fileName);
    file.open(QIODevice::Text | QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject root = json.object();

    // ============== VERSION =============
    if(!Utils::CheckJsonValue(root, "version", QJsonValue::Double, 100))
        return;

    if(root["version"].toInt() != 1)
    {
        Utils::ErrorMsg(102, "Wrong version: project version is " + QString::number(root["version"].toInt()) + " and should be 1");
        return;
    }

    // ============== DURATION =============
    if(!Utils::CheckJsonValue(root, "duration", QJsonValue::Double, 110))
        return;

    duration = root["duration"].toDouble();

    reset();

    // ============== CREATE COMPONENTS =============
    if(!Utils::CheckJsonValue(root, "components", QJsonValue::Array, 120))
        return;
    QJsonArray componentArray = root["components"].toArray();

    NodeItem::JsonToNodeArray(componentArray, this, QPointF(), m_undoStack);
    m_undoStack->clear();
}

int NodalScene::clearItems(int from)
{
    int nbRemoved = 0;
    for(int i = m_nodeList.size()-1; i >= from; --i)
    {
        m_nodeList[i]->unlink();
        delete m_nodeList[i];
        m_nodeList.remove(i);
        nbRemoved++;
    }

    return nbRemoved;
}

void NodalScene::reset()
{
    clearItems(1);
    m_nodeList[0]->setX(0);
    m_nodeList[0]->setY(0);
    m_nodeList[0]->setSelected(false);
}


int NodalScene::deleteSelection()
{
    int nbRemoved = 0;

    m_undoStack->beginMacro("Delete Selection");
    for(QGraphicsItem* item : selectedItems())
    {
        NodeItem* node = qgraphicsitem_cast<NodeItem*>(item);
        if(node != nullptr && (node->component() == nullptr || node->component()->isRemovable()))
        {
            node->unlink();
            m_undoStack->push(new DeleteComponentCommand(this, node, node->pos()));
            nbRemoved++;
        }
    }

    if(nbRemoved > 0)
    {
        setDirty();
    }

    m_undoStack->endMacro();
    return nbRemoved;
}

void NodalScene::selectAll()
{
    QUndoCommand* macroCommand = new QUndoCommand("Select All");
    for(QGraphicsItem* item : items())
    {
        if((item->flags() & QGraphicsItem::ItemIsSelectable) != 0)
        {
            if(!item->isSelected())
            {
                new SelectComponentCommand(item, true, macroCommand);
            }
        }
    }

    if(macroCommand->childCount() > 0)
    {
        m_undoStack->push(macroCommand);
    }
}

void NodalScene::deselectAll()
{
    QUndoCommand* macroCommand = new QUndoCommand("Deselect All");
    for(QGraphicsItem* item : items())
    {
        if((item->flags() & QGraphicsItem::ItemIsSelectable) != 0)
        {
            if(item->isSelected())
            {
                new SelectComponentCommand(item, false, macroCommand);
            }
        }
    }

    if(macroCommand->childCount() > 0)
    {
        m_undoStack->push(macroCommand);
    }
}

void NodalScene::copyComponents()
{
    m_nextPastePosition = QPoint(0, 0);

    // get all selected components except the output
    QVector<NodeItem*> selectedNodes;
    for(int i = 0; i < m_nodeList.size(); i++)
    {
        if(m_nodeList[i]->isSelected() && m_nodeList[i]->component()->getName() != "Output")
        {
            qDebug() << "Copy: " << m_nodeList[i]->component()->getName();
            selectedNodes.append(m_nodeList[i]);
        }
    }

    // create a json array with all selected components
    QJsonArray componentArray = NodeItem::NodeArrayToJson(selectedNodes);

    QJsonObject root;
    root["components"] = componentArray;

    QJsonDocument json(root);
    QByteArray data = json.toJson();

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("sound/component", data);

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void NodalScene::pasteComponents()
{
    m_nextPastePosition += QPoint(20, 20);

    QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();

    QJsonDocument json = QJsonDocument::fromJson(mimeData->data("sound/component"));

    QJsonObject root = json.object();
    if(!Utils::CheckJsonValue(root, "components", QJsonValue::Array, 120))
        return;
    QJsonArray componentArray = root["components"].toArray();

    if(componentArray.size() > 0)
    {
        deselectAll();
        NodeItem::JsonToNodeArray(componentArray, this, m_nextPastePosition, m_undoStack);
    }
}

void NodalScene::undoCommand()
{
    m_undoStack->undo();
}

void NodalScene::redoCommand()
{
    m_undoStack->redo();
}

void NodalScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        m_previousSelectedItems.clear();
        for(int i = 0; i < selectedItems().size(); ++i)
        {
            m_previousSelectedItems.append(selectedItems()[i]);
        }
    }

    QGraphicsScene::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_movingItemPositions.clear();
        m_movingItems.clear();
        for(int i = 0; i < selectedItems().size(); ++i)
        {
            m_movingItems.append(selectedItems()[i]);
            m_movingItemPositions.append(selectedItems()[i]->pos());
        }
    }
}


void NodalScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        // Create undo commands for selection
        QUndoCommand* selectMacroCommand = new QUndoCommand("Select Items");
        bool areItemsSelected = false;
        for(int i = 0; i < selectedItems().size(); ++i)
        {
            if(!m_previousSelectedItems.contains(selectedItems()[i]))
            {
                new SelectComponentCommand(selectedItems()[i], true, selectMacroCommand);
                areItemsSelected = true;
            }
        }
        for(int i = 0; i < m_previousSelectedItems.size(); ++i)
        {
            if(!selectedItems().contains(m_previousSelectedItems[i]))
            {
                new SelectComponentCommand(m_previousSelectedItems[i], false, selectMacroCommand);
            }
        }

        if(selectMacroCommand->childCount() > 0)
        {
            if(!areItemsSelected)
            {
                selectMacroCommand->setText("Deselect Items");
            }
            m_undoStack->push(selectMacroCommand);
        }
        else
        {
            delete selectMacroCommand;
        }

        // Create undo commands for movement
        if(m_movingItems.size() > 0)
        {
            QUndoCommand* moveMacroCommand = new QUndoCommand();
            int nbMovedItem = 0;
            for(int i = 0; i < m_movingItems.size(); ++i)
            {
                NodeItem* node = static_cast<NodeItem*>(m_movingItems[i]);
                if(node != nullptr && Utils::DistanceSqr(node->pos(), m_movingItemPositions[i]) > qreal(0.01f))
                {
                    new MoveComponentCommand(node, m_movingItemPositions[i], moveMacroCommand);
                    nbMovedItem++;
                }
            }

            if(nbMovedItem > 0)
            {
                m_undoStack->push(moveMacroCommand);
            }
            else
            {
                delete moveMacroCommand;
            }
        }
    }
}

NodeItem *NodalScene::createNode(QString componentName, qreal width)
{
    CreateComponentCommand* command = new CreateComponentCommand(this, componentName, m_creationPosition, width);
    m_undoStack->push(command);
    command->getItem()->setUndoStack(m_undoStack);
    return command->getItem();
}
