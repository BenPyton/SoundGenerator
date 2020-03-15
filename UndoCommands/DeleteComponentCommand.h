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

#ifndef DELETECOMPONENTCOMMAND_H
#define DELETECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class DeleteComponentCommand : public QUndoCommand
{
public:
    DeleteComponentCommand(NodalScene* _scene, NodeItem* _item, QPointF _position, QUndoCommand* _parent = nullptr);
    ~DeleteComponentCommand() override;

    void undo() override;
    void redo() override;

    inline NodeItem* getItem() { return m_item; }

private:
    NodalScene* m_scene;
    NodeItem* m_item;
    QPointF m_position;
    bool m_isOwner;
};

#endif // DELETECOMPONENTCOMMAND_H
