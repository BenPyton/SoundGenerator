#ifndef NODALSCENE_H
#define NODALSCENE_H

#include <QGraphicsScene>

class NodeItem;
class QUndoStack;

class NodalScene : public QGraphicsScene
{
    Q_OBJECT

public:
    NodalScene(QObject* parent = nullptr);
    virtual ~NodalScene() override;

    NodeItem* createComponent(QString _componentName, qreal _width = 200.0);
    void addNodeItem(NodeItem *_item);
    void removeNodeItem(NodeItem* _item);

    void save(QString fileName, const qreal duration);
    void load(QString fileName, qreal& duration);

    int clearItems(int from = 0);
    void reset();

    inline NodeItem* getOutput() { return m_nodeList[0]; }
    inline void setCreationPosition(QPointF position) { m_creationPosition = position; }
    inline void setContextPosition(QPointF position) { m_contextPosition = position; }
    inline void isContextMenu(bool value) { m_isContextMenu = value; }

public slots:
    int deleteSelection();
    void selectAll();
    void deselectAll();
    void copyComponents();
    void pasteComponents();
    void undoCommand();
    void redoCommand();
    void setDirty() { emit dirtyChanged(); }

signals:
    void dirtyChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    NodeItem *createNode(QString componentName, qreal width = 200.0);

private:
    QPointF m_creationPosition;
    QPointF m_nextPastePosition;
    QPointF m_contextPosition;
    QVector<NodeItem*> m_nodeList;
    bool m_isContextMenu;
    QUndoStack* m_undoStack;

    QList<QGraphicsItem*> m_movingItems;
    QList<QPointF> m_movingItemPositions;
    QList<QGraphicsItem*> m_previousSelectedItems;

};

#endif // NODALSCENE_H
