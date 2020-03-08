#ifndef MULTICOMMAND_H
#define MULTICOMMAND_H

#include <QUndoCommand>

class MultiCommand : public QUndoCommand
{
public:
    MultiCommand(QString _name, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;

private:

};

#endif // MULTICOMMAND_H
