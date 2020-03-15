/*
 * Copyright 2019-2020 Benoit Pelletier
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

#ifndef PUSHORDRAGBUTTON_H
#define PUSHORDRAGBUTTON_H

#include <QPushButton>

class PushOrDragButton : public QPushButton
{
public:
    PushOrDragButton(QWidget *parent = nullptr);
    PushOrDragButton(const QString &text, QWidget *parent = nullptr);
    PushOrDragButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPointF m_dragStartPosition;
};

#endif // PUSHORDRAGBUTTON_H
