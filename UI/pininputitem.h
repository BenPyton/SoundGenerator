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

#ifndef PinInputItem_H
#define PinInputItem_H

#include "pinitem.h"

class LineEditQReal;
class QGraphicsTextItem;
class QGraphicsProxyWidget;
class ComponentInput;


class PinInputItem : public PinItem
{
public:
    enum { Type = UserType + PININPUT_TYPE };
    int type() const override { return Type; }

    PinInputItem(QGraphicsItem *parent = nullptr);

    void setInput(ComponentInput* input);
    ComponentInput* input() { return m_input; }
    void setDefaultValue(qreal value);

protected:
    virtual bool _canConnect(PinItem* _other) override;
    virtual void _connect(PinItem* _other) override;
    virtual void _disconnect(PinItem* _other) override;
    void updateInput();

private:
    QGraphicsTextItem* m_label;
    LineEditQReal* m_inputDefaultValue;
    QGraphicsProxyWidget* m_proxyLineEdit;
    ComponentInput* m_input;
};

#endif // PinInputItem_H
