#include "MultiCommand.h"
#include "nodeitem.h"
#include "NodalScene.h"

MultiCommand::MultiCommand(QString _name, QUndoCommand* _parent)
    : QUndoCommand (_parent)
{
    setText(_name);
}

void MultiCommand::undo()
{
    QUndoCommand::undo();
}

void MultiCommand::redo()
{
    QUndoCommand::redo();
}

