#ifndef PRIVATISATION_H
#define PRIVATISATION_H
#include <ctime>
#include <vector>
#include <string>


using namespace std;
class MyPoint
{
    public:
    int x = 0, y = 0;
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
class PrivatisationGame;
class PrivatisationPlayer
{
    protected:
        PrivatisationPlayer(){NextPlayer = this;}
        PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer);
        int Score = 1, T = 1, life = 3;//1 - real player, 2 - bot, 0 - dead
        unsigned int id = 1;
        PrivatisationPlayer *NextPlayer;
        void removePrivatisationPlayer();
        void newGame(){Score=1; life = 3;}
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
        void newGame(){for(size_t i = 0; i < N; i++)for(size_t j = 0; j < M; j++) Map[i][j] = 0;}
        Lvalue<int> operator[](MyPoint r){return Lvalue<int>(&(Map[size_t(r.x)][size_t(r.y)]));}
    public:
        friend PrivatisationGame;
};
class PrivatisationNew
{
    protected:
        vector<MyPoint> New;
        PrivatisationNew() {generateNewItem();}
        void generateNewItem();
        void newGame(){generateNewItem();}
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
    PrivatisationGame(int n, int m, int idOfPlayers);
    void setPlayersTypes(vector<int> PlayersT);
    void newGame();
    MyPoint generateBotTurn();
    void addStartPoints();
    void doBotTurn();
    bool isTurnPossible(MyPoint r);
    void addItem(MyPoint r);
    void endGame();
    void skipTurn();
    void reroll();
    void pass();
    int getActivePlayerID();
    bool isPointIn(MyPoint P);
    bool isGameFinished(){return !(ActivePlayer&&ActivePlayer->life);}
    vector<vector<string>> generateScoreTable();
    ~PrivatisationGame();
};

#endif // PRIVATISATION_H
