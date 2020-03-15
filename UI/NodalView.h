/*
 * Copyright 2019-2020 Benoit Pelletier
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

class NodalScene;

class NodalView : public QGraphicsView
{
    Q_OBJECT

public:
    NodalView(QWidget* _parent = nullptr);

    void init();
    void setZoomLimits(qreal _min, qreal _max);
    void setZoom(qreal _zoom);
    inline qreal getZoom() { return m_zoom; }
    inline void setMenuAdd(QMenu* _menuAdd) { m_menuAdd = _menuAdd; }

protected:
    virtual void keyPressEvent(QKeyEvent* _event) override;
    virtual void wheelEvent(QWheelEvent* _event) override;
    virtual void mousePressEvent(QMouseEvent* _event) override;
    virtual void mouseReleaseEvent(QMouseEvent *_event) override;
    virtual void mouseMoveEvent(QMouseEvent* _event) override;
    virtual void dragEnterEvent(QDragEnterEvent* _event) override;
    virtual void dragMoveEvent(QDragMoveEvent* _event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* _event) override;
    virtual void dropEvent(QDropEvent* _event) override;
    virtual void drawBackground(QPainter* _painter, const QRectF& rect) override;

    NodalScene* getScene();

public slots:
    void showCustomContextMenu(const QPoint& _pos);

protected slots:
    void updateZoomView();

signals:
    void zoomChanged(qreal _zoom);

private:
    void autocomputeSceneSize();
    void updateTransform();

private:
    bool m_dragging;
    bool m_rightClickPressed;
    QPoint m_lastMousePos;
    QPoint m_startMousePos;
    QPointF m_startScenePos;

    qreal m_zoomMin;
    qreal m_zoomMax;
    qreal m_zoom;
    QPoint m_translation;

    QMenu* m_menuAdd;

    NodalScene* m_scene;

public:
    static const int gridUnit;
};

#endif // NODALVIEW_H
