#ifndef QTGAME_H
#define QTGAME_H
#include <QtCore>
#include <QObject>
#include <vector>
//#include <QGraphicsItem>
#include <QPushButton>
#include "privatisation.h"
class GraphWidget;
class QGraphicsItem;
class PrivatisationGame;
class QTPrivatisationNew;
class QTPrivatisationPointTable;
using namespace std;
class QTPrivatisationGame: public QObject, public PrivatisationGame
{
    protected:
    QTPrivatisationPointTable*Table;
    QPushButton*RerollButton;
    QPushButton*StartButton;
    QPushButton*NewGameButton;
    vector<QPushButton*>PlayerTypeButtons;
    QSignalMapper*signalMapper;
    public:
    static vector<enum Qt::GlobalColor> Colors;
    void SkipTurn();
        QTPrivatisationGame(int n, int m);
        QGraphicsItem* GetNew();
        QGraphicsItem* GetMap();
        QPushButton* GetRbutton();
        QPushButton* GetStartbutton();
        QPushButton* GetNewGameButton();
        QPushButton* GetPlayersTBtn(size_t);
        QGraphicsItem* GetPlayer(size_t);
        QGraphicsItem* GetTable();
        void RerollBtnUpdate();
        ~QTPrivatisationGame();
        void EndGame();
    Q_OBJECT
    public slots:
        void StartGame();
        void NewGame();
        void Reroll();
        void ChangeType(int);
};

#endif // QTGAME_H
