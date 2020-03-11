#ifndef LOOPABLEBUFFER_H
#define LOOPABLEBUFFER_H

#include <QBuffer>

class LoopableBuffer : public QBuffer
{
public:
    LoopableBuffer(QObject* _parent = nullptr);
    LoopableBuffer(QByteArray *_byteArray, QObject* _parent = nullptr);

    inline void setLoop(bool _loop) { m_loop = _loop; }
    inline bool loop() { return m_loop; }

protected:
    virtual qint64 readData(char* _data, qint64 _len) override;

private:
    bool m_loop;
};

#endif // LOOPABLEBUFFER_H
