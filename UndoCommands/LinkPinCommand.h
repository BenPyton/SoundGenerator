#ifndef LINKPINCOMMAND_H
#define LINKPINCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>

class PinItem;
class LinkItem;

class LinkPinCommand : public QUndoCommand
{
public:
    LinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent = nullptr);
    ~LinkPinCommand();

    void undo() override;
    void redo() override;

private:
    LinkItem* m_item;
    QGraphicsScene* m_scene;
    PinItem* m_pinA;
    PinItem* m_pinB;
    bool m_isOwner;
};

#endif // LINKPINCOMMAND_H
