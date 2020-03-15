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

#ifndef WAVEFORMVIEW_H
#define WAVEFORMVIEW_H

#include <QWidget>
#include <QtWidgets>
#include <QList>
#include "Signal.h"

class WaveFormChunk;

class WaveFormView : public QWidget
{
    Q_OBJECT
public:
    explicit WaveFormView(QWidget *parent = nullptr);
    virtual ~WaveFormView() override;

    void setScrollBar(QScrollBar* scrollBar);
    void setSignal(Signal* signal);

public slots:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    void setSampleOffset(int startIndex);
    void updateSignal();

protected:
    void updateAllChunks();
    void updateScrollBar(int _localOffset = 0);

private:
    int m_sampleOffset;
    int m_nbSampleViewed;
    int m_nbTotalSample;
    int m_samplePerChunk;

    qreal m_zoom;
    qreal m_zoomMin;
    qreal m_zoomMax;

    QList<WaveFormChunk*> m_chunkList;

    QScrollBar* m_scrollBar;

    Signal* m_signal;

    static const int chunkSize;
};

#endif // WAVEFORMVIEW_H
