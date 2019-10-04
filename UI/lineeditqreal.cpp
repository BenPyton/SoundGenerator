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
#include <QDoubleValidator>
#include <QTimer>

LineEditQReal::LineEditQReal(QWidget* parent)
    : QLineEdit(parent)
{
    setValidator(&getValidator());
}

void LineEditQReal::setValue(qreal value)
{
    setText(QString::number(value));
}

qreal LineEditQReal::value()
{
    return text().toDouble();
}

void LineEditQReal::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    //selectAll();
    QTimer::singleShot(0, this, &QLineEdit::selectAll);
}


QDoubleValidator &LineEditQReal::getValidator()
{
    static QDoubleValidator validator;
    validator.setLocale(QLocale::C);
    validator.setNotation(QDoubleValidator::StandardNotation);
    return validator;
}
