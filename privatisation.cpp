#ifndef PRIVATISATION_CPP
#define PRIVATISATION_CPP
#include "privatisation.h"
#include <ctime>
#include <queue>
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
bool PrivatisationGame::IsTurnPossible(MyPoint r)
{
    for(size_t i = 0; i < New->size(); i++)
    {
        if((!Map->IsPointIn((*New)[i]+r))||(*Map)[(*New)[i]+r]) return false;
        /*if(((*New)[i]+r).x<0||((*New)[i]+r).x >=int(Map->N)) return false;
        if(((*New)[i]+r).y<0||((*New)[i]+r).y >=int(Map->M)) return false;
        if((*Map)[(*New)[i]+r]) return false;*/
    }
    bool Conected = false;
    vector<MyPoint> Conecting({MyPoint(-1, 0), MyPoint(1, 0), MyPoint(0, 1), MyPoint(0, -1)});
    for(size_t i = 0; i < New->size(); i++) for(size_t j = 0; j < Conecting.size(); j++)
    {
        MyPoint P = (*New)[i]+Conecting[j] + r;
        if(/*(P.x >= 0 && P.x < int(Map->N)) && (P.y >= 0 && P.y < int(Map->M))*/Map->IsPointIn(P)
            &&(*Map)[P] == ActivePlayer->number) Conected = true;
    }
    return Conected;
}
void PrivatisationGame::AddItem(MyPoint r)
{
    if(!IsTurnPossible(r)) return;
    for(size_t i = 0; i < New->size(); i++) (*Map)[(*New)[i] + r] = ActivePlayer->number;
    ActivePlayer->Score += New->size();
    Pass();
}
void PrivatisationGame::SkipTurn()
{
    if(--ActivePlayer->life<=0)ActivePlayer->RemovePrivatisationPlayer();
    Pass();
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
    if(GenerateBotTurn().x==-1) Reroll();
    if(GenerateBotTurn().x==-1) Reroll();
    else AddItem(GenerateBotTurn());
}
void PrivatisationGame::Reroll()
{
    if(rerolls <=0) SkipTurn();
    else rerolls--;
    New->GenerateNewItem();
}


MyPoint PrivatisationGame::GenerateBotTurn()
{
    vector<MyPoint> Ans;
    vector<MyPoint> Conecting({MyPoint(-1, 0), MyPoint(1, 0), MyPoint(0, 1), MyPoint(0, -1)});
    for(MyPoint P(0,0);P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++)if(IsTurnPossible(P))Ans.push_back(P);
    if(Ans.size()==0)return MyPoint(-1, -1);
    MyPoint BestTurn(-1, -1), P;
    int BestTurnBadness = 99999, NumberOfPlayers = 0, disputed = 0;
    for(auto& i : Players) if(i->T) NumberOfPlayers++;

    queue<MyPoint> Q;
    vector<PrivatisationMap*> distance(Players.size());
    for(size_t i = 0; i < Players.size();i++)
    {
        distance[i] = new PrivatisationMap(Map->N, Map->M);
        if(Players[i]->T==0)continue;
        for(P.x = 0;P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++)if((*Map)[P]==(i+1))
        {
            Q.push(P);
            (*distance[i])[P]=1;
        }
        while(!Q.empty())
        {
            P = Q.front();
            for(auto& v:Conecting) if(Map->IsPointIn(P+v)&&!((*Map)[P+v]||(*distance[i])[P+v]))
            {
                (*distance[i])[P+v]=(*distance[i])[P]+1;
                Q.push(P+v);
            }
            Q.pop();
        }
    }
    for(MyPoint P(0,0);P.x<int(Map->N);P.x++)for(P.y=0;P.y<int(Map->M);P.y++) if((*distance[ActivePlayer->number-1])[P]>1)
    {
        for(size_t i = 0; i < Players.size();i++)if(i+1!=ActivePlayer->number&&(*distance[i])[P] && (*distance[ActivePlayer->number-1])[P] - (*distance[i])[P]>-9)
        {
            disputed++;
            break;
        }
    }
    if(disputed<30)//NumberOfPlayers<=1)
    {
        for(size_t i = 0; i < Players.size();i++) delete distance[i];
        PrivatisationMap Power(Map->N, Map->M);
        for(MyPoint P(0,0);P.x<int(Map->N); P.x++)for(P.y=0;P.y<int(Map->M);P.y++)for(size_t j=0;j<Conecting.size();j++)
                if(!Map->IsPointIn(P+Conecting[j])||(*Map)[P+Conecting[j]]) ++Power[P];
        for(size_t i = 0; i < Ans.size(); i++)
        {
            int Badness = 0;
            for(size_t j = 0; j < New->New.size(); j++) if(Power[(New->New)[j]+Ans[i]]>1)Badness--;
            for(size_t j = 0; j < New->New.size(); j++) (*Map)[((New->New)[j])+(Ans[i])]=int(ActivePlayer->number);
            for(size_t j = 0; j < New->New.size(); j++)for(size_t k=0;k<Conecting.size();k++)
                if(Map->IsPointIn(New->New[j]+Conecting[k])&&(!(*Map)[New->New[j]+Conecting[k]]&&Power[New->New[j]+Conecting[k]]))Badness++;
            for(size_t j = 0; j < New->New.size(); j++) (*Map)[((New->New)[j])+(Ans[i])]=0;
            if(Badness<BestTurnBadness) BestTurn=Ans[i];
            BestTurnBadness=min(Badness, BestTurnBadness);
        }
        if(rerolls&&(BestTurnBadness)>1) return MyPoint(-1,-1);
        return BestTurn;
    }
    else
    {
        PrivatisationMap newDistance(Map->N, Map->M);
        for(size_t i = 0; i < Ans.size(); i++)
        {
            int Badness = 0;
            newDistance.NewGame();
            for(auto& P: New->New)
            {
                Q.push(P+Ans[i]);
                newDistance[P+Ans[i]]=1;
            }
            //if(Ans.size()!=0)return(Ans[size_t(rand()%int(Ans.size()))]);
            while(!Q.empty())
            {
                P = Q.front();
                for(auto& v:Conecting) if(Map->IsPointIn(P+v)&&!((*Map)[P+v]||newDistance[P+v]))
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
       //if(Ans.size()!=0)
           return BestTurn;//return(Ans[size_t(rand()%int(Ans.size()))]);
        //else return MyPoint(-1,-1);
    }
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
int PrivatisationGame::GetActivePlayerID()
{
    if(ActivePlayer) return ActivePlayer->number;
    return 0;
}
PrivatisationGame::~PrivatisationGame()
{
    for(size_t i = 0; i < Players.size(); i++) delete Players[i];
    delete New;
    delete Map;
}
#endif // PRIVATISATION_CPP
