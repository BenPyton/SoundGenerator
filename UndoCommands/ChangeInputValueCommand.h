#ifndef CHANGEINPUTVALUECOMMAND_H
#define CHANGEINPUTVALUECOMMAND_H

#include <QUndoCommand>

class PinInputItem;

class ChangeInputValueCommand : public QUndoCommand
{
public:
    ChangeInputValueCommand(PinInputItem* input, qreal value, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;

private:
    PinInputItem* m_input;
    qreal m_oldValue;
    qreal m_newValue;
};

#endif // CHANGEINPUTVALUECOMMAND_H
