#ifndef CREATECOMPONENTCOMMAND_H
#define CREATECOMPONENTCOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class NodalScene;
class NodeItem;

class CreateComponentCommand : public QUndoCommand
{
public:
    CreateComponentCommand(NodalScene* _scene, QString _componentName, QPointF _initialPos, qreal _width, QUndoCommand* _parent = nullptr);
    ~CreateComponentCommand() override;

    void undo() override;
    void redo() override;

    inline NodeItem* getItem() const { return m_item; }

private:
    NodalScene* m_scene;
    NodeItem* m_item;
    QPointF m_initialPos;
    bool m_isOwner;
};

#endif // CREATECOMPONENTCOMMAND_H
