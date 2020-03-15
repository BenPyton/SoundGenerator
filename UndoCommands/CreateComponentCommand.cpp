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

#include "CreateComponentCommand.h"
#include "NodeItem.h"
#include "NodalScene.h"
#include "Components.h"

CreateComponentCommand::CreateComponentCommand(NodalScene* _scene, QString _componentName, QPointF _initialPos, qreal _width, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_scene(_scene), m_initialPos(_initialPos), m_isOwner(false)
{
    Q_ASSERT(m_scene != nullptr);
    Component* component = ComponentFactory::CreateComponent(_componentName);
    m_item = new NodeItem();
    m_item->setComponent(component);
    m_item->setWidth(_width);
}

CreateComponentCommand::~CreateComponentCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void CreateComponentCommand::undo()
{
    QUndoCommand::undo();
    QObject::disconnect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_scene->removeNodeItem(m_item);
    m_isOwner = true;
}

void CreateComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setPos(m_initialPos);
    m_scene->addNodeItem(m_item);
    QObject::connect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_isOwner = false;
}

