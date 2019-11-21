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
    if(Game->IsTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10))), 0))
    Game->AddItem(MyPoint(int(round((P.y())/10)), int(round((P.x())/10))), 0);
    Game->GetMap()->update();
    Game->RerollBtnUpdate();
    //GetMap()->update();
    //ActivePlayer->update();
    for(size_t i = 0; i < 4; i++) Game->GetPlayer(i)->update();
    setPos(0, 110);
   }
   QGraphicsItem::mousePressEvent(event);
   if(Game->GetActivePlayerID()==0) Game->EndGame();
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
    return path;
}
void QTPrivatisationNew::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Game->GetMap()->update();
    update();
    QGraphicsItem::mouseMoveEvent(event);
}
void QTPrivatisationGame::Reroll()
{
    if(ActivePlayer==NULL) return ClearMap();
    else if(rerolls <=0) SkipTurn();
    else PrivatisationGame::Reroll();
    GetNew()->update();
    RerollBtnUpdate();
}
QTPrivatisationGame::QTPrivatisationGame(int n, int m, GraphWidget *graphWidget)
{
    PlayersT.resize(4, 0);// = vector<int>({0, 0, 0, 0});
    srand (time(0));
    ActivePlayer = new QTPrivatisationPlayer(graphWidget);
    Players.push_back(ActivePlayer);
    for(size_t i = 0; i < PlayersT.size()-1; i++) Players.push_back(new QTPrivatisationPlayer(graphWidget, Players[i]));
    Map = new QTPrivatisationMap(graphWidget, size_t(n) , size_t(m));
    New = new QTPrivatisationNew(graphWidget, this);
    rBtn = new QPushButton("Reroll", NULL);
    ngBtn = new QPushButton("Start", NULL);
    rerolls=1;
    NewGameBtn = new QPushButton("New Game", NULL);
    PTbnt.resize(4);
    for(size_t i = 0;i < 4; i++) PTbnt[i] = new QPushButton("None");
    connect(rBtn, SIGNAL (released()), this, SLOT(Reroll()));
    connect(ngBtn, SIGNAL (released()), this, SLOT(StartGame()));
    connect(NewGameBtn, SIGNAL (released()), this, SLOT(NewGame()));
    QSignalMapper *signalMapper = new QSignalMapper;
    for(int i = 0;i < 4; i++)
    {
        connect(PTbnt[i], SIGNAL (released()), signalMapper, SLOT(map()));
        signalMapper->setMapping(PTbnt[i], i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(ChangeType(int)));

    ngBtn->setEnabled(0);
    rBtn->setVisible(0);
    for(size_t i = 0;i < 4; i++) ((QTPrivatisationPlayer*)Players[i])->setVisible(0);
    ((QTPrivatisationNew*)New)->setVisible(0);
}
QPushButton* QTPrivatisationGame::GetPlayersTBtn(size_t i)
{
    return PTbnt[i];
}
void QTPrivatisationGame::RerollBtnUpdate()
{
    if(rerolls == 1) rBtn->setText("Reroll");
    else if(rerolls > 0) rBtn->setText(QString::fromStdString("Reroll("+Int2Str(rerolls)+")"));
    else rBtn->setText("SkipTurn");
}

bool QTPrivatisationGame::SkipTurn()
{
    if(ActivePlayer == NULL) return false;
    PrivatisationPlayer* Skiping = ActivePlayer;
    //PrivatisationGame::SkipTurn();
    if(!PrivatisationGame::SkipTurn() || ActivePlayer==NULL)EndGame();
    ((QTPrivatisationPlayer*)Skiping)->update();
    RerollBtnUpdate();
    GetMap()->update();
    for(size_t i = 0; i < 4; i++) GetPlayer(i)->update();
    return true;
}
void QTPrivatisationGame::EndGame()
{
    ActivePlayer = NULL;
    rBtn->setVisible(false);
    PrivatisationGame::EndGame();
    //ngBtn->setVisible(true);
    /*
    draw point table
    */
}
void QTPrivatisationGame::ClearMap()
{
    //PrivatisationGame::NewGame();
    PrivatisationGame::EndGame();
    GetMap()->update();
    GetNew()->update();
    rBtn->setVisible(0);
    ngBtn->show();//setVisible(true);
    for(size_t i = 0; i < Players.size(); i++)
    {
        ((QTPrivatisationPlayer*)Players[i])->setVisible(PlayersT[i]);
        ((QTPrivatisationPlayer*)Players[i])->update();
    }
    rBtn->update();
}
void QTPrivatisationGame::NewGame()
{
    GetNew()->setVisible(false);
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->setVisible(0);
    rBtn->setVisible(0);
    for(size_t i = 0; i < PTbnt.size(); i++) PTbnt[i]->setVisible(1);
    ngBtn->setVisible(1);
    NewGameBtn->setVisible(0);
    PrivatisationGame::NewGame();
}
void QTPrivatisationGame::StartGame()
{
    PrivatisationGame::NewGame();
    AddStartPoints();
    rBtn->setVisible(true);
    GetNew()->setVisible(true);
    GetMap()->update();
    GetNew()->update();
    ngBtn->setVisible(0);
    NewGameBtn->setVisible(1);
    for(size_t i = 0; i < PTbnt.size(); i++) PTbnt[i]->setVisible(0);
    for(size_t i = 0; i < Players.size(); i++)
        ((QTPrivatisationPlayer*)Players[i])->setVisible(PlayersT[i]);
    for(size_t i = 0; i < Players.size(); i++)
        ((QTPrivatisationPlayer*)Players[i])->update();
    RerollBtnUpdate();
    if(ActivePlayer==NULL)EndGame();
}
void QTPrivatisationGame::ChangeType(int i)
{
    vector<QString> text({"None", "Player", "Bot"});
    PlayersT[i] = (PlayersT[i]+1)%3;
    PTbnt[i]->setText(text[PlayersT[i]]);
    int NumberPlayers = 0;
    for(size_t j = 0; j < 4; j++) NumberPlayers += (PlayersT[j]!=0);
    ngBtn->setEnabled(NumberPlayers!=0);
}
QTPrivatisationGame::~QTPrivatisationGame()
{
    delete rBtn;
    delete ngBtn;
    for(size_t i = 0; i < PTbnt.size(); i++) delete PTbnt[i];
    delete (QGraphicsItem*)Map;
    delete (QGraphicsItem*)New;
    for(size_t i = 0; i < Players.size(); i++) delete (QGraphicsItem*) Players[i];
}
int QTPrivatisationGame::GetActivePlayerID()
{
    if(ActivePlayer == NULL) return 0;
    return ((QTPrivatisationPlayer*)ActivePlayer)->number;
}
QGraphicsItem* QTPrivatisationGame::GetNew()
{
    return(QGraphicsItem*)(QTPrivatisationNew*)New;
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
QPushButton* QTPrivatisationGame::GetRbutton()
{
    return rBtn;
}
QPushButton* QTPrivatisationGame::GetNGbutton()
{
    return ngBtn;
}
QPushButton* QTPrivatisationGame::GetNewGameBtn()
{
    return NewGameBtn;
}
#endif
