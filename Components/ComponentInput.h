/*
 * Copyright 2019, 2021 Benoit Pelletier
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

#ifndef COMPONENTINPUT_H
#define COMPONENTINPUT_H

#include <QString>

class Component;

class ComponentInput
{
public:
    ComponentInput(QString _name, Component* _parent = nullptr);
    virtual ~ComponentInput();

    inline Component* getParent() const { return m_pParent; }
    virtual QString getName() const;

    inline void setComponent(Component* _comp) { m_pComponent = _comp; }
    inline Component* getComponent() const { return m_pComponent; }

    inline void setDefaultValue(qreal _value) { m_defaultValue = _value; }
    inline qreal getDefaultValue() const { return m_defaultValue; }

    inline void setEditable(bool _editable) { m_editable = _editable; }
    inline bool getEditable() const { return m_editable; }

    inline void setLinkable(bool _linkable) { m_linkable = _linkable; }
    inline bool getLinkable() const { return m_linkable; }

    bool canSetComponent(Component* _comp);
    virtual qreal getValue(qreal _time);

protected:
    Component* m_pParent = nullptr;
    Component* m_pComponent = nullptr;
    qreal m_defaultValue;
    QString m_name;
    bool m_editable;
    bool m_linkable;
};

#endif // COMPONENTINPUT_H
