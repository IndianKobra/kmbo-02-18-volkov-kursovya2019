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
    QPushButton*ngBtn;
    QPushButton*NewGameBtn;
    vector<QPushButton*> PTbnt;
    public:
    static vector<enum Qt::GlobalColor> Colors;
    bool SkipTurn();
        QTPrivatisationGame(int n, int m, GraphWidget *graphWidget);
        QGraphicsItem* GetNew();
        QGraphicsItem* GetMap();
        QPushButton* GetRbutton();
        QPushButton* GetNGbutton();
        QPushButton* GetNewGameBtn();
        QPushButton* GetPlayersTBtn(size_t);
        QGraphicsItem* GetActivePlayer();
        QGraphicsItem* GetPlayer(size_t i);
        void RerollBtnUpdate();
        ~QTPrivatisationGame();
        int GetActivePlayerID();
        void EndGame();
        void ClearMap();

    private:
        Q_OBJECT
        public slots:
            void StartGame();
            void NewGame();
            void Reroll();
            void ChangeType(int);
};

#endif // QTGAME_H
