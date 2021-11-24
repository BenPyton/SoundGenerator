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

#ifndef UNLINKPINCOMMAND_H
#define UNLINKPINCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>

class PinItem;
class LinkItem;

class UnlinkPinCommand : public QUndoCommand
{
public:
    UnlinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent = nullptr);
    virtual ~UnlinkPinCommand() override;

    void undo() override;
    void redo() override;

private:
    LinkItem* m_item;
    QGraphicsScene* m_scene;
    PinItem* m_pinA;
    PinItem* m_pinB;
    bool m_isOwner;
};

#endif // UNLINKPINCOMMAND_H
