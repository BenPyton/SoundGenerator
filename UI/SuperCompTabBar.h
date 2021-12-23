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

#ifndef SUPERCOMPTABBAR_H
#define SUPERCOMPTABBAR_H

#include <QTabBar>

class SuperComponentData;

class SuperCompTabBar : public QTabBar
{
    Q_OBJECT

public:
    SuperCompTabBar(QWidget* _parent = nullptr);

    int getTabIndex(SuperComponentData* _data);
    QUuid getTabDataUuid(int _index);

public slots:
    void editTab(SuperComponentData* _data);
    void renameTab(SuperComponentData* _data);
    void closeTab(SuperComponentData* _data);
    void closeTabAt(int _index);

signals:
    void tabClosed(QUuid* _uuid);
};

#endif // SUPERCOMPTABBAR_H
