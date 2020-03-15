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

#include "ActionCycle.h"
#include <QIcon>

ActionCycle::ActionCycle(QObject *_parent)
    : QAction(_parent), m_currentIndex(0)
{
    connect(this, SIGNAL(triggered()), this, SLOT(onTrigger()));
}

void ActionCycle::addAction(const QString &_text)
{
    m_texts.append(_text);
    m_icons.append(QIcon());
}

void ActionCycle::addAction(const QString &_text, const QIcon &_icon)
{
    m_texts.append(_text);
    m_icons.append(_icon);
}

void ActionCycle::setIndex(int _index)
{
    if(_index < 0 || _index >= m_texts.size())
        return;
    m_currentIndex = _index;
    _updateAction();
}

void ActionCycle::_updateAction()
{
    setText(m_texts[m_currentIndex]);
    setIcon(m_icons[m_currentIndex]);
}

void ActionCycle::onTrigger()
{
    if(m_texts.size() <= 0)
        return;

    emit triggered(m_currentIndex);
    m_currentIndex = (m_currentIndex + 1) % m_texts.size();
    _updateAction();
}
