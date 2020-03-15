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

#ifndef CHANGEINPUTVALUECOMMAND_H
#define CHANGEINPUTVALUECOMMAND_H

#include <QUndoCommand>

class PinInputItem;

class ChangeInputValueCommand : public QUndoCommand
{
public:
    ChangeInputValueCommand(PinInputItem* input, qreal value, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;

private:
    PinInputItem* m_input;
    qreal m_oldValue;
    qreal m_newValue;
};

#endif // CHANGEINPUTVALUECOMMAND_H
