#ifndef PRIVATISATION_CPP
#define PRIVATISATION_CPP
#include "privatisation.h"
#include <ctime>
string Int2Str(int n)
{
    string Ans = "";
    if(n<0) return '-' + Int2Str(-n);
    do{
        Ans = char('0'+(n%10)) + Ans;
        n/=10;
    }while(n>0);
    return Ans;
}
int Str2Int(string S)
{
    int Ans = 0;
    for(size_t i = 0;i<S.size();i++) Ans = S[i]=='-'?Ans*-1:Ans*10+int(S[i]-'0');
    return Ans;
}
PrivatisationPlayer::PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer)
{
    NextPlayer = PreviousPlayer->NextPlayer;
    PreviousPlayer->NextPlayer = this;
    number = PreviousPlayer->number+1;
}
void PrivatisationPlayer::RemovePrivatisationPlayer()
{
    PrivatisationPlayer *PreviousPlayer = this;
    while(PreviousPlayer->NextPlayer != this) PreviousPlayer = PreviousPlayer->NextPlayer;
    PreviousPlayer->NextPlayer = NextPlayer;
    T = 0;
}
void PrivatisationNew::GenerateNewItem()
{
    int NewX = rand() % 6 + 1, NewY = rand() % 6 + 1;
    New.resize(size_t(NewY*NewX));
    for(int i = 0; i < NewX; i++) for(int j = 0; j < NewY; j++) New[size_t(i*NewY+j)] = MyPoint(i, j);
}
PrivatisationMap::PrivatisationMap(size_t n, size_t m)
{
    N = n; M = m;
    Map.resize(size_t(n));
    for(size_t i = 0; i < n; i++) Map[i].resize(size_t(m), 0);
}
PrivatisationGame::PrivatisationGame(int n, int m, int NumberOfPlayers)
{
    Map = new PrivatisationMap(size_t(n) , size_t(m));
    srand (time(0));
    ActivePlayer = new PrivatisationPlayer();
    Players.push_back(ActivePlayer);
    for(size_t i = 0; int(i) < NumberOfPlayers-1; i++) Players.push_back(new PrivatisationPlayer(*Players[i]));
    AddStartPoints();
}
bool PrivatisationGame::IsTurnPossible(MyPoint r, bool FirsTurn)
{
    for(size_t i = 0; i < New->size(); i++)
    {
        if(((*New)[i]+r).x<0||((*New)[i]+r).x >=int(Map->N)) return false;
        if(((*New)[i]+r).y<0||((*New)[i]+r).y >=int(Map->M)) return false;
        if((*Map)[(*New)[i]+r]!=0) return false;
    }
    if(FirsTurn) return true;
    bool Conected = false;
    vector<MyPoint> Conecting({MyPoint(-1, 0), MyPoint(1, 0), MyPoint(0, 1), MyPoint(0, -1)});
    for(size_t i = 0; i < New->size(); i++) for(size_t j = 0; j < Conecting.size(); j++)
    {
        MyPoint P = (*New)[i]+Conecting[j] + r;
        if((P.x >= 0 && P.x < int(Map->N)) && (P.y >= 0 && P.y < int(Map->M))
            &&(*Map)[P] == ActivePlayer->number) Conected = true;
    }
    return Conected;
}
bool PrivatisationGame::AddItem(MyPoint r, bool FirsTurn)
{
    if(r.x==-1&&r.y==-1)
    {
        Reroll();
        return false;
    }
    if(!IsTurnPossible(r, FirsTurn)) return false;
    for(size_t i = 0; i < New->size(); i++) (*Map)[(*New)[i] + r] = ActivePlayer->number;
    ActivePlayer->Score += New->size();
    Pass();
    return true;
}
bool PrivatisationGame::SkipTurn()
{
    if(ActivePlayer==nullptr||ActivePlayer->life<=0) return false;
    ActivePlayer->life--;
    if(ActivePlayer->life<=0)ActivePlayer->RemovePrivatisationPlayer();
    if(ActivePlayer==ActivePlayer->NextPlayer && ActivePlayer->life <= 0)
    {
        EndGame();
        return false;
    }
    Pass();
    return true;
}
void PrivatisationGame::Pass()
{
    ActivePlayer=ActivePlayer->NextPlayer;
    rerolls = 1;
    if(ActivePlayer)New->GenerateNewItem();
    else return;
    if(ActivePlayer->T==2) DoBotTurn();
}
void PrivatisationGame::DoBotTurn()
{
    if(!AddItem(GenerateBotTurn()))AddItem(GenerateBotTurn());
    //Pass();
}
void PrivatisationGame::Reroll()
{
    if(rerolls <=0) SkipTurn();
    else rerolls--;
    if(ActivePlayer!=nullptr)New->GenerateNewItem();
}

void PrivatisationGame::EndGame()
{
    New->EndGame();
    ActivePlayer = nullptr;
}
MyPoint PrivatisationGame::GenerateBotTurn()
{
    vector<MyPoint> Ans;
    for(MyPoint P(0,0);P.x<int(Map->N); P.x++)for(P.y=0; P.y<int(Map->M);P.y++)if(IsTurnPossible(P))Ans.push_back(P);
    if(Ans.size()!=0)return(Ans[size_t(rand()%int(Ans.size()))]);
    else return MyPoint(-1,-1);
}
void PrivatisationGame::SetPlayersTypes(vector<int> PlayersT)
{
    for(size_t i = 0; i < Players.size(); i++)if(!PlayersT[i])
    {
        if(ActivePlayer == Players[i]) ActivePlayer = ActivePlayer->NextPlayer;
        Players[i]->RemovePrivatisationPlayer();
    }
    for(size_t i = 0; i < Players.size(); i++) Players[i]->T =PlayersT[i];
}
void PrivatisationGame::NewGame()
{
    New->NewGame();
    Map->NewGame();
    for(size_t i=0;i<Players.size();i++)(Players[i])->NewGame();
    for(size_t i=0;i<Players.size()-1;i++)(Players[i])->NextPlayer = Players[i+1];
    (Players[Players.size()-1])->NextPlayer = Players[0];
    ActivePlayer = Players[0];
    SetPlayersTypes(PlayersT);
    rerolls = 1;
}
void PrivatisationGame::AddStartPoints()
{
    if(Players[0]->T!=0)(*Map)[MyPoint(0, 0)] = 1;
    if(Players.size()>1 && Players[1]->T!=0)(*Map)[MyPoint(0, int(Map->M)-1)] = 2;
    if(Players.size()>2 && Players[2]->T!=0)(*Map)[MyPoint(int(Map->N)-1, int(Map->M)-1)] = 3;
    if(Players.size()>3 && Players[3]->T!=0)(*Map)[MyPoint(int(Map->N)-1, 0)] = 4;
    if(ActivePlayer->T==2) DoBotTurn();
}
vector<vector<string>> PrivatisationGame::GenerateScoreTable()
{
    vector<vector<string>> Ans;
    for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i])
    {
        vector<string> NewString;
        NewString.push_back(Int2Str(Players[i]->Score));
        NewString.push_back(Int2Str(int(i)+1));
        NewString.push_back(Int2Str(Players[i]->Score*100/int(Map->N*Map->M))+'%');
        Ans.push_back(NewString);
    }
    sort(Ans.begin(), Ans.end(), [](vector<string> A, vector<string> B){return Str2Int(A[0]) > Str2Int(B[0]);});
    for(size_t i = 0; i < Ans.size(); i++) Ans[i].push_back(Int2Str(int(i+1)));
    vector<string> NewString;
    int blank = int(Map->N*Map->M);
    for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i]) blank-=Players[i]->Score;
    NewString.push_back(Int2Str(blank));
    NewString.push_back(Int2Str(0));
    NewString.push_back(Int2Str(blank*100/int(Map->N*Map->M))+'%');
    NewString.push_back("");
    Ans.push_back(NewString);
    return Ans;
}

PrivatisationGame::~PrivatisationGame()
{
    for(size_t i = 0; i < Players.size(); i++) delete Players[i];
    delete New;
    delete Map;
}
#endif // PRIVATISATION_CPP
