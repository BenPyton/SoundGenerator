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

#include "LineEditQReal.h"
#include "PinInputItem.h"
#include "PinItem.h"
#include <QGraphicsTextItem>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <qvalidator.h>
#include "PinOutputItem.h"
#include "Components/Component.h"
#include "UndoCommands/ChangeInputValueCommand.h"
#include "NodeItem.h"

PinInputItem::PinInputItem(QGraphicsItem* _parent)
    : PinItem(_parent)
{
    m_label = new QGraphicsTextItem(this);
    assert(nullptr != m_label);
    m_label->setFont(QFont("consolas", 10));
    m_label->setDefaultTextColor(QColor(200, 200, 200));
    m_label->setPlainText("No input");
    m_label->setX(radius() + 10);
    m_label->setY(-m_label->boundingRect().height() * 0.5 - 2); // why 2 ? (I don't remember it)

    m_inputDefaultValue = new LineEditQReal();
    QObject::connect(m_inputDefaultValue, &QLineEdit::editingFinished, this, &PinInputItem::updateInput);
    QObject::connect(this, &PinItem::onLink, [this](){m_inputDefaultValue->setEnabled(false);});
    QObject::connect(this, &PinItem::onUnlink, [this](){m_inputDefaultValue->setEnabled(true);});

    m_proxyLineEdit = new QGraphicsProxyWidget(this);
    m_proxyLineEdit->setWidget(m_inputDefaultValue);
    m_proxyLineEdit->setGeometry(QRectF(radius() + 10, 10, 50, 10));

    m_maxLink = 1;
}

void PinInputItem::setInput(ComponentInput* _input)
{
    m_input = _input;
    if (_input != nullptr)
    {
        m_label->setPlainText(_input->getName());
        m_label->setY(-m_label->boundingRect().height() * 0.5 - 2);
        m_inputDefaultValue->setValue(_input->getDefaultValue());
        m_proxyLineEdit->setY(_input->getName().isEmpty() ? -10 : 10);
        m_proxyLineEdit->setVisible(_input->getEditable());
        setPinVisible(_input->getLinkable());
    }
    else
    {
        m_label->setPlainText("NULL");
        m_label->setY(-m_label->boundingRect().height() * 0.5);
        m_inputDefaultValue->setValue(0.);
        m_proxyLineEdit->setVisible(false);
        setPinVisible(false);
    }
}

void PinInputItem::setDefaultValue(qreal _value)
{
    if(m_input != nullptr)
    {
        m_input->setDefaultValue(_value);
        m_inputDefaultValue->setValue(_value);
    }
}

bool PinInputItem::_canConnect(PinItem* _other)
{
    PinOutputItem* output = qgraphicsitem_cast<PinOutputItem*>(_other);
    if (output != nullptr && m_input != nullptr && m_input->getParent() != nullptr
        && output->component() != nullptr && output->component() != m_input->getParent()
        && m_input->canSetComponent(output->component()))
    {
        return PinItem::_canConnect(_other);
    }
    return false;
}

void PinInputItem::_connect(PinItem* _other)
{
    PinItem::_connect(_other);
    PinOutputItem* output = qgraphicsitem_cast<PinOutputItem*>(_other);
    Q_ASSERT(nullptr != output);
    m_input->setComponent(output->component());
    connect(_other->parentNode(), &NodeItem::outputChanged, this, &PinInputItem::setOutputChanged);
    emit outputChanged();
}

void PinInputItem::_disconnect(PinItem* _other)
{
    PinItem::_disconnect(_other);
    m_input->setComponent(nullptr);
    disconnect(_other->parentNode(), &NodeItem::outputChanged, this, &PinInputItem::setOutputChanged);
    emit outputChanged();
}

void PinInputItem::updateInput()
{
    if(m_input != nullptr
        && !qFuzzyCompare(m_input->getDefaultValue(), m_inputDefaultValue->value()))
    {
        undoStack()->push(new ChangeInputValueCommand(this, m_inputDefaultValue->value()));
        setDirty();
        emit outputChanged();
    }
    m_proxyLineEdit->clearFocus();
}
