#ifndef QTGAME_H
#define QTGAME_H
#include <QtCore>
#include <QObject>
#include <vector>
#include <QGraphicsItem>
#include <QPushButton>
#include "privatisation.h"
class GraphWidget;
class PrivatisationGame;
using namespace std;
class QTPrivatisationGame: public QObject  , public PrivatisationGame
{
    protected:
    QPushButton*rBtn;
    public:
    static vector<enum Qt::GlobalColor> Colors;
    bool SkipTurn();
        QTPrivatisationGame(int n, int m, vector<int> PlayersT, GraphWidget *graphWidget);
        QGraphicsItem* GetNew();
        QGraphicsItem* GetMap();
        QGraphicsItem* GetActivePlayer();
        QGraphicsItem* GetPlayer(size_t i);
        void RerollBtnUpdate();
        virtual ~QTPrivatisationGame(){}
        int GetActivePlayerID();
        void EndGame();
    private:
        Q_OBJECT
        public slots:
            void Reroll();

};

#endif // QTGAME_H
