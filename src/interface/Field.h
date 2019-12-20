//
// Created by emiel on 19-12-18.
//

#ifndef ROBOLOGS_FIELD_H
#define ROBOLOGS_FIELD_H


#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "game/GameStateTracker.h"


class Field : public QWidget {
Q_OBJECT
public:
    //Todo make this a reference or pointer
    const GameState* gameState = nullptr;

    explicit Field(QWidget *parent = 0);
    void setGameState(const GameState* gameState);
    void paintEvent(QPaintEvent *event) override;

    void drawField(QPainter& painter);
};

namespace FieldGeometry {

    const int FieldWidth = 800;
    const int FieldHeight = 600;

    const QPoint BorderTopLeft     = QPoint(0  , 0);
    const QPoint BorderTopRight    = QPoint(669, 0);
    const QPoint BorderBottomLeft  = QPoint(0  , 519);
    const QPoint BorderBottomRight = QPoint(669, 519);

    const QPoint FieldTopLeft     = QPoint(30 , 30);
    const QPoint FieldTopRight    = QPoint(629, 30);
    const QPoint FieldBottomLeft  = QPoint(30 , 479);
    const QPoint FieldBottomRight = QPoint(629, 479);

    const QPoint PenaltyLeftTopLeft     = QPoint(0  , 225);
    const QPoint PenaltyLeftTopRight    = QPoint(80 , 225);
    const QPoint PenaltyLeftBottomLeft  = QPoint(0  , 380);
    const QPoint PenaltyLeftBottomRight = QPoint(80 , 380);

    const QPoint PenaltyRightTopLeft     = QPoint(720, 220);
    const QPoint PenaltyRightTopRight    = QPoint(799, 220);
    const QPoint PenaltyRightBottomLeft  = QPoint(720, 380);
    const QPoint PenaltyRightBottomRight = QPoint(799, 380);

    const QPoint HalfHorizontalTop    = QPoint(400, 0);
    const QPoint HalfHorizontalBottom = QPoint(400, 599);
}

#endif //ROBOLOGS_FIELD_H
