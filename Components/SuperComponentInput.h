/*
 * Copyright 2021 Benoit Pelletier
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

#ifndef SUPERCOMPONENTINPUT_H
#define SUPERCOMPONENTINPUT_H

#include "ComponentInput.h"
#include <QUuid>

class InputComponent;

class SuperComponentInput : public ComponentInput
{
public:
    SuperComponentInput(InputComponent* _input, Component* _parent = nullptr);

    inline QUuid uuid() { return m_cachedUuid; }
    virtual qreal getValue(qreal _time) override;
    virtual QString getName() const override;

private:
    QUuid m_cachedUuid;
    InputComponent* m_inputComponent;
};

#endif // SUPERCOMPONENTINPUT_H
