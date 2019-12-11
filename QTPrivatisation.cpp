#ifndef QTPRIVATISATION_CPP
#define QTPRIVATISATION_CPP
#include "QTPrivatisation.h"
#include "privatisation.h"
#include <QPushButton>
#include <QWidget>
#include <QCoreApplication>


vector<enum Qt::GlobalColor> QTPrivatisationGame::Colors(
    {Qt::gray, Qt::yellow, Qt::green, Qt::cyan, Qt::magenta});
void QTPrivatisationPlayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QColor QC(QTPrivatisationGame::Colors[id]);
    if(T == 0) QC.setAlpha(75);
    painter->setBrush(QC);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(0, 0, 45, 35);
    painter->drawText(QRectF(0, 0, 100, 100), QString::fromStdString(to_string(Score)));
    painter->drawText(QRectF(0, 15, 100, 100), QString::fromStdString(to_string(life)+"❤️"));
}
QTPrivatisationMap::QTPrivatisationMap(size_t n, size_t m):PrivatisationMap(n, m){}
void QTPrivatisationMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 0));
    for(size_t i = 0; i < N; i++) for(size_t j = 0; j < M; j++)
    {
        painter->setBrush(QTPrivatisationGame::Colors[size_t(Map[i][j])]);
        painter->drawRect(int(j)*10, int(i)*10, 10, 10);
    }
}
void QTPrivatisationNew::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF P = pos() - Game->GetMap()->pos();
    if(Game->isTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10)))))
    Game->addItem(MyPoint(int(round((P.y())/10)), int(round((P.x())/10))));
    Game->GetMap()->update();
    Game->rerollBtnUpdate();
    for(size_t i = 0; i < 4; i++) Game->GetPlayer(i)->update();//!! 4-Players.size()
    setPos(0, 110);
    QGraphicsItem::mousePressEvent(event);
}
void QTPrivatisationPointTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawText(QRectF(-100, 0, 100, 50),  QString::fromStdString("Game over"));
    for(int i = 0; i < int(Table.size()); i++)
    {
        painter->setBrush(QTPrivatisationGame::Colors[size_t(stoi(Table[size_t(i)][1]))]);
        painter->drawRect(0,  18*i, 15, 18);
        painter->drawRect(15, 18*i, 40, 18);
        painter->drawRect(55, 18*i, 40, 18);
        painter->drawText(QRectF(0, 18*i-1, 100, 50), QString::fromStdString(Table[size_t(i)][3]));
        painter->drawText(QRectF(25, 18*i-1, 100, 50), QString::fromStdString(Table[size_t(i)][0]));
        painter->drawText(QRectF(60, 18*i-1, 100, 50), QString::fromStdString(Table[size_t(i)][2]));
    }
}
void QTPrivatisationNew::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    QPointF P = pos() - Game->GetMap()->pos();
    int x = int(round((P.y())/10)), y = int(round((P.x())/10));
    if(!Game->isTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10)))))
        painter->setBrush(Qt::red);
    else painter->setBrush(QTPrivatisationGame::Colors[size_t(Game->getActivePlayerID())]);
    P = QPointF(round(P.x()/10)*10, round(P.y()/10)*10) + Game->GetMap()->pos() - pos();
    for(size_t i = 0; i < size(); i++)
    {
        if((x+(New)[i].x >= 0 && y+(New)[i].y >= 0)&&(x+(New)[i].x < 20 &&y+(New)[i].y < 30))
        painter->drawRect(int(P.x()+double((New)[i].y)*10), int(P.y()+double((New)[i].x)*10), 10,10);
    }
    painter->setPen(QPen(Qt::black, 0));


    painter->setBrush(QTPrivatisationGame::Colors[size_t(Game->getActivePlayerID())]);
    painter->setPen(QPen(Qt::black, 0));
    for(size_t i = 0; i < size(); i++)painter->drawRect((New)[i].y*10, (New)[i].x*10, 10,10);
}
QTPrivatisationNew::QTPrivatisationNew(QTPrivatisationGame* game):PrivatisationNew()
{
    Game = game;
    setFlag(ItemIsMovable);
}
QPainterPath QTPrivatisationNew::shape()
{
    QPainterPath path;
    for(size_t i = 0; i < size(); i++)path.addRect((New)[i].y*10, (New)[i].x*10, 10,10);
    return path;
}
void QTPrivatisationNew::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Game->GetMap()->update();
    QGraphicsItem::mouseMoveEvent(event);
}
#endif
