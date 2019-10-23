/*
 * Copyright 2019 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef NODALVIEW_H
#define NODALVIEW_H

#include <QtWidgets>
#include <QWidget>
#include <QGraphicsView>

class NodeItem;
//class Component;

class NodalView : public QGraphicsView
{
    Q_OBJECT

public:
    NodalView(QWidget* parent = nullptr);

    void init();
    void setZoomLimits(qreal min, qreal max);
    void setZoom(qreal zoom);
    inline qreal getZoom() { return m_zoom; }

    void save(QString fileName, const qreal duration);
    void load(QString fileName, qreal& duration);

    NodeItem* createComponent(QString componentName, qreal width = 200.0);
    void addNodeItem(NodeItem *item);
    int clearItems(int from = 0);
    void reset();

    NodeItem* getOutput() { return m_nodeList[0]; }

    void setMenuAdd(QMenu* menuAdd) { m_menuAdd = menuAdd; }

protected:
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

    virtual void drawBackground(QPainter * painter, const QRectF & rect) override;

public slots:
    void showCustomContextMenu(const QPoint& pos);
    int deleteSelection();
    void selectAll();
    void deselectAll();
    void copyComponents();
    void pasteComponents();
    void setDirty() { emit dirtyChanged(); }

protected slots:
    void updateZoomView();

signals:
    void zoomChanged(qreal zoom);
    void dirtyChanged();

private:
    void autocomputeSceneSize();
    void updateTransform();
    NodeItem *createNode(QString componentName, qreal width = 200.0);

private:
    bool m_dragging;
    bool m_rightClickPressed;
    bool m_contextMenu;
    QPoint m_lastMousePos;
    QPoint m_startMousePos;
    QPointF m_startScenePos;

    qreal m_zoomMin;
    qreal m_zoomMax;

    qreal m_zoom;
    QPoint m_translation;

    QPointF m_nextCreationPosition;
    QPointF m_nextPastePosition;
    QPointF m_ContextPosition;
    QVector<NodeItem*> m_nodeList;

    QMenu* m_menuAdd;


public:
    static const int gridUnit;
};

#endif // NODALVIEW_H
