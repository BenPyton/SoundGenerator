#include "MoveComponentCommand.h"
#include "nodeitem.h"
#include "NodalScene.h"

MoveComponentCommand::MoveComponentCommand(NodeItem* _item, QPointF _oldPosition, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_item(_item), m_oldPosition(_oldPosition)
{
    Q_ASSERT(m_item != nullptr);
    m_newPosition = m_item->pos();
}

void MoveComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setPos(m_oldPosition);
}

void MoveComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setPos(m_newPosition);
}
