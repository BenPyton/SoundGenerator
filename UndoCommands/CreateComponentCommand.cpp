#include "CreateComponentCommand.h"
#include "nodeitem.h"
#include "NodalScene.h"
#include "Components.h"

CreateComponentCommand::CreateComponentCommand(NodalScene* _scene, QString _componentName, QPointF _initialPos, qreal _width, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_scene(_scene), m_initialPos(_initialPos), m_isOwner(false)
{
    Q_ASSERT(m_scene != nullptr);
    Component* component = ComponentFactory::CreateComponent(_componentName);
    m_item = new NodeItem();
    m_item->setComponent(component);
    m_item->setWidth(_width);
}

CreateComponentCommand::~CreateComponentCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void CreateComponentCommand::undo()
{
    QUndoCommand::undo();
    QObject::disconnect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_scene->removeNodeItem(m_item);
    m_isOwner = true;
}

void CreateComponentCommand::redo()
{
    QUndoCommand::redo();
    m_item->setPos(m_initialPos);
    m_scene->addNodeItem(m_item);
    QObject::connect(m_item, &NodeItem::dirtyChanged, m_scene, &NodalScene::setDirty);
    m_isOwner = false;
}

