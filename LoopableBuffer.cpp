#include "LoopableBuffer.h"

LoopableBuffer::LoopableBuffer(QObject* _parent)
    : QBuffer(_parent), m_loop(false)
{
}

LoopableBuffer::LoopableBuffer(QByteArray *_byteArray, QObject* _parent)
    : QBuffer(_byteArray, _parent), m_loop(false)
{
}

qint64 LoopableBuffer::readData(char *_data, qint64 _len)
{
    if(_len <= 0)
        return qint64(0);

    if(m_loop)
    {
        qint64 dataPos = 0;
        qint64 tmpLen = 0;
        while(dataPos < _len)
        {
            tmpLen = qMin(_len - dataPos, size() - pos());
            dataPos += QBuffer::readData(_data + dataPos, tmpLen);
            if(atEnd())
                reset();
        }
        return dataPos;
    }
    else
    {
        return QBuffer::readData(_data, _len);
    }
}
