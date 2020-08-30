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
    explicit WaveFormView(QWidget *_parent = nullptr);
    virtual ~WaveFormView() override;

    void setSignal(Signal* _signal);
    void setCursorTime(qreal cursorTime);

    inline int getSampleOffset() { return m_sampleOffset; }
    inline int getNbSampleViewed() { return m_nbSampleViewed; }
    inline int getNbTotalSample() { return m_nbTotalSample; }

public slots:
    virtual void resizeEvent(QResizeEvent* _event) override;
    virtual void paintEvent(QPaintEvent* _event) override;
    virtual void wheelEvent(QWheelEvent* _event) override;

    void setSampleOffset(int _startIndex);
    void updateSignal();

signals:
    void zoomChanged();

protected:
    void updateAllChunks();
    void updateZoom(int _localOffset);

private:
    int m_sampleOffset;
    int m_nbSampleViewed;
    int m_nbTotalSample;
    int m_samplePerChunk;
    int m_cursorSample;

    qreal m_zoom;
    qreal m_zoomMin;
    qreal m_zoomMax;

    QList<WaveFormChunk*> m_chunkList;

    Signal* m_signal;

    static const int chunkSize;
};

#endif // WAVEFORMVIEW_H
