#include "LinkPinCommand.h"
#include "NodalScene.h"
#include "LinkItem.h"
#include "PinInputItem.h"
#include "PinOutputItem.h"

LinkPinCommand::LinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent)
    : QUndoCommand (_parent)
    , m_scene(_scene)
    , m_pinA(_pinA)
    , m_pinB(_pinB)
    , m_isOwner(true)
{
    Q_ASSERT(nullptr != m_scene);
    Q_ASSERT(nullptr != m_pinA);
    Q_ASSERT(nullptr != m_pinB);
    m_item = new LinkItem();
}

LinkPinCommand::~LinkPinCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void LinkPinCommand::undo()
{
    QUndoCommand::undo();
    m_pinA->_disconnect(m_pinB);
    m_pinB->_disconnect(m_pinA);
    m_item->setPins(nullptr, nullptr);
    m_scene->removeItem(m_item);
    m_isOwner = true;
}

void LinkPinCommand::redo()
{
    QUndoCommand::redo();
    m_scene->addItem(m_item);
    m_item->setPins(m_pinA, m_pinB);
    m_pinA->_connect(m_pinB);
    m_pinB->_connect(m_pinA);
    m_isOwner = false;
}

