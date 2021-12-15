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

#include "SuperCompTabBar.h"
#include "Components/SuperComponentData.h"
#include <QVariant>

SuperCompTabBar::SuperCompTabBar(QWidget *_parent)
    : QTabBar(_parent)
{
    connect(this, &QTabBar::tabCloseRequested, this, &QTabBar::removeTab);
}

int SuperCompTabBar::getTabIndex(SuperComponentData *_data)
{
    Q_ASSERT(_data != nullptr);

    int tabIndex = -1;
    QUuid uuid = _data->uuid();
    for(int i = 0; tabIndex < 0 && i < count(); ++i)
    {
        if(tabData(i).toUuid() == uuid)
        {
            tabIndex = i;
        }
    }
    return tabIndex;
}

QUuid SuperCompTabBar::getTabDataUuid(int _index)
{
    Q_ASSERT(_index >= 0 && _index < count());

    QUuid uuid = tabData(_index).toUuid();

    return uuid;
}

void SuperCompTabBar::editTab(SuperComponentData* _data)
{
    Q_ASSERT(_data != nullptr);

    int tabIndex = getTabIndex(_data);
    if(tabIndex < 0)
    {
        tabIndex = QTabBar::addTab(_data->name());
        // setCurrentIndex is done inside addTab for the first added tab : no uuid is set during this signal
        setTabData(tabIndex, _data->uuid());
    }
    setCurrentIndex(tabIndex);
}

void SuperCompTabBar::renameTab(SuperComponentData* _data)
{
    Q_ASSERT(_data != nullptr);

    int tabIndex = getTabIndex(_data);
    if(tabIndex >= 0)
    {
        setTabText(tabIndex, _data->name());
    }
}

void SuperCompTabBar::deleteTab(SuperComponentData* _data)
{
    Q_ASSERT(_data != nullptr);

    int tabIndex = getTabIndex(_data);
    if(tabIndex >= 0)
    {
        QTabBar::removeTab(tabIndex);
    }
}

