#include "DeleteComponentCommand.h"
#include "NodeItem.h"
#include "NodalScene.h"
#include <QtCore>

DeleteComponentCommand::DeleteComponentCommand(NodalScene* _scene, NodeItem* _item, QPointF _position, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_scene(_scene), m_item(_item), m_position(_position), m_isOwner(false)
{
    Q_ASSERT(m_scene != nullptr);
    Q_ASSERT(m_item != nullptr);
}

DeleteComponentCommand::~DeleteComponentCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void DeleteComponentCommand::undo()
{
    QUndoCommand::undo();
    m_item->setPos(m_position);
    m_scene->addNodeItem(m_item);
    QObject::connect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_isOwner = false;
}

void DeleteComponentCommand::redo()
{
    QUndoCommand::redo();
    QObject::disconnect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_scene->removeNodeItem(m_item);
    m_isOwner = true;
}

