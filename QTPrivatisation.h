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
    QTPrivatisationPlayer(PrivatisationPlayer *PrevPlayer):PrivatisationPlayer(PrevPlayer){}
    QTPrivatisationPlayer():PrivatisationPlayer(){}
    QPainterPath shape() const override{return QPainterPath();}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    QRectF boundingRect() const override{return QRectF(0, 0, 100, 100);}
    friend QTPrivatisationGame;
};

class QTPrivatisationMap : public PrivatisationMap, public QGraphicsItem
{
private:
    QTPrivatisationMap(size_t n, size_t m);
public:
    QRectF boundingRect() const override{return QRectF(0, 0, 10*M, 10*N);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    friend QTPrivatisationGame;
};

class QTPrivatisationPointTable: public QGraphicsItem
{
vector<vector<string>> Table;
public:
    void UpdateTable(vector<vector<string>> table){Table=table;}
    QRectF boundingRect() const override{return QRectF(0, 0, 100, 100);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
};

class QTPrivatisationNew: private PrivatisationNew, public QGraphicsItem
{
    protected:
    QTPrivatisationGame* Game;
    QTPrivatisationNew(QTPrivatisationGame* game);
    public:
    QPainterPath shape();
    QRectF boundingRect() const override{return QRectF(0, 0, 60, 60);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    friend QTPrivatisationGame;
};

#endif // QTPRIVATISATION_H
