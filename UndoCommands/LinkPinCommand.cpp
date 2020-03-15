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

#include "LinkPinCommand.h"
#include "NodalScene.h"
#include "LinkItem.h"
#include "PinInputItem.h"
#include "PinOutputItem.h"

LinkPinCommand::LinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent)
    : QUndoCommand (_parent)
    , m_scene(_scene)
    , m_pinA(_pinA)
    , m_pinB(_pinB)
    , m_isOwner(true)
{
    Q_ASSERT(nullptr != m_scene);
    Q_ASSERT(nullptr != m_pinA);
    Q_ASSERT(nullptr != m_pinB);
    m_item = new LinkItem();
}

LinkPinCommand::~LinkPinCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void LinkPinCommand::undo()
{
    QUndoCommand::undo();
    m_pinA->_disconnect(m_pinB);
    m_pinB->_disconnect(m_pinA);
    m_item->setPins(nullptr, nullptr);
    m_scene->removeItem(m_item);
    m_isOwner = true;
}

void LinkPinCommand::redo()
{
    QUndoCommand::redo();
    m_scene->addItem(m_item);
    m_item->setPins(m_pinA, m_pinB);
    m_pinA->_connect(m_pinB);
    m_pinB->_connect(m_pinA);
    m_isOwner = false;
}

