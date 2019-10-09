/*
 * Copyright 2019 Benoit Pelletier
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

#include "lineeditqreal.h"
#include "pininputitem.h"
#include "pinitem.h"
#include <QGraphicsTextItem>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <qvalidator.h>
#include "pinoutputitem.h"
#include "Components/Component.h"

PinInputItem::PinInputItem(QGraphicsItem *parent)
    : PinItem(parent)
{
    m_label = new QGraphicsTextItem(this);
    assert(nullptr != m_label);
    m_label->setFont(QFont("consolas", 10));
    m_label->setDefaultTextColor(QColor(200, 200, 200));
    m_label->setPlainText("No input");
    m_label->setX(15);
    m_label->setY(-m_label->boundingRect().height()*0.5 - 2);

    m_inputDefaultValue = new LineEditQReal();
    //m_inputDefaultValue->setValidator(&getValidator());
    QObject::connect(m_inputDefaultValue, &QLineEdit::editingFinished, this, &PinInputItem::updateInput);
    QObject::connect(this, &PinItem::onLink, [this](){m_inputDefaultValue->setEnabled(false);});
    QObject::connect(this, &PinItem::onUnlink, [this](){m_inputDefaultValue->setEnabled(true);});

    m_proxyLineEdit = new QGraphicsProxyWidget(this);
    m_proxyLineEdit->setWidget(m_inputDefaultValue);
    m_proxyLineEdit->setGeometry(QRectF(15, 10, 50, 10));

    m_maxLink = 1;
}

void PinInputItem::setInput(ComponentInput *input)
{
    m_input = input;
    if (input != nullptr)
    {
        m_label->setPlainText(input->getName());
        m_label->setY(-m_label->boundingRect().height()*0.5 - 2);
        m_inputDefaultValue->setValue(input->getDefaultValue());
        m_proxyLineEdit->setY(input->getName().isEmpty() ? -10 : 10);
        m_proxyLineEdit->setVisible(input->getEditable());
        setPinVisible(input->getLinkable());
    }
    else
    {
        m_label->setPlainText("NULL");
        m_label->setY(-m_label->boundingRect().height()*0.5);
        m_inputDefaultValue->setValue(0.);
        m_proxyLineEdit->setVisible(true);
        setPinVisible(true);
    }
}

void PinInputItem::setDefaultValue(qreal value)
{
    if(m_input != nullptr)
    {
        m_input->setDefaultValue(value);
        m_inputDefaultValue->setValue(value);
    }
}

bool PinInputItem::_tryConnect(PinItem *other)
{
    PinOutputItem* output = qgraphicsitem_cast<PinOutputItem*>(other);
    if (output != nullptr && m_input != nullptr && m_input->getParent() != nullptr
        && output->component() != nullptr && output->component() != m_input->getParent()
        && m_input->setComponent(output->component()))
    {
        return true;
    }

    return false;
}

void PinInputItem::_disconnect()
{
    m_input->setComponent(nullptr);
}

void PinInputItem::updateInput()
{
    if(m_input != nullptr)
    {
        m_input->setDefaultValue(m_inputDefaultValue->value());
        m_proxyLineEdit->clearFocus();
        setDirty();
    }
}
