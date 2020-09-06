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

#ifndef WAVEFORMSCROLLBAR_H
#define WAVEFORMSCROLLBAR_H

#include <QAbstractSlider>

class WaveFormScrollBar : public QAbstractSlider
{
    Q_OBJECT

public:
    WaveFormScrollBar(QWidget* _parent = nullptr);

    void setCursorValue(int _value);
    inline int getCursorValue() { return m_cursorValue; }

signals:
    void pageStepChanged(int _pageStep);

protected:
    virtual void paintEvent(QPaintEvent* _event) override;
    virtual void mouseMoveEvent(QMouseEvent* _event) override;
    virtual void mousePressEvent(QMouseEvent* _event) override;
    virtual void mouseReleaseEvent(QMouseEvent* _event) override;
    virtual void mouseHoverMoveEvent(QHoverEvent* _event);
    virtual void mouseEnterEvent(QHoverEvent* _event);
    virtual void mouseLeaveEvent(QHoverEvent* _event);
    virtual bool eventFilter(QObject* _object, QEvent* _event) override;

private:
    enum class SliderZone { NONE = -1, DEFAULT, MINIMUM, MAXIMUM, PAGEUP, PAGEDOWN };

    int mapToScreen(int _value);
    int mapToValue(int _screen);

    void updateSliderState(int _mousePos);

    QColor getColor(bool _hovered, bool _pressed);
    SliderZone getSliderZone(int _x);

    void getScreenStartAndWidth(int& _start, int& _width);

private:
    SliderZone m_dragZone = SliderZone::NONE;
    SliderZone m_hoverZone = SliderZone::NONE;
    int m_mouseStart = 0;
    int m_valueStart = 0;
    int m_cursorValue = 0;

    const QColor NORMAL_COLOR = QColor(60, 60, 60);
    const QColor HOVERED_COLOR = QColor(80, 80, 80);
    const QColor PRESSED_COLOR = QColor(200, 200, 200);
    const QColor HANDLE_BORDER_COLOR = QColor(200, 200, 200);
    const QColor BORDER_COLOR = QColor(140, 140, 140);

    const int HANDLE_WIDTH = 10;
    const int SLIDER_MIN_WIDTH = 10;
};

#endif // WAVEFORMSCROLLBAR_H
