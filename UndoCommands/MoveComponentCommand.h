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

#ifndef MOVECOMPONENTCOMMAND_H
#define MOVECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class MoveComponentCommand : public QUndoCommand
{
public:
    MoveComponentCommand(NodeItem* _item, QPointF _oldPosition, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;
    //bool mergeWith(const QUndoCommand *other) override;

    inline NodeItem* getItem() { return m_item; }

private:
    NodeItem* m_item;
    QPointF m_oldPosition;
    QPointF m_newPosition;
};

#endif // MOVECOMPONENTCOMMAND_H
