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

#include "SelectComponentCommand.h"
#include <QGraphicsItem>

SelectComponentCommand::SelectComponentCommand(QGraphicsItem* _item, bool _selected, QUndoCommand* _parent)
    : QUndoCommand ("Select Component", _parent)
    , m_item(_item)
    , m_selected(_selected)
{
    Q_ASSERT(nullptr != m_item);
}

void SelectComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setSelected(!m_selected);
}

void SelectComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setSelected(m_selected);
}
