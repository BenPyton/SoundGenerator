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

#ifndef ACTIONCYCLE_H
#define ACTIONCYCLE_H

#include <QAction>

class ActionCycle : public QAction
{
    Q_OBJECT

public:
    ActionCycle(QObject* _parent = nullptr);

    void addAction(const QString &_text);
    void addAction(const QString &_text, const QIcon &_icon);
    void setIndex(int _index);

public slots:
    inline void reset() { setIndex(0); }

private :
    void _updateAction();

signals:
    void triggered(int _index);

private slots:
    void onTrigger();

private:
    int m_currentIndex;
    QList<QString> m_texts;
    QList<QIcon> m_icons;
};

#endif // ACTIONCYCLE_H
