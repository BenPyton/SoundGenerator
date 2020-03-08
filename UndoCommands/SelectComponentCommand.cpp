#include "SelectComponentCommand.h"
#include <QGraphicsItem>

SelectComponentCommand::SelectComponentCommand(QGraphicsItem* _item, bool _selected, QUndoCommand* _parent)
    : QUndoCommand (_parent)
    , m_item(_item)
    , m_selected(_selected)
{
    Q_ASSERT(nullptr != m_item);
}

void SelectComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setSelected(!m_selected);
}

void SelectComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setSelected(m_selected);
}
