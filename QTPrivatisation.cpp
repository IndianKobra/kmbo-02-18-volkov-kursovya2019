#include "QTPrivatisation.h"
#include "QTGame.h"
#include <QPushButton>
#include <QWidget>
#include <QCoreApplication>
#ifndef QTPRIVATISATION_CPP
#define QTPRIVATISATION_CPP
vector<enum Qt::GlobalColor> QTPrivatisationGame::Colors({Qt::gray, Qt::yellow, Qt::green, Qt::cyan, Qt::magenta});
void QTPrivatisationPlayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setBrush(QTPrivatisationGame::Colors[number]);
    if(T == 0)
    {
        QColor QC(QTPrivatisationGame::Colors[number]);
        QC.setAlpha(75);
        painter->setBrush(QC);
    }
    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(0, 0, 45, 35);
    string S = Int2Str(Score);
    painter->drawText(QRectF(0, 0, 100, 100),  QString::fromStdString(S));
    painter->drawText(QRectF(0, 15, 100, 100),  QString::fromStdString(Int2Str(life)+"❤️"));
}
QTPrivatisationMap::QTPrivatisationMap(GraphWidget *graphWidget, size_t n, size_t m):PrivatisationMap(n, m)
{
    graph = graphWidget;
    setZValue(-1);
}
void QTPrivatisationMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setBrush(Qt::gray);
    painter->setPen(QPen(Qt::black, 0));
    for(size_t i = 0; i < N; i++) for(size_t j = 0; j < M; j++)
    {
        painter->setBrush(QTPrivatisationGame::Colors[Map[i][j]]);
        painter->drawRect(j*10, i*10, 10, 10);
    }
}
void QTPrivatisationNew::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
   if(Game->GetActivePlayerID()!=0)
    {
    QGraphicsItem* ActivePlayer = Game->GetActivePlayer();
    QPointF P = /*event->scenePos() - Clicked*/ pos()   - Game->GetMap()->pos();
    Game->AddItem(MyPoint(int(round((P.y())/10)), int(round((P.x())/10))), 0);
    Game->GetMap()->update();
    Game->RerollBtnUpdate();
    ActivePlayer->update();
    setPos(0, 110);
   }
   QGraphicsItem::mousePressEvent(event);
}

void QTPrivatisationNew::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    QPointF P = pos() - Game->GetMap()->pos();
    int x = int(round((P.y())/10)), y = int(round((P.x())/10));
    if(!Game->IsTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10)))))painter->setBrush(Qt::red);
    else painter->setBrush(QTPrivatisationGame::Colors[Game->GetActivePlayerID()]);
    P = QPointF(round(P.x()/10)*10, round(P.y()/10)*10) + Game->GetMap()->pos() - pos();
    for(size_t i = 0; i < size(); i++)
    {
        if((x+(New)[i].x >= 0 && y+(New)[i].y >= 0)&&(x+(New)[i].x < 20 &&y+(New)[i].y < 30))//!
            painter->drawRect(P.x()+(New)[i].y*10, P.y()+(New)[i].x*10, 10,10);
    }


    painter->setBrush(QTPrivatisationGame::Colors[Game->GetActivePlayerID()]);
    painter->setPen(QPen(Qt::black, 0));
    for(size_t i = 0; i < size(); i++)painter->drawRect((New)[i].y*10, (New)[i].x*10, 10,10);
}
QTPrivatisationNew::QTPrivatisationNew(GraphWidget *graphWidget, QTPrivatisationGame* game):PrivatisationNew()
{
    Game = game;
    graph = graphWidget;
    setFlag(ItemIsMovable);
}
QPainterPath QTPrivatisationNew::shape()
{
    QPainterPath path;
    for(size_t i = 0; i < size(); i++)path.addRect((New)[i].y*10, (New)[i].x*10, 10,10);
    path.addRect(-200, -200, 400, 400);
    return path;
}
void QTPrivatisationNew::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    Game->GetMap()->update();
    QGraphicsItem::mouseMoveEvent(event);
}
void QTPrivatisationGame::Reroll()
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
QTPrivatisationGame::QTPrivatisationGame(int n, int m, vector<int> PlayersT, GraphWidget *graphWidget)
{
    srand (time(0));
    ActivePlayer = new QTPrivatisationPlayer(graphWidget);
    Players.push_back(ActivePlayer);
    for(size_t i = 0; i < PlayersT.size()-1; i++) Players.push_back(new QTPrivatisationPlayer(graphWidget, Players[i]));
    Map = new QTPrivatisationMap(graphWidget, size_t(n) , size_t(m));
    New = new QTPrivatisationNew(graphWidget, this);
    rBtn = new QPushButton("Reroll", graphWidget);
    SetPlayersTypes(PlayersT);
    AddStartPoints();
    connect(rBtn, SIGNAL (released()), this, SLOT(Reroll()));
}
void QTPrivatisationGame::EndGame()
{
    ActivePlayer = NULL;
    rBtn->setText("End Game");
    //rBtn->setVisible(false);
    PrivatisationGame::EndGame();
}
void QTPrivatisationGame::RerollBtnUpdate()
{
    if(rerolls > 0) rBtn->setText("Reroll");
    else rBtn->setText("SkipTurn");
    if(ActivePlayer == NULL) rBtn->setText("End Game");
}

bool QTPrivatisationGame::SkipTurn()
{
    if(ActivePlayer == NULL) return false;
    PrivatisationPlayer* Skiping = ActivePlayer;
    if(! PrivatisationGame::SkipTurn())EndGame();
    ((QTPrivatisationPlayer*)Skiping)->update();
    RerollBtnUpdate();
}
int QTPrivatisationGame::GetActivePlayerID()
{
    if(ActivePlayer == NULL) return 0;
    return ((QTPrivatisationPlayer*)ActivePlayer)->number;
}
QGraphicsItem* QTPrivatisationGame::GetNew()
{
    return (QGraphicsItem*)(QTPrivatisationNew*)New;
}
QGraphicsItem* QTPrivatisationGame::GetMap()
{
    return (QGraphicsItem*)(QTPrivatisationMap*)Map;
}
QGraphicsItem* QTPrivatisationGame::GetActivePlayer()
{
    return(QGraphicsItem*)(QTPrivatisationPlayer*)ActivePlayer;
}
QGraphicsItem* QTPrivatisationGame::GetPlayer(size_t i)
{
    return(QGraphicsItem*)(QTPrivatisationPlayer*)Players[i];
}

#endif
