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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QVector>
#include <QString>
#include <QtCore>

class ComponentInput;

class Component
{
	friend ComponentInput;

protected:
    QVector<ComponentInput> m_inputs;
    QString m_name;
	bool m_hasOutput = true;
    bool m_removable = true;

public:
    Component();
	virtual ~Component();

	virtual void init();

    bool isRemovable() { return m_removable; }
	bool hasOutput() { return m_hasOutput; }
    virtual qreal getOutput(qreal _time) = 0;

    ComponentInput* getInput(int _index);
    ComponentInput* getInput(QString _name);
    int getInputCount() { return m_inputs.size(); }

    QString getName() { return m_name; }

private:
	bool _hasAlreadyInput(Component* _comp);
};

class ComponentInput
{
	friend Component;

private:
	Component* m_pParent = nullptr;
	Component* m_pComponent = nullptr;
    qreal m_defaultValue;
    QString m_name;
    bool m_editable;
    bool m_linkable;

public:
    ComponentInput();
    ComponentInput(QString _name, Component* _parent = nullptr);

	Component* getParent() { return m_pParent; }
    QString getName() { return m_name; }
    qreal getValue(qreal _time);

	bool setComponent(Component* _comp);
	Component* getComponent() { return m_pComponent; }

    void setDefaultValue(qreal _value) { m_defaultValue = _value; }
    qreal getDefaultValue() { return m_defaultValue; }

    void setEditable(bool _editable) { m_editable = _editable; }
    bool getEditable() { return m_editable; }

    void setLinkable(bool _linkable) { m_linkable = _linkable; }
    bool getLinkable() { return m_linkable; }
};

#endif // COMPONENT_H
