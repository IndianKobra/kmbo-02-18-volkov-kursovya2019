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
class QTPrivatisationNew;
class QTPrivatisationPointTable;
using namespace std;
class QTPrivatisationGame: public QObject  , public PrivatisationGame
{
    protected:
    QTPrivatisationPointTable*Table;
    QPushButton*rBtn;
    QPushButton*ngBtn;
    QPushButton*NewGameBtn;
    vector<QPushButton*> PTbnt;
    QSignalMapper *signalMapper;
    public:
    static vector<enum Qt::GlobalColor> Colors;
    bool SkipTurn();
        QTPrivatisationGame(int n, int m);
        QGraphicsItem* GetNew();
        QGraphicsItem* GetMap();
        QPushButton* GetRbutton();
        QPushButton* GetNGbutton();
        QPushButton* GetNewGameBtn();
        QPushButton* GetPlayersTBtn(size_t);
        QGraphicsItem* GetActivePlayer();
        QGraphicsItem* GetPlayer(size_t i);
        QGraphicsItem* GetTable();
        void RerollBtnUpdate();
        ~QTPrivatisationGame();
        int GetActivePlayerID();
        void EndGame();
    private:
        Q_OBJECT
        public slots:
            void StartGame();
            void NewGame();
            void Reroll();
            void ChangeType(int);
};

#endif // QTGAME_H
