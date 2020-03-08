#ifndef UNLINKPINCOMMAND_H
#define UNLINKPINCOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>

class PinItem;
class LinkItem;

class UnlinkPinCommand : public QUndoCommand
{
public:
    UnlinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent = nullptr);
    ~UnlinkPinCommand();

    void undo() override;
    void redo() override;

private:
    LinkItem* m_item;
    QGraphicsScene* m_scene;
    PinItem* m_pinA;
    PinItem* m_pinB;
    bool m_isOwner;
};

#endif // UNLINKPINCOMMAND_H
