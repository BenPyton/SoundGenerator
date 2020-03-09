#include "PushOrDragButton.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <qevent.h>
#include <QtCore>

PushOrDragButton::PushOrDragButton(QWidget *parent)
    : QPushButton(parent)
{ }

PushOrDragButton::PushOrDragButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{ }

PushOrDragButton::PushOrDragButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{ }

void PushOrDragButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();
    qDebug() << "click";
}

void PushOrDragButton::mouseMoveEvent(QMouseEvent *event)
{
    QPushButton::mouseMoveEvent(event);

    if (!(event->buttons() & Qt::LeftButton))
            return;
    if ((event->pos() - m_dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    qDebug() << "drag";

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData();

    mimeData->setData("add/component", text().toUtf8());
    drag->setMimeData(mimeData);

    drag->exec(Qt::MoveAction);

    setDown(false);
}
