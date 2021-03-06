/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "QTPrivatisation.h"
#include <QGraphicsItem>
#include <math.h>
#include <QKeyEvent>
#include <vector>
#include <QGraphicsProxyWidget>
#include "QTPrivatisationGame.h"


void AddPrivatisationItems2Scene(QTPrivatisationGame* Game, QGraphicsScene *scene)
{
    (scene->addWidget(Game->GetRbutton()))->setGeometry(QRectF(-129, 110, 30, 50));
    (scene->addWidget(Game->GetNewGameButton()))->setGeometry(QRectF(-55, -150, 30, 25));
    (scene->addWidget(Game->GetStartbutton()))->setGeometry(QRectF(-55, -150, 30, 25));
    (scene->addWidget(Game->GetPlayersTBtn(0)))->setGeometry(QRectF(-190, -150, 30, 25));
    (scene->addWidget(Game->GetPlayersTBtn(1)))->setGeometry(QRectF(75, -150, 30, 25));
    (scene->addWidget(Game->GetPlayersTBtn(2)))->setGeometry(QRectF(75, 120, 30, 25));
    (scene->addWidget(Game->GetPlayersTBtn(3)))->setGeometry(QRectF(-190, 120, 30, 25));
    scene->addItem(Game->GetPlayer(0));
    scene->addItem(Game->GetPlayer(1));
    scene->addItem(Game->GetPlayer(2));
    scene->addItem(Game->GetPlayer(3));
    scene->addItem(Game->GetMap());
    scene->addItem(Game->GetNew());
    scene->addItem(Game->GetTable());
    Game->GetPlayer(0)->setPos(-180, -140);
    Game->GetPlayer(1)->setPos(150, -140);
    Game->GetPlayer(2)->setPos(150, 120);
    Game->GetPlayer(3)->setPos(-180, 120);
    Game->GetMap()->setPos(-150, -100);
    Game->GetNew()->setPos(0, 110);
    Game->GetTable()->setPos(0, 105);
}
GraphWidget::GraphWidget(QWidget *parent): QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Privatisation Game"));
    Game = new QTPrivatisationGame(20, 30, 4);
    AddPrivatisationItems2Scene(Game, scene);
    srand (time(0));
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:

    case Qt::Key_Enter:
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);


}

#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    QRectF textRect(sceneRect.left()+4, sceneRect.top()+4, sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Privatiosation game"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100) return;
    scale(scaleFactor, scaleFactor);
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
GraphWidget::~GraphWidget()
{
    delete Game;
}
