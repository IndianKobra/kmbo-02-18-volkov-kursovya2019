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
        enum Status {dead = 0, realPlayer = 1, bot = 2};
        PrivatisationPlayer(){NextPlayer = this;}
        PrivatisationPlayer(PrivatisationPlayer *PreviousPlayer);
        Status status = realPlayer;
        unsigned int id = 1, Score = 1, life = 3;
        PrivatisationPlayer *NextPlayer;
        void removePrivatisationPlayer();
        void newGame(){Score=1; life = 3;}
        friend PrivatisationGame;
};
class PrivatisationMap
{
    protected:
        vector<vector<int>> Map;
        size_t N, M;
        PrivatisationMap(size_t n, size_t m);
        void newGame(){for(size_t i = 0; i < N; i++)for(size_t j = 0; j < M; j++) Map[i][j] = 0;}
        Lvalue<int> operator[](MyPoint r){return Lvalue<int>(&(Map[size_t(r.x)][size_t(r.y)]));}
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
        friend PrivatisationGame;
};
class PrivatisationGame
{
protected:
    vector<PrivatisationPlayer::Status> PlayersT;
    vector<PrivatisationPlayer*> Players;
    PrivatisationPlayer *ActivePlayer;
    PrivatisationNew *New;
    PrivatisationMap *Map;
    PrivatisationGame(){}
    int rerolls = 1;
public:
    PrivatisationGame(int n, int m, int numberOfPlayers);
    void changeType(size_t i);
    void setPlayersTypes(vector<PrivatisationPlayer::Status> PlayersT);
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
