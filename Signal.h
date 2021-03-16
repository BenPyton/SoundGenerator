/*
 * Copyright 2019-2021 Benoit Pelletier
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
class AudioSettings;

class Signal : public QObject
{
    Q_OBJECT

public:
    enum class Error {
        NO_OUTPUT_DEVICE,
        AUDIO_FORMAT_NOT_SUPPORTED,
        UNKNOWN_ERROR
    };

    Signal(QObject* parent = nullptr);
    virtual ~Signal() override;

    inline void setComponent(Component* _component) { m_component = _component; }
    inline Component* component() { return m_component; }

    void setVolume(qreal _volume);
    qreal volume();

    int sampleCount();
    qint64 getSample(int _index);
    inline int getCursorSample() { return m_cursorSample; }
    inline void setCursorSample(int _cursorSample) { m_cursorSample = _cursorSample; }
    inline AudioSettings& settings() { return *m_audioSettings; }
    inline const QAudioDeviceInfo& deviceInfo() { return m_deviceInfo; }

    void updateAudioOutput(const QAudioDeviceInfo& _info);

signals:
    void signalChanged();
    void stopped();
    void handleError(Error _code);

public slots:
    void play();
    void stop();
    void pause();
    void toStart();
    void toEnd();
    void generate();
    void exportWAV(QString _fileName);
    void loop(bool _enable);
    void setCursorTime(qreal _cursorTime);

private slots:
    void handleStateChanged(QAudio::State _newState);

private:
    bool checkOutputDevice();

private:
    AudioSettings* m_audioSettings;
    QAudioOutput* m_audio;
    QByteArray* m_samples;
    LoopableBuffer* m_buffer;
    Component* m_component;
    int m_cursorSample;
    QAudioDeviceInfo m_deviceInfo;
};

#endif // SIGNAL_H
