#ifndef PUSHORDRAGBUTTON_H
#define PUSHORDRAGBUTTON_H

#include <QPushButton>

class PushOrDragButton : public QPushButton
{
public:
    PushOrDragButton(QWidget *parent = nullptr);
    PushOrDragButton(const QString &text, QWidget *parent = nullptr);
    PushOrDragButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPointF m_dragStartPosition;
};

#endif // PUSHORDRAGBUTTON_H
