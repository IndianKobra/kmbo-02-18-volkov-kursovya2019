#ifndef QTPRIVATISATION_H
#define QTPRIVATISATION_H
#include "privatisation.h"
#include "graphwidget.h"
#include <QGraphicsItem>
#include <math.h>
#include <QString>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QTime>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QPainter>
#include <QPushButton>
#include <QObject>

class QTPrivatisationGame;
class QTPrivatisationPlayer : private PrivatisationPlayer, public QGraphicsItem
{
public:
    GraphWidget *graph;
    QTPrivatisationPlayer(GraphWidget*widget, PrivatisationPlayer *PrevPlayer):PrivatisationPlayer(PrevPlayer),graph(widget){}
    QTPrivatisationPlayer(GraphWidget * widget):PrivatisationPlayer(),graph(widget){}
    QPainterPath shape() const override{return QPainterPath();}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    QRectF boundingRect() const override{return QRectF(0, 0, 100, 100);}
    friend QTPrivatisationGame;
};

class QTPrivatisationMap : public PrivatisationMap, public QGraphicsItem
{
private:
    QTPrivatisationMap(GraphWidget *graphWidget, size_t n, size_t m);
public:
    GraphWidget *graph;
    QRectF boundingRect() const override{return QRectF(0, 0, 10*M, 10*N);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    friend QTPrivatisationGame;
};

class QTPrivatisationNew: private PrivatisationNew, public QGraphicsItem
{
    protected:
    QTPrivatisationGame* Game;
    QTPrivatisationNew(GraphWidget *graphWidget, QTPrivatisationGame* game);
    public:
    QPainterPath shape();
    QRectF boundingRect() const override{return QRectF(0, 0, 60, 60);}
    GraphWidget *graph;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    friend QTPrivatisationGame;
};

#endif // QTPRIVATISATION_H
