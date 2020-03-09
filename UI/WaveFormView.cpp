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

#include "WaveFormView.h"
#include "WaveFormChunk.h"
#include <QPainter>

const int WaveFormView::chunkSize = 128;

WaveFormView::WaveFormView(QWidget *parent)
    : QWidget(parent), m_signal(nullptr)
{
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_scrollBar = nullptr;
    m_sampleOffset = 0;
    m_nbSampleViewed = 2400;
    m_nbTotalSample = 0;

    m_samplePerChunk = chunkSize;

    m_zoom = 1.0;
    m_zoomMin = 0.01;
    m_zoomMax = 6.0;
}

WaveFormView::~WaveFormView()
{
    for(WaveFormChunk* chunk : m_chunkList)
    {
        delete chunk;
    }
}

void WaveFormView::setScrollBar(QScrollBar *scrollBar)
{
    if(nullptr != m_scrollBar)
    {
        disconnect(m_scrollBar, SIGNAL(valueChanged(int)), this, SLOT(setSampleOffset(int)));
    }
    m_scrollBar = scrollBar;
    if(nullptr != m_scrollBar)
    {
        connect(m_scrollBar, SIGNAL(valueChanged(int)), this, SLOT(setSampleOffset(int)));
        m_scrollBar->setMinimum(0);
        m_scrollBar->setMaximum(qMax(m_nbTotalSample - m_nbSampleViewed, m_nbSampleViewed));
        m_scrollBar->setPageStep(m_nbSampleViewed);
    }
}

void WaveFormView::setSignal(Signal *signal)
{
    if(m_signal != nullptr)
    {
        this->disconnect(m_signal);
    }

    m_signal = signal;
    if(m_signal != nullptr)
    {
        connect(m_signal, SIGNAL(signalChanged()), this, SLOT(updateSignal()));
        m_nbTotalSample = m_signal->sampleCount();
    }
}

void WaveFormView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    int nbChunk = 3 + size().width() / chunkSize;

    while(nbChunk > m_chunkList.size())
    {
        int startIndex = m_chunkList.size() > 0 ? m_chunkList.last()->startIndex() + m_samplePerChunk : 0;
        m_chunkList.push_back(new WaveFormChunk(chunkSize, 100));
        m_chunkList.last()->setIndexRange(startIndex, m_samplePerChunk);
        m_chunkList.last()->updateWave(m_signal);
        m_chunkList.last()->setColor(QColor(255, 0, 0));
    }

    while(nbChunk < m_chunkList.size())
    {
        delete m_chunkList.last();
        m_chunkList.pop_back();
    }

    updateScrollBar();
}

void WaveFormView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    int pixelOffset = qRound((m_chunkList.first()->startIndex() - m_sampleOffset) * m_zoom);

    for(int  i = 0; i < m_chunkList.size(); i++)
    {
        painter.drawPixmap(pixelOffset + i * (chunkSize), 0,
                           m_chunkList[i]->width(), m_chunkList[i]->height(),
                           *m_chunkList[i],
                           0, 0,
                           m_chunkList[i]->width(), m_chunkList[i]->height());
    }

}

void WaveFormView::wheelEvent(QWheelEvent *event)
{

    // zoom
    int angle = event->angleDelta().y();
    qreal factor = 1.0 + (angle > 0.0 ? 0.1 : -0.1);
    m_zoom *= factor;
    if(m_zoomMin > 0.0 && m_zoom < m_zoomMin)
    {
        m_zoom = m_zoomMin;
    }
    else if(m_zoomMax > 0.0 && m_zoom > m_zoomMax)
    {
        m_zoom = m_zoomMax;
    }

    int localOffset = qRound(m_scrollBar->value() + m_nbSampleViewed * qreal(event->x()) / width());
    updateScrollBar(localOffset);

    updateAllChunks();
    update();
}

void WaveFormView::setSampleOffset(int startIndex)
{
    m_sampleOffset = startIndex;

    int chunkStartIndex = m_chunkList.first()->startIndex();


    if(m_sampleOffset - chunkStartIndex >= 2*m_samplePerChunk)
    {
        WaveFormChunk* chunk = m_chunkList.first();
        m_chunkList.pop_front();
        chunk->setIndexRange(m_chunkList.last()->startIndex() + m_samplePerChunk, m_samplePerChunk);
        chunk->updateWave(m_signal);
        m_chunkList.push_back(chunk);
    }
    else if(m_sampleOffset - chunkStartIndex < 0)
    {
        WaveFormChunk* chunk = m_chunkList.last();
        m_chunkList.pop_back();
        chunk->setIndexRange(m_chunkList.first()->startIndex() - m_samplePerChunk, m_samplePerChunk);
        chunk->updateWave(m_signal);
        m_chunkList.push_front(chunk);
    }

    qDebug() << "Start Offset: " << startIndex << " | " << m_chunkList.first()->startIndex();

    update();
}

void WaveFormView::updateSignal()
{
    if(m_signal != nullptr)
    {
        m_nbTotalSample = m_signal->sampleCount();
        updateAllChunks();
        update();
    }
}

void WaveFormView::updateAllChunks()
{
    for(int i = 0; i < m_chunkList.size(); i++)
    {
        m_chunkList[i]->setIndexRange(m_chunkList.first()->startIndex() + i * m_samplePerChunk, m_samplePerChunk);
        m_chunkList[i]->updateWave(m_signal);
    }
}

void WaveFormView::updateScrollBar(int _localOffset)
{
    int oldSampleViewed = m_nbSampleViewed;
    m_samplePerChunk = qRound(chunkSize / m_zoom);
    m_nbSampleViewed = qRound(m_samplePerChunk * qreal(width()) / chunkSize);
    qDebug() << "Zoom: " << m_zoom << " | Sample: " << m_nbSampleViewed;

    if(nullptr != m_scrollBar)
    {
        m_scrollBar->setMaximum(qMax(m_nbTotalSample - m_nbSampleViewed, 0));
        m_scrollBar->setPageStep(m_nbSampleViewed);
        m_scrollBar->setValue(qRound(_localOffset + (m_scrollBar->value() - _localOffset) * qreal(m_nbSampleViewed) / oldSampleViewed));
    }

    if(m_nbSampleViewed > m_nbTotalSample)
    {
        m_chunkList.first()->setIndexRange(0, chunkSize);
        m_sampleOffset = 0;
        if(nullptr != m_scrollBar)
        {
            m_scrollBar->setValue(m_scrollBar->minimum());
        }
    }
}
