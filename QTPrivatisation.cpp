#include "QTPrivatisation.h"
#include "privatisation.h"
#include "QTGame.h"
#include <QPushButton>
#include <QWidget>
#include <QCoreApplication>
#ifndef QTPRIVATISATION_CPP
#define QTPRIVATISATION_CPP
vector<enum Qt::GlobalColor> QTPrivatisationGame::Colors({Qt::gray, Qt::yellow, Qt::green, Qt::cyan, Qt::magenta});
void QTPrivatisationPlayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setBrush(QTPrivatisationGame::Colors[size_t(number)]);
    if(T == 0)
    {
        QColor QC(QTPrivatisationGame::Colors[size_t(number)]);
        QC.setAlpha(75);
        painter->setBrush(QC);
    }
    painter->setPen(QPen(Qt::black, 0));
    painter->drawRect(0, 0, 45, 35);
    string S = Int2Str(Str2Int(Int2Str(Score)));
    painter->drawText(QRectF(0, 0, 100, 100),  QString::fromStdString(S));
    painter->drawText(QRectF(0, 15, 100, 100),  QString::fromStdString(Int2Str(life)+"❤️"));
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
    if(Game->IsTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10)))))
    Game->AddItem(MyPoint(int(round((P.y())/10)), int(round((P.x())/10))));
    Game->GetMap()->update();
    Game->RerollBtnUpdate();
    for(size_t i = 0; i < 4; i++) Game->GetPlayer(i)->update();//!! 4-Players.size()
    setPos(0, 110);
    QGraphicsItem::mousePressEvent(event);
}
void QTPrivatisationPointTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawText(QRectF(-100, 0, 100, 50),  QString::fromStdString("Game over"));
    for(int i = 0; i < int(Table.size()); i++)
    {
        painter->setBrush(QTPrivatisationGame::Colors[size_t(Str2Int(Table[size_t(i)][1]))]);
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
    if(!Game->IsTurnPossible(MyPoint(int(round((P.y())/10)), int(round((P.x())/10)))))painter->setBrush(Qt::red);
    else painter->setBrush(QTPrivatisationGame::Colors[size_t(Game->GetActivePlayerID())]);
    P = QPointF(round(P.x()/10)*10, round(P.y()/10)*10) + Game->GetMap()->pos() - pos();
    for(size_t i = 0; i < size(); i++)
    {
        if((x+(New)[i].x >= 0 && y+(New)[i].y >= 0)&&(x+(New)[i].x < 20 &&y+(New)[i].y < 30))
        painter->drawRect(int(P.x()+double((New)[i].y)*10), int(P.y()+double((New)[i].x)*10), 10,10);
    }
    painter->setPen(QPen(Qt::black, 0));


    painter->setBrush(QTPrivatisationGame::Colors[size_t(Game->GetActivePlayerID())]);
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
void QTPrivatisationGame::Reroll()
{
    if(rerolls <=0) SkipTurn();
    else PrivatisationGame::Reroll();
    GetNew()->update();
    RerollBtnUpdate();
}
QTPrivatisationGame::QTPrivatisationGame(int n, int m)
{
    PlayersT.resize(4, 0);
    srand (time(0));
    ActivePlayer = new QTPrivatisationPlayer();
    Players.push_back(ActivePlayer);
    for(size_t i = 0; i < PlayersT.size()-1; i++) Players.push_back(new QTPrivatisationPlayer(Players[i]));
    Map = new QTPrivatisationMap(size_t(n) , size_t(m));
    New = new QTPrivatisationNew(this);
    RerollButton = new QPushButton("Reroll", nullptr);
    StartButton = new QPushButton("Start", nullptr);
    NewGameButton = new QPushButton("New Game", nullptr);
    Table = new QTPrivatisationPointTable;
    PlayerTypeButtons.resize(4);
    for(size_t i = 0;i < 4; i++) PlayerTypeButtons[i] = new QPushButton("None");
    connect(RerollButton, SIGNAL (released()), this, SLOT(Reroll()));
    connect(StartButton, SIGNAL (released()), this, SLOT(StartGame()));
    connect(NewGameButton, SIGNAL (released()), this, SLOT(NewGame()));
    signalMapper = new QSignalMapper;
    for(size_t i = 0; i < 4; i++)
    {
        connect(PlayerTypeButtons[i], SIGNAL (released()), signalMapper, SLOT(map()));
        signalMapper->setMapping(PlayerTypeButtons[i], int(i));
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(ChangeType(int)));
    GetTable()->hide();
    StartButton->setEnabled(0);
    RerollButton->hide();
    for(size_t i = 0; i < 4; i++) GetPlayer(i)->hide();
    GetNew()->hide();
}
void QTPrivatisationGame::RerollBtnUpdate()
{
    if(rerolls == 1) RerollButton->setText("Reroll");
    else if(rerolls > 0) RerollButton->setText(QString::fromStdString("Reroll("+Int2Str(rerolls)+")"));
    else RerollButton->setText("SkipTurn");
}

void QTPrivatisationGame::SkipTurn()
{
    PrivatisationGame::SkipTurn();
    if(IsGameFinished())EndGame();
    RerollBtnUpdate();
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->update();
}
void QTPrivatisationGame::EndGame()
{
    RerollButton->hide();
    Table->UpdateTable(GenerateScoreTable());
    Table->show();
    GetNew()->hide();
}

void QTPrivatisationGame::NewGame()
{
    GetNew()->hide();
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->hide();
    RerollButton->hide();
    NewGameButton->hide();
    Table->hide();
    for(size_t i = 0; i < PlayerTypeButtons.size(); i++) PlayerTypeButtons[i]->show();
    StartButton->show();
    PrivatisationGame::NewGame();
}
void QTPrivatisationGame::StartGame()
{
    PrivatisationGame::NewGame();
    AddStartPoints();
    RerollButton->show();
    GetNew()->show();
    StartButton->hide();
    NewGameButton->show();
    for(size_t i = 0; i < PlayerTypeButtons.size(); i++) PlayerTypeButtons[i]->hide();
    for(size_t i = 0; i < Players.size(); i++)GetPlayer(i)->setVisible(PlayersT[i]);
    RerollBtnUpdate();
    if(IsGameFinished()) EndGame();
}
void QTPrivatisationGame::ChangeType(int i)
{
    vector<QString> text({"None", "Player", "Bot"});
    PlayersT[size_t(i)] = (PlayersT[size_t(i)]+1)%3;
    PlayerTypeButtons[size_t(i)]->setText(text[size_t(PlayersT[size_t(i)])]);
    int NumberPlayers = 0;
    for(size_t j = 0; j < PlayersT.size(); j++) NumberPlayers += (PlayersT[j]!=0);
    StartButton->setEnabled(NumberPlayers!=0);
}
QTPrivatisationGame::~QTPrivatisationGame()
{
    delete RerollButton;
    delete StartButton;
    for(size_t i=0; i<PlayerTypeButtons.size(); i++) delete PlayerTypeButtons[i];
    delete GetMap();
    delete GetNew();
    for(size_t i = 0; i < Players.size(); i++) delete GetPlayer(i);
    delete signalMapper;
}
QGraphicsItem* QTPrivatisationGame::GetNew()
{
    return(QGraphicsItem*)(QTPrivatisationNew*)New;
}
QGraphicsItem* QTPrivatisationGame::GetMap()
{
    return (QGraphicsItem*)(QTPrivatisationMap*)Map;
}
QGraphicsItem* QTPrivatisationGame::GetPlayer(size_t i)
{
    return(QGraphicsItem*)(QTPrivatisationPlayer*)Players[i];
}
QPushButton* QTPrivatisationGame::GetRbutton()
{
    return RerollButton;
}
QPushButton* QTPrivatisationGame::GetStartbutton()
{
    return StartButton;
}
QPushButton* QTPrivatisationGame::GetNewGameButton()
{
    return NewGameButton;
}
QPushButton* QTPrivatisationGame::GetPlayersTBtn(size_t i)
{
    return PlayerTypeButtons[i];
}
QGraphicsItem* QTPrivatisationGame::GetTable()
{
    return Table;
}
#endif
