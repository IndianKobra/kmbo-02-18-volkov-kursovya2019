#ifndef PRIVATISATION_H
#define PRIVATISATION_H
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;
class MyPoint
{
    public:
    int x, y;
    MyPoint(){}
    MyPoint(int arg1, int arg2){x=arg1;y=arg2;}
    MyPoint operator+(MyPoint P){return MyPoint(P.x+x, P.y+y);}
};
template<typename Type = int>
class Lvalue
{
private:
    Type* value;
public:
    Lvalue(Type *arg){value = arg;}
    operator Type() {return *value;}
    Type operator=(Type NewValue) {return *value = NewValue;}
};
string Int2Str(int);
int Str2Int(string);
class PrivatisationGame;
class PrivatisationPlayer
{
    protected:
        PrivatisationPlayer(){NextPlayer = this;}
        PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer);
        //int Score = 0, number = 1, T = 1, life = 3;//1 - real player, 2 - bot, 0 - dead
        PrivatisationPlayer *NextPlayer;
        void RemovePrivatisationPlayer();
        void NewGame(){Score=1; life = 3;}
    public:
        int Score = 1, number = 1, T = 1, life = 3;//1 - real player, 2 - bot, 0 - dead
        friend PrivatisationGame;
};
class PrivatisationMap
{
    protected:
        vector<vector<int>> Map;
        size_t N, M;
        PrivatisationMap(){}
        PrivatisationMap(size_t n, size_t m);
        void NewGame(){for(size_t i = 0; i < N; i++)for(size_t j = 0; j < M; j++) Map[i][j] = 0;}
        Lvalue<int> operator[](MyPoint r){return Lvalue<int>(&(Map[size_t(r.x)][size_t(r.y)]));}
    public:
        friend PrivatisationGame;
};
class PrivatisationNew
{
    protected:
        vector<MyPoint> New;
        string Name;
        PrivatisationNew() {GenerateNewItem();}
        void EndGame() {New.resize(0);}
        void GenerateNewItem();
        void NewGame(){GenerateNewItem();}
        MyPoint operator[](size_t i)const{return New[i];}
        size_t size()const{return New.size();}
    public:
        friend PrivatisationGame;

};
class PrivatisationGame
{
protected:
    vector<int> PlayersT;
    vector<PrivatisationPlayer*> Players;
    PrivatisationPlayer *ActivePlayer;
    PrivatisationNew *New;
    PrivatisationMap *Map;
public:
    int rerolls = 1;
    PrivatisationGame(int n, int m, int NumberOfPlayers);
    PrivatisationGame(){}
    void SetPlayersTypes(vector<int> PlayersT)
    {
        for(size_t i = 0; i < Players.size(); i++)if(!PlayersT[i])
        {
            if(ActivePlayer == Players[i]) ActivePlayer = ActivePlayer->NextPlayer;
            Players[i]->RemovePrivatisationPlayer();
        }
        for(size_t i = 0; i < Players.size(); i++) Players[i]->T =PlayersT[i];
    }
    void NewGame()
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
    MyPoint GenerateBotTurn();
    void AddStartPoints()
    {
        if(Players[0]->T!=0)(*Map)[MyPoint(0, 0)] = 1;
        if(Players.size()>1 && Players[1]->T!=0)(*Map)[MyPoint(0, Map->M-1)] = 2;
        if(Players.size()>2 && Players[2]->T!=0)(*Map)[MyPoint(Map->N-1, Map->M-1)] = 3;
        if(Players.size()>3 && Players[3]->T!=0)(*Map)[MyPoint(Map->N-1, 0)] = 4;
        if(ActivePlayer->T==2)DoBotTurn();
    }
    void DoBotTurn();
    bool IsTurnPossible(MyPoint r, bool FirsTurn = false);
    bool AddItem(MyPoint r, bool FirsTurn = false);
    void EndGame();
    bool SkipTurn();
    void Reroll();
    void Pass();
    vector<vector<string>> GenerateScoreTable()
    {
        vector<vector<string>> Ans;
        //int NumberOfPlayers = 0;
        //for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i])NumberOfPlayers++;
        //vector<vector<string>> Ans(NumberOfPlayers);
        for(size_t i = 0; i < PlayersT.size(); i++) if(PlayersT[i])
        {
            vector<string> NewString;
            NewString.push_back(Int2Str(Players[i]->Score));
            NewString.push_back(Int2Str(int(i)+1));
            NewString.push_back(Int2Str(Players[i]->Score*100/int(Map->N*Map->M))+'%');
            Ans.push_back(NewString);
        }
        sort(Ans.begin(), Ans.end(), [](vector<string> A, vector<string> B){return Str2Int(A[0]) > Str2Int(B[0]);});
        //sort(Ans.rbegin(), Ans.rend());
        for(size_t i = 0; i < Ans.size(); i++) Ans[i].push_back(Int2Str(int(i+1)));//[1]=Int2Str(int(i+1));
        //for(size_t i = 0; i < PlayersT.size(); i++)
        return Ans;
    }
    ~PrivatisationGame();
};

#endif // PRIVATISATION_H
