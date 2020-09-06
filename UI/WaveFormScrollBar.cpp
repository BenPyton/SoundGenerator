/*
 * Copyright 2020 Benoit Pelletier
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

#include "WaveFormScrollBar.h"
#include <QPainter>
#include <QtWidgets>
#include <QMouseEvent>
#include "Utils.h"

WaveFormScrollBar::WaveFormScrollBar(QWidget* _parent)
    : QAbstractSlider(_parent)
{
    installEventFilter(this);
}

void WaveFormScrollBar::setCursorValue(int _value)
{
    m_cursorValue = _value;
    repaint();
}

void WaveFormScrollBar::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event);

    int radius = 5;
    int minX, maxX;
    getScreenStartAndWidth(minX, maxX);
    maxX += minX - HANDLE_WIDTH;
    minX += HANDLE_WIDTH;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(140, 140, 140));
    pen.setCosmetic(true);
    painter.setPen(pen);

    painter.drawRoundedRect(0, 0, size().width(), size().height(), 6, 6);

    QBrush brush(QColor(0, 0, 0));

    pen.setColor(QColor(200, 200, 200));
    painter.setPen(pen);

    brush.setColor(getColor(m_dragZone == SliderZone::NONE && m_hoverZone == SliderZone::DEFAULT, m_dragZone == SliderZone::DEFAULT));
    painter.setBrush(brush);
    painter.drawRect(minX, 0, maxX - minX, size().height());

    QPainterPath leftHandle;
    leftHandle.moveTo(minX, 0);
    leftHandle.lineTo(minX, size().height());
    leftHandle.arcTo(minX - HANDLE_WIDTH, size().height() - 2*radius, 2*radius, 2*radius, -90, -90);
    leftHandle.arcTo(minX - HANDLE_WIDTH, 0, 2*radius, 2*radius, 180, -90);
    leftHandle.closeSubpath();

    brush.setColor(getColor(m_dragZone == SliderZone::NONE && m_hoverZone == SliderZone::MINIMUM, m_dragZone == SliderZone::MINIMUM));
    painter.setBrush(brush);
    painter.drawPath(leftHandle);

    QPainterPath rightHandle;
    rightHandle.moveTo(maxX, 0);
    rightHandle.lineTo(maxX, size().height());
    rightHandle.arcTo(maxX + HANDLE_WIDTH - 2*radius, size().height() - 2*radius, 2*radius, 2*radius, -90, 90);
    rightHandle.arcTo(maxX + HANDLE_WIDTH - 2*radius, 0, 2*radius, 2*radius, 0, 90);
    rightHandle.closeSubpath();

    brush.setColor(getColor(m_dragZone == SliderZone::NONE && m_hoverZone == SliderZone::MAXIMUM, m_dragZone == SliderZone::MAXIMUM));
    painter.setBrush(brush);
    painter.drawPath(rightHandle);

    int cursorX = mapToScreen(m_cursorValue);
    pen.setColor(QColor("yellow"));
    painter.setPen(pen);
    painter.drawLine(cursorX, 0, cursorX, size().height());
}

// Called only when mouse button is pressed inside and dragged
void WaveFormScrollBar::mouseMoveEvent(QMouseEvent *_event)
{
    int newValue = 0;
    int newPageStep = 0;
    int mouseValue = m_valueStart + mapToValue(_event->pos().x() - m_mouseStart);

    switch(m_dragZone)
    {
    case SliderZone::DEFAULT:
        newValue = mouseValue;
        setValue(newValue);
        break;
    case SliderZone::MINIMUM:
        newValue = Utils::Clamp(mouseValue, minimum(), value() + pageStep() - 1);
        newPageStep = value() + pageStep() - newValue;
        setMaximum(maximum() + pageStep() - newPageStep);
        setPageStep(newPageStep);
        setValue(newValue);
        break;
    case SliderZone::MAXIMUM:
        newPageStep = Utils::Clamp(mouseValue, value() + 1, maximum() + pageStep()) - value();
        setMaximum(maximum() + pageStep() - newPageStep);
        setPageStep(newPageStep);
        emit pageStepChanged(newPageStep);
        break;
    default:
        break;
    }
}

void WaveFormScrollBar::mousePressEvent(QMouseEvent *_event)
{
    int mousePos = _event->pos().x();

    SliderZone zone = getSliderZone(mousePos);
    if(zone == SliderZone::PAGEDOWN)
    {
        setValue(value() - pageStep());
        updateSliderState(mousePos);
    }
    else if(zone == SliderZone::PAGEUP)
    {
        setValue(value() + pageStep());
        updateSliderState(mousePos);
    }
    else if(zone != m_dragZone)
    {
        m_dragZone = zone;
        m_mouseStart = mousePos;
        m_valueStart = value() + (zone == SliderZone::MAXIMUM ? pageStep() : 0);
        updateSliderState(mousePos);
        repaint();
    }
}

void WaveFormScrollBar::mouseReleaseEvent(QMouseEvent *_event)
{
    m_dragZone = SliderZone::NONE;
    updateSliderState(_event->pos().x());
    repaint();
}

void WaveFormScrollBar::mouseEnterEvent(QHoverEvent *_event)
{
    updateSliderState(_event->pos().x());
}

void WaveFormScrollBar::mouseLeaveEvent(QHoverEvent *_event)
{
    updateSliderState(_event->pos().x());
}

void WaveFormScrollBar::mouseHoverMoveEvent(QHoverEvent *_event)
{
    updateSliderState(_event->pos().x());
}

bool WaveFormScrollBar::eventFilter(QObject* _object, QEvent* _event)
{
    Q_UNUSED(_object)

    switch(_event->type())
    {
    case QEvent::HoverEnter:
        mouseEnterEvent(static_cast<QHoverEvent*>(_event));
        break;
    case QEvent::HoverLeave:
        mouseLeaveEvent(static_cast<QHoverEvent*>(_event));
        break;
    case QEvent::HoverMove:
        mouseHoverMoveEvent(static_cast<QHoverEvent*>(_event));
        break;
    default:
        break;
    }
    return false;
}

int WaveFormScrollBar::mapToScreen(int _value)
{
    return Utils::MapValue(_value, minimum(), maximum() + pageStep(), 0, size().width());
}

int WaveFormScrollBar::mapToValue(int _screen)
{
    return Utils::MapValue(_screen, 0, size().width(), minimum(), maximum() + pageStep());
}

void WaveFormScrollBar::updateSliderState(int _mousePos)
{
    SliderZone hoverMode = getSliderZone(_mousePos);
    if(hoverMode != m_hoverZone)
    {
        m_hoverZone = hoverMode;
        repaint();
    }

    if(m_dragZone == SliderZone::NONE && m_hoverZone == SliderZone::DEFAULT)
    {
        setCursor(Qt::OpenHandCursor);
    }
    else if (m_dragZone == SliderZone::DEFAULT)
    {
        setCursor(Qt::ClosedHandCursor);
    }
    else if(m_dragZone == SliderZone::MINIMUM || m_dragZone == SliderZone::MAXIMUM
       || m_hoverZone == SliderZone::MINIMUM || m_hoverZone == SliderZone::MAXIMUM)
    {
        setCursor(Qt::SizeHorCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

QColor WaveFormScrollBar::getColor(bool _hovered, bool _pressed)
{
    return _pressed ? PRESSED_COLOR : (_hovered ? HOVERED_COLOR : NORMAL_COLOR);
}

WaveFormScrollBar::SliderZone WaveFormScrollBar::getSliderZone(int _x)
{
    int min, max;
    getScreenStartAndWidth(min, max);
    max += min;

    if(_x >= min && _x <= min + HANDLE_WIDTH)
        return SliderZone::MINIMUM;
    if(_x >= max - HANDLE_WIDTH && _x <= max)
        return SliderZone::MAXIMUM;
    if(_x > min && _x < max)
        return SliderZone::DEFAULT;
    if(_x >= minimum() && _x < min)
        return SliderZone::PAGEDOWN;
    if(_x > max && _x <= maximum())
        return SliderZone::PAGEUP;
    return SliderZone::NONE;
}

void WaveFormScrollBar::getScreenStartAndWidth(int& _start, int& _width)
{
    int targetWidth = mapToScreen(pageStep());
    int minWidth = 2 * HANDLE_WIDTH + SLIDER_MIN_WIDTH;
    if(targetWidth < minWidth)
    {
        qreal percentage = (value() - minimum()) / static_cast<qreal>(maximum() - minimum());
        int offset = qRound((minWidth - targetWidth) * percentage);
        _start = mapToScreen(value()) - offset;
        _width = minWidth;

        qDebug() << QString("start: %1 | percentage: %2 | %3/%4 | %5/%6")
                    .arg(_start)
                    .arg(percentage)
                    .arg(value())
                    .arg(maximum())
                    .arg(offset)
                    .arg(minWidth);
    }
    else
    {
        _start = mapToScreen(value());
        _width = targetWidth;
    }

}
