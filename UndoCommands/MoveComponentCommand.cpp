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

#include "MoveComponentCommand.h"
#include "NodeItem.h"
#include "NodalScene.h"

MoveComponentCommand::MoveComponentCommand(NodeItem* _item, QPointF _oldPosition, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_item(_item), m_oldPosition(_oldPosition)
{
    Q_ASSERT(m_item != nullptr);
    m_newPosition = m_item->pos();
}

void MoveComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setPos(m_oldPosition);
}

void MoveComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setPos(m_newPosition);
}
