#ifndef MOVECOMPONENTCOMMAND_H
#define MOVECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class MoveComponentCommand : public QUndoCommand
{
public:
    MoveComponentCommand(NodeItem* _item, QPointF _oldPosition, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;
    //bool mergeWith(const QUndoCommand *other) override;

    inline NodeItem* getItem() { return m_item; }

private:
    NodeItem* m_item;
    QPointF m_oldPosition;
    QPointF m_newPosition;
};

#endif // MOVECOMPONENTCOMMAND_H
