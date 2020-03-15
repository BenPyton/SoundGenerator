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

#include "PushOrDragButton.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <qevent.h>
#include <QtCore>

PushOrDragButton::PushOrDragButton(QWidget *parent)
    : QPushButton(parent)
{ }

PushOrDragButton::PushOrDragButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{ }

PushOrDragButton::PushOrDragButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{ }

void PushOrDragButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();
    qDebug() << "click";
}

void PushOrDragButton::mouseMoveEvent(QMouseEvent *event)
{
    QPushButton::mouseMoveEvent(event);

    if (!(event->buttons() & Qt::LeftButton))
            return;
    if ((event->pos() - m_dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    qDebug() << "drag";

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData();

    mimeData->setData("add/component", text().toUtf8());
    drag->setMimeData(mimeData);

    drag->exec(Qt::MoveAction);

    setDown(false);
}
