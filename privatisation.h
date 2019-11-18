#ifndef PRIVATISATION_H
#define PRIVATISATION_H
#include <vector>
#include <string>
#include <ctime>
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
string Int2Str(int n);
class PrivatisationGame;
class PrivatisationPlayer
{
    protected:
        PrivatisationPlayer(){NextPlayer = this;}
        PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer);
    public:
        int Score = 0, number = 1, T = 1, life = 3;//1 - real player, 2 - bot, 0 - dead
        PrivatisationPlayer *NextPlayer;
        void RemovePrivatisationPlayer();
        friend PrivatisationGame;
};
class PrivatisationMap
{
    protected:
        vector<vector<int>> Map;
        size_t N, M;
        PrivatisationMap(){}
        //Lvalue<int> operator[](MyPoint r){return Lvalue<int>(&(Map[size_t(r.x)][size_t(r.y)]));}
        PrivatisationMap(size_t n, size_t m);
    public:
        Lvalue<int> operator[](MyPoint r){return Lvalue<int>(&(Map[size_t(r.x)][size_t(r.y)]));}
        friend PrivatisationGame;
};
class PrivatisationNew
{
    protected:
        vector<MyPoint> New;
        string Name;
    public:
        PrivatisationNew() {GenerateNewItem();}
        void EndGame() {New.resize(0);}
        void GenerateNewItem();
        MyPoint operator[](size_t i)const{return New[i];}
        size_t size()const{return New.size();}
};
class PrivatisationGame
{
    protected:
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
            for(size_t i = 0; i < Players.size()-1; i++)if(!PlayersT[i])
            {
                if(ActivePlayer == Players[i]) ActivePlayer = ActivePlayer->NextPlayer;
                Players[i]->RemovePrivatisationPlayer();
            }
            for(size_t i = 0; i < Players.size()-1; i++) Players[i]->T =PlayersT[i];
        }
        void AddStartPoints()
        {
            if(Players[0]->T!=0)(*Map)[MyPoint(0, 0)] = 1;
            if(Players.size()>1 && Players[1]->T!=0)(*Map)[MyPoint(0, Map->M-1)] = 2;
            if(Players.size()>2 && Players[2]->T!=0)(*Map)[MyPoint(Map->N-1, Map->M-1)] = 3;
            if(Players.size()>3 && Players[3]->T!=0)(*Map)[MyPoint(Map->N-1, 0)] = 4;
        }
        bool IsTurnPossible(MyPoint r, bool FirsTurn = false);
        bool AddItem(MyPoint r, bool FirsTurn = false);
        void EndGame();
        bool SkipTurn();
        void Reroll();
        ~PrivatisationGame();
};

#endif // PRIVATISATION_H
