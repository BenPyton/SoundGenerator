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

#include "ChangeInputValueCommand.h"
#include "PinInputItem.h"
#include "Components/Component.h"

ChangeInputValueCommand::ChangeInputValueCommand(PinInputItem* input, qreal value, QUndoCommand* _parent)
    : QUndoCommand ("Change Input Value", _parent), m_input(input)
{
    Q_ASSERT(m_input != nullptr);
    m_oldValue = m_input->input()->getDefaultValue();
    m_newValue = value;
}

void ChangeInputValueCommand::undo()
{
    QUndoCommand::undo();
    m_input->setDefaultValue(m_oldValue);
}

void ChangeInputValueCommand::redo()
{
    QUndoCommand::redo();
    m_input->setDefaultValue(m_newValue);
}

