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

#ifndef CREATECOMPONENTCOMMAND_H
#define CREATECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class CreateComponentCommand : public QUndoCommand
{
public:
    CreateComponentCommand(NodalScene* _scene, QString _componentName, QPointF _initialPos, qreal _width, QUndoCommand* _parent = nullptr);
    ~CreateComponentCommand() override;

    void undo() override;
    void redo() override;

    inline NodeItem* getItem() const { return m_item; }

private:
    NodalScene* m_scene;
    NodeItem* m_item;
    QPointF m_initialPos;
    bool m_isOwner;
};

#endif // CREATECOMPONENTCOMMAND_H
