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

#include "WaveFormView.h"
#include "WaveFormChunk.h"
#include <QPainter>
#include <QtMath>
#include "Utils.h"

const int WaveFormView::chunkSize = 128;

WaveFormView::WaveFormView(QWidget *_parent)
    : QWidget(_parent), m_signal(nullptr), m_firstUpdate(true)
{
    // These values will be correctly set by setting a signal
    m_sampleOffset = 0;
    m_cursorSample = 0;
    m_nbSampleViewed = 0;
    m_samplePerChunk = 0;

    // zoom is the number of sample per pixel
    m_zoom = 0.0;
    m_zoomMin = 0.1; // means 1 sample = 10 pixels
    m_zoomMax = 1.0; // this is just a default value that will be changed later
}

WaveFormView::~WaveFormView()
{
    for(WaveFormChunk* chunk : m_chunkList)
    {
        delete chunk;
    }
}

void WaveFormView::setSignal(Signal *_signal)
{
    if(m_signal != nullptr)
    {
        disconnect(m_signal);
    }

    m_signal = _signal;
    if(m_signal != nullptr)
    {
        connect(m_signal, &Signal::signalChanged, this, &WaveFormView::updateSignal);
        m_nbSampleViewed = m_signal->sampleCount();
        setZoomMax(m_signal->sampleCount() / static_cast<qreal>(size().width()));
        setZoom(m_zoomMax);
    }
}

void WaveFormView::setCursorSample(int newValue)
{
    m_cursorSample = newValue;
    update();
}

int WaveFormView::getNbTotalSample()
{
    if(m_signal != nullptr)
    {
        return m_signal->sampleCount();
    }
    return 0;
}

void WaveFormView::setNbSampleViewed(int _nbSampleViewed)
{
    if(m_nbSampleViewed == _nbSampleViewed)
        return;

    setZoom(_nbSampleViewed / static_cast<qreal>(size().width()));
}

void WaveFormView::setZoomMin(qreal _value)
{
    m_zoomMin = _value;
    if(m_zoom < m_zoomMin)
    {
        setZoom(m_zoomMin);
    }
}

void WaveFormView::setZoomMax(qreal _value)
{
    m_zoomMax = _value;
    if(m_zoom > m_zoomMax)
    {
        setZoom(m_zoomMax);
    }
}

void WaveFormView::setZoom(qreal _value)
{
    qreal newZoom = Utils::Clamp(_value, m_zoomMin, m_zoomMax);
    if(!qFuzzyCompare(newZoom, m_zoom))
    {
        m_zoom = newZoom;
        updateZoom();
    }
}

void WaveFormView::setSampleOffset(int _startIndex)
{
    if(m_sampleOffset == _startIndex)
        return;

    m_sampleOffset = qMax(0, _startIndex);
    qDebug() << "Start Offset: " << m_sampleOffset << " | " << m_chunkList.first()->startIndex();
    updateView();
}

void WaveFormView::resizeEvent(QResizeEvent *_event)
{
    Q_UNUSED(_event)

    setNbSampleViewed(qRound(m_zoom * size().width()));
    emit zoomChanged();
}

void WaveFormView::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event);

    QPainter painter(this);

    int pixelOffset = qRound((m_chunkList.first()->startIndex() - m_sampleOffset) * chunkSize / static_cast<qreal>(m_samplePerChunk));

    for(int  i = 0; i < m_chunkList.size(); i++)
    {
        painter.drawPixmap(pixelOffset + i * (chunkSize), 0,
                           m_chunkList[i]->width(), m_chunkList[i]->height(),
                           *m_chunkList[i],
                           0, 0,
                           m_chunkList[i]->width(), m_chunkList[i]->height());
    }

    int x = Utils::MapValue(m_cursorSample - m_sampleOffset, 0, m_nbSampleViewed, 0, size().width());

    QPen pen(QColor("yellow"));
    painter.setPen(pen);
    painter.drawLine(x, 0, x, size().width());

}

void WaveFormView::wheelEvent(QWheelEvent *_event)
{
    int localOffset = m_sampleOffset + qRound(m_zoom * _event->pos().x());

    // zoom
    int angle = _event->angleDelta().y();
    qreal factor = 1.0 + (angle > 0.0 ? 0.1 : -0.1);
    m_zoom = Utils::Clamp(m_zoom * factor, m_zoomMin, m_zoomMax);
    updateZoom(localOffset);
}

void WaveFormView::updateSignal()
{
    if(m_signal != nullptr)
    {
        setZoomMax(m_signal->sampleCount() / static_cast<qreal>(size().width()));
        if(m_firstUpdate)
        {
            setZoom(m_zoomMax);
            m_firstUpdate = false;
        }
        else
        {
            updateAllChunks();
            emit zoomChanged();
        }
    }
}

void WaveFormView::updateAllChunks()
{
    for(int i = 0; i < m_chunkList.size(); i++)
    {
        m_chunkList[i]->setIndexRange(m_chunkList.first()->startIndex() + i * m_samplePerChunk, m_samplePerChunk);
        m_chunkList[i]->updateWave(m_signal);
    }
    update();
}

void WaveFormView::updateZoom(int _localOffset)
{
    int oldSampleViewed = m_nbSampleViewed;
    m_nbSampleViewed = qRound(m_zoom * size().width());
    if(_localOffset != 0)
    {
        m_sampleOffset = Utils::Clamp(
                    qRound(_localOffset + (m_sampleOffset - _localOffset) * qreal(m_nbSampleViewed) / oldSampleViewed)
                    , 0, m_signal->sampleCount() - m_nbSampleViewed);
    }
    qDebug() << QString("Zoom: %1 | Sample Viewed: %2 | Sample Count: %3 | Sample Offset: %4 | Local Offset %5 | Witdh: %6")
                .arg(m_zoom)
                .arg(m_nbSampleViewed)
                .arg(getNbTotalSample())
                .arg(m_sampleOffset)
                .arg(_localOffset)
                .arg(size().width());
    updateView();
    emit zoomChanged();
}

void WaveFormView::updateView()
{
    m_samplePerChunk = qRound(chunkSize * m_zoom);
    int startIndex = m_samplePerChunk * (m_sampleOffset / m_samplePerChunk);

    // Update the number of chunk depending on the widget width
    updateChunkList();
    m_chunkList.first()->setIndexRange(startIndex, m_samplePerChunk);

    // set correct start index and update pixmap of each chunk
    updateAllChunks();
}

void WaveFormView::updateChunkList()
{
    int nbChunk = 3 + (size().width() / chunkSize);

    while(nbChunk > m_chunkList.size())
    {
        int startIndex = m_chunkList.size() > 0 ? m_chunkList.last()->startIndex() + m_samplePerChunk : 0;
        m_chunkList.push_back(new WaveFormChunk(chunkSize, 100));
        m_chunkList.last()->setIndexRange(startIndex, m_samplePerChunk);
        m_chunkList.last()->updateWave(m_signal);
        m_chunkList.last()->setColor(QColor(255, 0, 255));
    }

    while(nbChunk < m_chunkList.size())
    {
        delete m_chunkList.last();
        m_chunkList.pop_back();
    }
}
