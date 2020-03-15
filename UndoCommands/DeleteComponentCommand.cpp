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

#include "DeleteComponentCommand.h"
#include "NodeItem.h"
#include "NodalScene.h"
#include <QtCore>

DeleteComponentCommand::DeleteComponentCommand(NodalScene* _scene, NodeItem* _item, QPointF _position, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_scene(_scene), m_item(_item), m_position(_position), m_isOwner(false)
{
    Q_ASSERT(m_scene != nullptr);
    Q_ASSERT(m_item != nullptr);
}

DeleteComponentCommand::~DeleteComponentCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void DeleteComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setPos(m_position);
    m_scene->addNodeItem(m_item);
    QObject::connect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_isOwner = false;
}

void DeleteComponentCommand::redo()
{
    QUndoCommand::redo();
    QObject::disconnect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_scene->removeNodeItem(m_item);
    m_isOwner = true;
}

