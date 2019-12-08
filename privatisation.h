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
        Type operator++(){return (*value)++;}
};
string Int2Str(int);
int Str2Int(string);
class PrivatisationGame;
class PrivatisationPlayer
{
    protected:
        PrivatisationPlayer(){NextPlayer = this;}
        PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer);
        int Score = 1, T = 1, life = 3;//1 - real player, 2 - bot, 0 - dead
        unsigned int number = 1;
        PrivatisationPlayer *NextPlayer;
        void RemovePrivatisationPlayer();
        void NewGame(){Score=1; life = 3;}
    public:
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
        bool IsPointIn(MyPoint P){return (P.x >= 0 && P.x < int(N)) && (P.y >= 0 && P.y < int(M));}
    public:
        friend PrivatisationGame;
};
class PrivatisationNew
{
    protected:
        vector<MyPoint> New;
        string Name;
        PrivatisationNew() {GenerateNewItem();}
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
    PrivatisationGame(){}
public:
    int rerolls = 1;
    PrivatisationGame(int n, int m, int NumberOfPlayers);
    void SetPlayersTypes(vector<int> PlayersT);
    void NewGame();
    MyPoint GenerateBotTurn();
    void AddStartPoints();
    void DoBotTurn();
    bool IsTurnPossible(MyPoint r);
    void AddItem(MyPoint r);
    void EndGame();
    void SkipTurn();
    void Reroll();
    void Pass();
    int GetActivePlayerID();
    bool IsGameFinished(){return !(ActivePlayer&&ActivePlayer->life);}
    vector<vector<string>> GenerateScoreTable();
    ~PrivatisationGame();
};

#endif // PRIVATISATION_H
