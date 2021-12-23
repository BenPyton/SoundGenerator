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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QVector>
#include <QString>
#include <QDebug>

class ComponentInput;

class Component
{
    friend ComponentInput;

public:
    Component();
	virtual ~Component();

	virtual void init();

    inline bool isRemovable() const { return m_removable; }
    inline bool isRenamable() const { return m_renamable; }
    inline bool hasOutput() const { return m_hasOutput; }
    inline bool showTitle() const { return m_showTitle; }
    inline bool showBackground() const { return m_showBackground; }
    inline bool canDragPins() const { return m_canDragPins; }
    virtual qreal getOutput(qreal _time) = 0;

    ComponentInput* getInput(int _index);
    int getInputCount() const { return m_inputs.size(); }

    QString getName() const { return m_name; }
    void setName(QString _name);

protected:
    // Takes the ownership of the input
    void addInput(ComponentInput* _input);
    void swapInputArrays(QVector<ComponentInput*>& _array);

private:
    bool _hasAlreadyInput(Component* _comp);

protected:
    QVector<ComponentInput*> m_inputs;
    QString m_name;
    bool m_hasOutput = true;
    bool m_removable = true;
    bool m_showTitle = true;
    bool m_showBackground = true;
    bool m_canDragPins = true;
    bool m_renamable = false;
};

#endif // COMPONENT_H
