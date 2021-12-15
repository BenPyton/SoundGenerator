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

#include "SuperComponentButton.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <qevent.h>
#include <QtCore>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include "../Components/SuperComponentData.h"

SuperComponentButton::SuperComponentButton(SuperComponentData* _data, QWidget *_parent)
    : PushOrDragButton(_parent), m_data(_data)
{
    init();
}

SuperComponentButton::SuperComponentButton(SuperComponentData* _data, const QString &_text, QWidget *_parent)
    : PushOrDragButton(_text, _parent), m_data(_data)
{
    init();
}

SuperComponentButton::SuperComponentButton(SuperComponentData* _data, const QIcon &_icon, const QString &_text, QWidget *_parent)
    : PushOrDragButton(_icon, _text, _parent), m_data(_data)
{
    init();
}

SuperComponentButton::~SuperComponentButton()
{
}

void SuperComponentButton::resizeEvent(QResizeEvent *_event)
{
    PushOrDragButton::resizeEvent(_event);
    updateNameEditPosition();
}

QByteArray SuperComponentButton::dragData()
{
    Q_ASSERT(m_data != nullptr);
    return m_data->uuid().toByteArray();
}

void SuperComponentButton::updateNameEditPosition()
{
    int parentWidth = size().width();
    int parentHeight = size().height();
    int width = m_nameEdit->size().width();
    int height = m_nameEdit->size().height();
    m_nameEdit->move((parentWidth - width)/2, (parentHeight - height)/2);
}

void SuperComponentButton::showCustomContextMenu(const QPoint& _point)
{
    // for most widgets
    QPoint globalPos = mapToGlobal(_point);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QAction act_rename("Rename");
    QAction act_edit("Edit");
    QAction act_delete("Delete");

    QMenu myMenu;
    myMenu.addAction(&act_rename);
    myMenu.addAction(&act_edit);
    myMenu.addAction(&act_delete);

    // don't show menu if no action inside
    if(myMenu.isEmpty())
        return;

    QAction* selectedAction = myMenu.exec(globalPos);
    if (selectedAction == &act_rename)
    {
        qDebug() << "Rename!!!";
        m_nameEdit->setVisible(true);
        m_nameEdit->setFocus(Qt::FocusReason::OtherFocusReason);
    }
    else if (selectedAction == &act_edit)
    {
        qDebug() << "Edit!!!";
        emit onEditCalled(m_data);
    }
    else if (selectedAction == &act_delete)
    {
        qDebug() << "Delete!!!";
        emit onDeleteCalled(this);
    }
    else
    {
        // nothing was chosen
    }
}

void SuperComponentButton::onNameEdited()
{
    qDebug() << "Name Edited";
    m_nameEdit->clearFocus();
    m_nameEdit->setVisible(false);
    setText(m_nameEdit->text());
    m_data->setName(m_nameEdit->text());
}

void SuperComponentButton::init()
{
    Q_ASSERT(m_data != nullptr);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &SuperComponentButton::showCustomContextMenu);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setMaxLength(16);
    m_nameEdit->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    updateNameEditPosition();
    m_nameEdit->setVisible(false);
    connect(m_nameEdit, &QLineEdit::editingFinished, this, &SuperComponentButton::onNameEdited);
}
