#ifndef ACTIONCYCLE_H
#define ACTIONCYCLE_H

#include <QAction>

class ActionCycle : public QAction
{
    Q_OBJECT

public:
    ActionCycle(QObject* _parent = nullptr);

    void addAction(const QString &_text);
    void addAction(const QString &_text, const QIcon &_icon);
    void setIndex(int _index);

public slots:
    inline void reset() { setIndex(0); }

private :
    void _updateAction();

signals:
    void triggered(int _index);

private slots:
    void onTrigger();

private:
    int m_currentIndex;
    QList<QString> m_texts;
    QList<QIcon> m_icons;
};

#endif // ACTIONCYCLE_H
