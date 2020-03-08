#ifndef DELETECOMPONENTCOMMAND_H
#define DELETECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class DeleteComponentCommand : public QUndoCommand
{
public:
    DeleteComponentCommand(NodalScene* _scene, NodeItem* _item, QPointF _position, QUndoCommand* _parent = nullptr);
    ~DeleteComponentCommand() override;

    void undo() override;
    void redo() override;

    inline NodeItem* getItem() { return m_item; }

private:
    NodalScene* m_scene;
    NodeItem* m_item;
    QPointF m_position;
    bool m_isOwner;
};

#endif // DELETECOMPONENTCOMMAND_H
