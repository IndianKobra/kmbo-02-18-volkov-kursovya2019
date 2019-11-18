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

#ifndef QTPRIVATISATION_H
#define QTPRIVATISATION_H
class QTPrivatisationGame;
class MyButton;
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

class QTPrivatisationGame: public PrivatisationGame , public QObject
{
    protected:
    MyButton* rBtn;
    QPushButton* pushB;
    public:
    static vector<enum Qt::GlobalColor> Colors;
    void Reroll()
    {
        GetNew()->update();
        if(rerolls <=0)
        {
            SkipTurn();
            return;
        }
        PrivatisationGame::Reroll();
        RerollBtnUpdate();
    }
    bool SkipTurn()
    {
        if(ActivePlayer == NULL) return false;
        PrivatisationPlayer* Skiping = ActivePlayer;
        if(! PrivatisationGame::SkipTurn())EndGame();
        ((QTPrivatisationPlayer*)Skiping)->update();
        RerollBtnUpdate();
    }
        QTPrivatisationGame(int n, int m, vector<int> PlayersT, GraphWidget *graphWidget);
        QGraphicsItem* GetNew() {return (QGraphicsItem*)(QTPrivatisationNew*)New;}
        QGraphicsItem* GetMap() {return (QGraphicsItem*)(QTPrivatisationMap*)Map;}
        QGraphicsItem* GetActivePlayer(){return(QGraphicsItem*)(QTPrivatisationPlayer*)ActivePlayer;}
        QGraphicsItem* GetPlayer(size_t i){return(QGraphicsItem*)(QTPrivatisationPlayer*)Players[i];}
        void RerollBtnUpdate();
        int GetActivePlayerID()
        {
            if(ActivePlayer == NULL) return 0;
            return ((QTPrivatisationPlayer*)ActivePlayer)->number;
        }
        void EndGame();

    private:
};

class MyButton: public QPushButton
{
public:
    QTPrivatisationGame* Game;
    MyButton(QWidget *parent, QTPrivatisationGame* game):QPushButton("Reroll", parent){Game = game;}
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        Game->Reroll();
        QPushButton::mouseReleaseEvent(event);
    }
};
#endif // QTPRIVATISATION_H
