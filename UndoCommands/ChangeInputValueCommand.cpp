#include "ChangeInputValueCommand.h"
#include "pininputitem.h"
#include "Components/Component.h"

ChangeInputValueCommand::ChangeInputValueCommand(PinInputItem* input, qreal value, QUndoCommand* _parent)
    : QUndoCommand (_parent), m_input(input)
{
    Q_ASSERT(m_input != nullptr);
    m_oldValue = m_input->input()->getDefaultValue();
    m_newValue = value;
}

void ChangeInputValueCommand::undo()
{
    QUndoCommand::undo();
    m_input->setDefaultValue(m_oldValue);
}

void ChangeInputValueCommand::redo()
{
    QUndoCommand::redo();
    m_input->setDefaultValue(m_newValue);
}

