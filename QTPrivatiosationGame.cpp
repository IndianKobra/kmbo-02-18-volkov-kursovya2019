#include "QTPrivatisationGame.h"
#include "QTPrivatisation.h"

#ifndef QTPRIVATISATIONGAME_CPP
#define QTPRIVATISATIONGAME_CPP
void QTPrivatisationGame::reroll()
{
    GetNew()->hide();
    if(rerolls <=0) skipTurn();
    else PrivatisationGame::reroll();
    if(!isGameFinished())GetNew()->show();
    rerollBtnUpdate();
}
QTPrivatisationGame::QTPrivatisationGame(int n, int m)
{
    PlayersT.resize(4, PrivatisationPlayer::dead);
    ActivePlayer = new QTPrivatisationPlayer();
    Players.push_back(ActivePlayer);
    for(size_t i = 0; i < PlayersT.size()-1; i++)
        Players.push_back(new QTPrivatisationPlayer(Players[i]));
    Map = new QTPrivatisationMap(size_t(n) , size_t(m));
    New = new QTPrivatisationNew(this);
    rerollButton = new QPushButton("Reroll", nullptr);
    StartButton = new QPushButton("Start", nullptr);
    NewGameButton = new QPushButton("New Game", nullptr);
    Table = new QTPrivatisationPointTable;
    PlayerTypeButtons.resize(Players.size());
    for(auto& elem:PlayerTypeButtons) elem = new QPushButton("None");
    connect(rerollButton, SIGNAL (released()), this, SLOT(reroll()));
    connect(StartButton, SIGNAL (released()), this, SLOT(startGame()));
    connect(NewGameButton, SIGNAL (released()), this, SLOT(newGame()));
    for(size_t i = 0; i < PlayerTypeButtons.size(); i++)
    {
        connect(PlayerTypeButtons[i], SIGNAL (released()), &signalMapper, SLOT(map()));
        signalMapper.setMapping(PlayerTypeButtons[i], int(i));
    }
    connect(&signalMapper, SIGNAL(mapped(int)), this, SLOT(changeType(int)));
    GetTable()->hide();
    StartButton->setEnabled(0);
    rerollButton->hide();
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->hide();
    GetNew()->hide();
}
void QTPrivatisationGame::rerollBtnUpdate()
{
    if(rerolls == 1) rerollButton->setText("Reroll");
    else if(rerolls)rerollButton->setText(
                QString::fromStdString("Reroll("+to_string(rerolls)+")"));
    else rerollButton->setText("SkipTurn");
}

void QTPrivatisationGame::skipTurn()
{
    PrivatisationGame::skipTurn();
    if(isGameFinished())endGame();
    rerollBtnUpdate();
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->update();
}
void QTPrivatisationGame::endGame()
{
    rerollButton->hide();
    Table->updateTable(generateScoreTable());
    Table->show();
    GetNew()->hide();
}

void QTPrivatisationGame::newGame()
{
    GetNew()->hide();
    for(size_t i = 0; i < Players.size(); i++) GetPlayer(i)->hide();
    rerollButton->hide();
    NewGameButton->hide();
    Table->hide();
    for(auto& button:PlayerTypeButtons) button->show();
    StartButton->show();
    PrivatisationGame::newGame();
}
void QTPrivatisationGame::startGame()
{
    PrivatisationGame::newGame();
    addStartPoints();
    rerollButton->show();
    GetNew()->show();
    StartButton->hide();
    NewGameButton->show();
    for(auto& button:PlayerTypeButtons) button->hide();
    for(size_t i=0;i<Players.size(); i++)GetPlayer(i)->setVisible(PlayersT[i]);
    rerollBtnUpdate();
    if(isGameFinished()) endGame();
}
void QTPrivatisationGame::changeType(int i)
{
    vector<QString> text({"None", "Player", "Bot"});
    PrivatisationGame::changeType(size_t(i));
    PlayerTypeButtons[size_t(i)]->setText(text[size_t(PlayersT[size_t(i)])]);
    StartButton->setEnabled(0);
    for(auto& player:PlayersT) if(player) StartButton->setEnabled(1);
}
QTPrivatisationGame::~QTPrivatisationGame()
{
    delete rerollButton;
    delete StartButton;
    for(auto& button:PlayerTypeButtons) delete button;
    delete GetMap();
    delete GetNew();
    for(size_t i = 0; i < Players.size(); i++) delete GetPlayer(i);
}
QGraphicsItem* QTPrivatisationGame::GetNew()
{
    return (QTPrivatisationNew*)New;
}
QGraphicsItem* QTPrivatisationGame::GetMap()
{
    return (QTPrivatisationMap*)Map;
}
QGraphicsItem* QTPrivatisationGame::GetPlayer(size_t i)
{
    return (QTPrivatisationPlayer*)Players[i];
}
QPushButton* QTPrivatisationGame::GetRbutton()
{
    return rerollButton;
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
