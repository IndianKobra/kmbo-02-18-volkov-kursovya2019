#ifndef PRIVATISATION_CPP
#define PRIVATISATION_CPP
#include "privatisation.h"
#include <ctime>
#include <queue>
#include <string>
#include <algorithm>


PrivatisationPlayer::PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer)
{
    NextPlayer = PreviousPlayer->NextPlayer;
    PreviousPlayer->NextPlayer = this;
    id = PreviousPlayer->id+1;
}
void PrivatisationPlayer::removePrivatisationPlayer()
{
    PrivatisationPlayer *PreviousPlayer = this;
    while(PreviousPlayer->NextPlayer != this) PreviousPlayer = PreviousPlayer->NextPlayer;
    PreviousPlayer->NextPlayer = NextPlayer;
    status = PrivatisationPlayer::dead;
}
void PrivatisationNew::generateNewItem()
{
    size_t NewX = rand() % 6 + 1, NewY = rand() % 6 + 1;
    New.resize(NewY*NewX);
    for(size_t i = 0; i < NewX; i++) for(size_t j = 0; j < NewY; j++)
        New[i*NewY+j] = MyPoint(int(i), int(j));
}
PrivatisationMap::PrivatisationMap(size_t n, size_t m)
{
    N = n; M = m;
    Map.resize(size_t(n));
    for(size_t i = 0; i < n; i++) Map[i].resize(size_t(m), 0);
}
PrivatisationGame::PrivatisationGame(int n, int m, int idOfPlayers)
{
    Map = new PrivatisationMap(size_t(n) , size_t(m));
    ActivePlayer = new PrivatisationPlayer();
    Players.push_back(ActivePlayer);
    for(size_t i = 0; int(i) < idOfPlayers-1; i++) Players.push_back(new PrivatisationPlayer(*Players[i]));
    addStartPoints();
}
void PrivatisationGame::changeType(size_t i)
{
    PlayersT[i] = PrivatisationPlayer::Status((int(PlayersT[i])+1)%3);
}
bool PrivatisationGame::isTurnPossible(MyPoint r)
{
    for(auto& P:New->New) if((!isPointIn(P+r))||(*Map)[P+r]) return false;
    bool Conected = false;
    vector<MyPoint> Conecting({MyPoint(-1, 0), MyPoint(1, 0), MyPoint(0, 1), MyPoint(0, -1)});
    for(auto& P:New->New) for(auto& v:Conecting)
        if(isPointIn(P+r+v)&&(*Map)[P+r+v]==int(ActivePlayer->id)) Conected=true;
    return Conected;
}
void PrivatisationGame::addItem(MyPoint r)
{
    if(!isTurnPossible(r)) return;
    for(auto& P:New->New) (*Map)[P + r] = int(ActivePlayer->id);
    ActivePlayer->Score += New->size();
    pass();
}
void PrivatisationGame::skipTurn()
{
    if(--ActivePlayer->life<=0)ActivePlayer->removePrivatisationPlayer();
    pass();
}
void PrivatisationGame::pass()
{
    ActivePlayer=ActivePlayer->NextPlayer;
    rerolls = 1;
    if(ActivePlayer)New->generateNewItem();
    else return;
    if(ActivePlayer->status==PrivatisationPlayer::bot) doBotTurn();
}
void PrivatisationGame::doBotTurn()
{
    if(generateBotTurn().x==-1) reroll();
    if(generateBotTurn().x==-1) reroll();
    else addItem(generateBotTurn());
}
void PrivatisationGame::reroll()
{
    if(rerolls <=0) skipTurn();
    else rerolls--;
    New->generateNewItem();
}


MyPoint PrivatisationGame::generateBotTurn()
{
    vector<MyPoint> Ans;
    vector<MyPoint> Conecting({MyPoint(-1, 0), MyPoint(1, 0), MyPoint(0, 1), MyPoint(0, -1)});
    for(MyPoint P(0,0);P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++)if(isTurnPossible(P))Ans.push_back(P);
    if(Ans.size()==0)return MyPoint(-1, -1);
    MyPoint BestTurn(-1, -1), P;
    int BestTurnBadness = 99999, disputed = 0;
    queue<MyPoint> Q;
    vector<PrivatisationMap*> distance(Players.size());
    for(size_t i = 0; i < Players.size();i++)
    {
        distance[i] = new PrivatisationMap(Map->N, Map->M);
        if(Players[i]->status==0)continue;
        for(P.x = 0;P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++)if((*Map)[P]==int(i+1))
        {
            Q.push(P);
            (*distance[i])[P]=1;
        }
        while(!Q.empty())
        {
            P = Q.front();
            for(auto& v:Conecting) if(isPointIn(P+v)&&!((*Map)[P+v]||(*distance[i])[P+v]))
            {
                (*distance[i])[P+v]=(*distance[i])[P]+1;
                Q.push(P+v);
            }
            Q.pop();
        }
    }
    for(MyPoint P(0,0);P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++) if((*distance[ActivePlayer->id-1])[P])
    {
        for(size_t i = 0; i < Players.size();i++)if(i+1!=ActivePlayer->id&&(*distance[i])[P] && (*distance[ActivePlayer->id-1])[P] - (*distance[i])[P]>-9)
        {
            disputed++;
            break;
        }
    }
    if(disputed<30)
    {
        for(size_t i = 0; i < Players.size();i++) delete distance[i];
        PrivatisationMap Power(Map->N, Map->M);
        for(MyPoint P(0,0);P.x<int(Map->N); P.x++)for(P.y=0;P.y<int(Map->M);P.y++)for(size_t j=0;j<Conecting.size();j++)
                if(!isPointIn(P+Conecting[j])||(*Map)[P+Conecting[j]]) ++Power[P];
        for(size_t i = 0; i < Ans.size(); i++)
        {
            int Badness = 0;
            for(size_t j = 0; j < New->New.size(); j++) if(Power[(New->New)[j]+Ans[i]]>1)Badness--;
            for(size_t j = 0; j < New->New.size(); j++) (*Map)[((New->New)[j])+(Ans[i])]=int(ActivePlayer->id);
            for(size_t j = 0; j < New->New.size(); j++)for(size_t k=0;k<Conecting.size();k++)
                if(isPointIn(New->New[j]+Conecting[k])&&(!(*Map)[New->New[j]+Conecting[k]]&&Power[New->New[j]+Conecting[k]]))Badness++;
            for(size_t j = 0; j < New->New.size(); j++) (*Map)[((New->New)[j])+(Ans[i])]=0;
            if(Badness<BestTurnBadness) BestTurn=Ans[i];
            BestTurnBadness=min(Badness, BestTurnBadness);
        }
        if(rerolls&&(BestTurnBadness)>1) return MyPoint(-1,-1);
    }
    else
    {
        PrivatisationMap newDistance(Map->N, Map->M);
        for(size_t i = 0; i < Ans.size(); i++)
        {
            int Badness = 0;
            newDistance.newGame();
            for(auto& P: New->New)
            {
                Q.push(P+Ans[i]);
                newDistance[P+Ans[i]]=1;
            }
            while(!Q.empty())
            {
                P = Q.front();
                for(auto& v:Conecting) if(isPointIn(P+v)&&!((*Map)[P+v]||newDistance[P+v]))
                {
                    newDistance[P+v]=newDistance[P]+1;
                    Q.push(P+v);
                }
                Q.pop();
            }
            for(P.x = 0;P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++)if(!(*Map)[P])
            {
                bool Cool = true;
                for(size_t j = 0; j < Players.size();j++) if((*distance[j])[P]<newDistance[P]&&(*distance[j])[P]) Cool = false;
                for(size_t j = 0; j < Players.size();j++) if((*distance[j])[P] && Cool)Badness--;

            }
            if(Badness<BestTurnBadness) BestTurn=Ans[i];
            BestTurnBadness=min(Badness, BestTurnBadness);
        }
        for(size_t i = 0; i < Players.size();i++) delete distance[i];
        if(New->New.size()<9 && rerolls)return MyPoint(-1, -1);
    }
    return BestTurn;
}
void PrivatisationGame::setPlayersTypes(vector<PrivatisationPlayer::Status> PlayersT)
{
    for(size_t i = 0; i < Players.size(); i++)if(!PlayersT[i])
    {
        if(ActivePlayer == Players[i]) ActivePlayer = ActivePlayer->NextPlayer;
        Players[i]->removePrivatisationPlayer();
    }
    for(size_t i = 0; i < Players.size(); i++) Players[i]->status = PlayersT[i];
}
void PrivatisationGame::newGame()
{
    New->newGame();
    Map->newGame();
    for(size_t i=0;i<Players.size();i++)(Players[i])->newGame();
    for(size_t i=0;i<Players.size()-1;i++)(Players[i])->NextPlayer = Players[i+1];
    (Players[Players.size()-1])->NextPlayer = Players[0];
    ActivePlayer = Players[0];
    setPlayersTypes(PlayersT);
    rerolls = 1;
}
void PrivatisationGame::addStartPoints()
{
    if(Players[0]->status!=0)(*Map)[MyPoint(0, 0)] = 1;
    if(Players.size()>1 && Players[1]->status!=PrivatisationPlayer::dead)
        (*Map)[MyPoint(0, int(Map->M)-1)] = 2;
    if(Players.size()>2 && Players[2]->status!=PrivatisationPlayer::dead)(*Map)[MyPoint(int(Map->N)-1, int(Map->M)-1)] = 3;
    if(Players.size()>3 && Players[3]->status!=PrivatisationPlayer::dead)(*Map)[MyPoint(int(Map->N)-1, 0)] = 4;
    if(ActivePlayer->status==PrivatisationPlayer::bot) doBotTurn();
}
vector<vector<string>> PrivatisationGame::generateScoreTable()
{
    vector<vector<string>> Ans;
    for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i]!=PrivatisationPlayer::dead)
    {
        vector<string> NewString;
        NewString.push_back(to_string(Players[i]->Score));
        NewString.push_back(to_string(int(i)+1));
        NewString.push_back(to_string(Players[i]->Score*100/int(Map->N*Map->M))+'%');
        Ans.push_back(NewString);
    }
    sort(Ans.begin(), Ans.end(), [](vector<string> A, vector<string> B){return stoi(A[0]) > stoi(B[0]);});
    for(size_t i = 0; i < Ans.size(); i++) Ans[i].push_back(to_string(int(i+1)));
    vector<string> NewString;
    int blank = int(Map->N*Map->M);
    for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i]) blank-=Players[i]->Score;
    NewString.push_back(to_string(blank));
    NewString.push_back(to_string(0));
    NewString.push_back(to_string(blank*100/int(Map->N*Map->M))+'%');
    NewString.push_back("");
    Ans.push_back(NewString);
    return Ans;
}
int PrivatisationGame::getActivePlayerID()
{
    if(ActivePlayer) return ActivePlayer->id;
    return 0;
}
bool PrivatisationGame::isPointIn(MyPoint P)
{
    return (P.x >= 0 && P.x < int(Map->N)) && (P.y >= 0 && P.y < int(Map->M));
}
PrivatisationGame::~PrivatisationGame()
{
    for(size_t i = 0; i < Players.size(); i++) delete Players[i];
    delete New;
    delete Map;
}
#endif // PRIVATISATION_CPP
