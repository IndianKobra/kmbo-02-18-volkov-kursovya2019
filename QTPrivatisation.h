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

#ifndef QTPRIVATISATION_H
#define QTPRIVATISATION_H
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
    QPointF Clicked;
    QTPrivatisationGame* Game;
    QRectF boundingRect() const override{return QRectF(0, 0, 10*6, 10*6);}
    QTPrivatisationNew(GraphWidget *graphWidget, QTPrivatisationGame* game);
    QPainterPath shape();
    public:
    GraphWidget *graph;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    friend QTPrivatisationGame;
};

class QTPrivatisationGame: public PrivatisationGame
{
    //protected:
    public:
    static vector<enum Qt::GlobalColor> Colors;
        QTPrivatisationGame(int n, int m, int NumberOfPlayers, GraphWidget *graphWidget);
        QGraphicsItem* GetNew() {return (QGraphicsItem*)(QTPrivatisationNew*)New;}
        QGraphicsItem* GetMap() {return (QGraphicsItem*)(QTPrivatisationMap*)Map;}
        QGraphicsItem* GetActivePlayer(){return(QGraphicsItem*)(QTPrivatisationPlayer*)ActivePlayer;}
        QGraphicsItem* GetPlayer(size_t i){return(QGraphicsItem*)(QTPrivatisationPlayer*)Players[i];}
        int GetActivePlayerID() {return ((QTPrivatisationPlayer*)ActivePlayer)->number;}
    private:
};

#endif // QTPRIVATISATION_H
