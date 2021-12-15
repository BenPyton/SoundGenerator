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

#ifndef SUPERCOMPONENTBUTTON_H
#define SUPERCOMPONENTBUTTON_H

#include "PushOrDragButton.h"

class QLineEdit;
class SuperComponentData;

class SuperComponentButton : public PushOrDragButton
{
    Q_OBJECT

public:
    SuperComponentButton(SuperComponentData* _data, QWidget *_parent = nullptr);
    SuperComponentButton(SuperComponentData* _data, const QString &_text, QWidget *_parent = nullptr);
    SuperComponentButton(SuperComponentData* _data, const QIcon &_icon, const QString &_text, QWidget *_parent = nullptr);
    virtual ~SuperComponentButton() override;

    inline SuperComponentData* data() { return m_data; }

protected:
    virtual void resizeEvent(QResizeEvent* _event) override;
    virtual QByteArray dragData() override;

    void updateNameEditPosition();

public slots:
    void showCustomContextMenu(const QPoint& _point);
    void onNameEdited();

signals:
    void onEditCalled(SuperComponentData* _data);
    void onDeleteCalled(SuperComponentButton* _button);

private:
    void init();

private:
    QLineEdit* m_nameEdit;
    SuperComponentData* m_data;
};

#endif // SUPERCOMPONENTBUTTON_H
