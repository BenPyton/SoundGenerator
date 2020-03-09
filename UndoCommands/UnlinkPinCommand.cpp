#include "UnlinkPinCommand.h"
#include "LinkItem.h"
#include "PinInputItem.h"
#include "PinOutputItem.h"

UnlinkPinCommand::UnlinkPinCommand(QGraphicsScene* _scene, PinItem* _pinA, PinItem* _pinB, QUndoCommand* _parent)
    : QUndoCommand (_parent)
    , m_scene(_scene)
    , m_pinA(_pinA)
    , m_pinB(_pinB)
    , m_isOwner(false)
{
    Q_ASSERT(nullptr != m_scene);
    Q_ASSERT(nullptr != m_pinA);
    Q_ASSERT(nullptr != m_pinB);

    m_item = LinkItem::getLinkBetween(m_pinA, m_pinB);
    Q_ASSERT(nullptr != m_item);
}

UnlinkPinCommand::~UnlinkPinCommand()
{
    if(m_isOwner)
    {
        delete m_item;
    }
}

void UnlinkPinCommand::undo()
{
    QUndoCommand::undo();
    m_scene->addItem(m_item);
    m_item->setPins(m_pinA, m_pinB);
    m_pinA->_connect(m_pinB);
    m_pinB->_connect(m_pinA);
    m_isOwner = false;
}

void UnlinkPinCommand::redo()
{
    QUndoCommand::redo();
    m_pinA->_disconnect(m_pinB);
    m_pinB->_disconnect(m_pinA);
    m_item->setPins(nullptr, nullptr);
    m_scene->removeItem(m_item);
    m_isOwner = true;
}
