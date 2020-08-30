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

#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>
#include <QBuffer>
#include <QAudio>
#include <QAudioOutput>

class Component;
class LoopableBuffer;

class Signal : public QObject
{
    Q_OBJECT

public:
    Signal(QObject* parent = nullptr);
    virtual ~Signal() override;

    inline void setComponent(Component* _component) { m_component = _component; }
    inline Component* component() { return m_component; }

    void setVolume(qreal _volume);
    qreal volume();

    inline void setDuration(qreal _duration) { m_duration = _duration; }
    inline qreal duration() { return m_duration; }
    int sampleCount();
    qint32 getSample(int _index);
    inline int getSampleRate() { return m_sampleRate; }

signals:
    void signalChanged();
    void stopped();

public slots:
    void play();
    void stop();
    void pause();
    void toStart();
    void toEnd();
    void generate();
    void exportWAV(QString _fileName);
    void loop(bool _enable);

private slots:
    void handleStateChanged(QAudio::State _newState);

private:
    int m_sampleRate;
    qreal m_duration;
    QAudioOutput* m_audio;
    QByteArray* m_samples;
    LoopableBuffer* m_buffer;
    Component* m_component;
};


#endif // SIGNAL_H
