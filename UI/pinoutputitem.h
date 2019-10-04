/*
 * Copyright 2019 Benoit Pelletier
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

#ifndef PINOUTPUTITEM_H
#define PINOUTPUTITEM_H

#include "pinitem.h"
#include "Components/Component.h"

class PinOutputItem : public PinItem
{
public:
    enum { Type = UserType + PINOUTPUT_TYPE };
    int type() const override { return Type; }

    PinOutputItem(QGraphicsItem *parent = nullptr);

    void setComponent(Component* comp) { m_component = comp;}
    Component* component() { return m_component; }

protected:
    virtual bool _tryConnect(PinItem* other) override;

private:
    Component* m_component;
};

#endif // PINOUTPUTITEM_H
