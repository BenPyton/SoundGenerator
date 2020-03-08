#ifndef SELECTCOMPONENTCOMMAND
#define SELECTCOMPONENTCOMMAND

#include <QUndoCommand>
#include <QGraphicsScene>

class SelectComponentCommand : public QUndoCommand
{
public:
    SelectComponentCommand(QGraphicsItem* _item, bool _selected, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* m_item;
    bool m_selected;
};

#endif // SELECTCOMPONENTCOMMAND
