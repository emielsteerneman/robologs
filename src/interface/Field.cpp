//
// Created by emiel on 19-12-18.
//

#include <QWidget>
#include <QPainter>
#include <QBrush>

#include "Field.h"
#include <iostream>
#include <math.h>

Field::Field(QWidget *parent, int _fieldWidth, int _fieldHeight, int _fieldPadding)
: QWidget(parent), fieldWidth(_fieldWidth), fieldHeight(_fieldHeight), fieldPadding(_fieldPadding){
    setFixedSize(fieldWidth + 2 * fieldPadding, fieldHeight + 2 * fieldPadding);
    std::cout << "[Field] New field created; Width=" << width() << " Height=" << height() << std::endl;
}

void Field::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    painter.setBrush(QBrush(QColor::fromRgb(51, 102, 0)));
    painter.drawRect(0, 0, fieldWidth + 2 * fieldPadding - 1, fieldHeight + 2 * fieldPadding - 1);
    drawField(painter);

    if(this->gameState == nullptr)
        return;

    painter.setPen(QPen(Qt::white, 0));
    painter.setBrush(QBrush(QColor::fromRgb(255, 196, 0)));
    for(const Robot& robot : gameState->yellow.robots){
        int x = (int)(fieldWidth * (robot.x_buf.avg() + 6000) / 12000);
        int y = (int)(fieldHeight* (robot.y_buf.avg() + 4500) / 9000);
        painter.drawEllipse(x + fieldPadding, y + fieldPadding, 10, 10);

        double xRot = std::cos(robot.rot) * 20;
        double yRot = std::sin(robot.rot) * 20;
        painter.drawLine(x+5 + fieldPadding, y+5 + fieldPadding, x+5+xRot + fieldPadding, y+5+yRot + fieldPadding);
    }

    painter.setBrush(QBrush(QColor::fromRgb(0, 100, 255)));
    for(const Robot& robot : gameState->blue.robots){
        int x = (int)(fieldWidth * (robot.x_buf.avg() + 6000) / 12000);
        int y = (int)(fieldHeight* (robot.y_buf.avg() + 4500) / 9000);
        painter.drawEllipse(x + fieldPadding, y + fieldPadding, 10, 10);

        double xRot = std::cos(robot.rot) * 20;
        double yRot = std::sin(robot.rot) * 20;
        painter.drawLine(x+5 + fieldPadding, y+5 + fieldPadding, x+5+xRot + fieldPadding, y+5+yRot + fieldPadding);
    }

    painter.setBrush(QBrush(QColor::fromRgb(255, 137, 0)));
    int x = (int)(fieldWidth * (gameState->ball.x_buf.avg() + 6000) / 12000);
    int y = (int)(fieldHeight* (gameState->ball.y_buf.avg() + 4500) / 9000);
    painter.drawEllipse(x + fieldPadding, y+fieldPadding, 7, 7);

}

void Field::setGameState(const GameState* gameState) {
    this->gameState = gameState;
}

void Field::drawField(QPainter& painter){
//    using namespace FieldGeometry;

    painter.setPen(QPen(Qt::black, 1));

//    painter.drawLine(BorderTopLeft    , BorderTopRight);
//    painter.drawLine(BorderTopRight   , BorderBottomRight);
//    painter.drawLine(BorderBottomRight, BorderBottomLeft);
//    painter.drawLine(BorderBottomLeft , BorderTopLeft);

    painter.setPen(QPen(Qt::white, 1));

    const QPoint FieldTopLeft     = QPoint(fieldPadding , fieldPadding);
    const QPoint FieldTopRight    = QPoint(fieldWidth + fieldPadding, fieldPadding);
    const QPoint FieldBottomLeft  = QPoint(fieldPadding , fieldHeight + fieldPadding);
    const QPoint FieldBottomRight = QPoint(fieldWidth + fieldPadding, fieldHeight + fieldPadding);

    painter.drawLine(FieldTopLeft    , FieldTopRight);
    painter.drawLine(FieldTopRight   , FieldBottomRight);
    painter.drawLine(FieldBottomRight, FieldBottomLeft);
    painter.drawLine(FieldBottomLeft , FieldTopLeft);

    const QPoint PenaltyLeftTopLeft    (fieldPadding, fieldPadding + 4 * fieldHeight / 10);
    const QPoint PenaltyLeftBottomLeft (fieldPadding, fieldPadding + 6 * fieldHeight / 10);
    const QPoint PenaltyLeftTopRight   (fieldPadding + fieldWidth / 10, fieldPadding + 4 * fieldHeight / 10);
    const QPoint PenaltyLeftBottomRight(fieldPadding + fieldWidth / 10, fieldPadding + 6 * fieldHeight / 10);

    const QPoint PenaltyRightTopLeft    (fieldPadding + 9 * fieldWidth / 10, fieldPadding + 4 * fieldHeight / 10);
    const QPoint PenaltyRightBottomLeft (fieldPadding + 9 * fieldWidth / 10, fieldPadding + 6 * fieldHeight / 10);
    const QPoint PenaltyRightTopRight   (fieldPadding + fieldWidth, fieldPadding + 4 * fieldHeight / 10);
    const QPoint PenaltyRightBottomRight(fieldPadding + fieldWidth, fieldPadding + 6 * fieldHeight / 10);

    const QPoint HalfHorizontalTop    = QPoint(fieldWidth / 2 + fieldPadding, fieldPadding);
    const QPoint HalfHorizontalBottom = QPoint(fieldWidth / 2 + fieldPadding, fieldPadding + fieldHeight);

    painter.drawLine(PenaltyLeftTopLeft    , PenaltyLeftTopRight);
    painter.drawLine(PenaltyLeftTopRight   , PenaltyLeftBottomRight);
    painter.drawLine(PenaltyLeftBottomRight, PenaltyLeftBottomLeft);

    painter.drawLine(PenaltyRightTopLeft    , PenaltyRightTopRight);
    painter.drawLine(PenaltyRightTopLeft    , PenaltyRightBottomLeft);
    painter.drawLine(PenaltyRightBottomRight, PenaltyRightBottomLeft);

    painter.drawEllipse(fieldPadding + fieldWidth/2 -15, fieldPadding + fieldHeight/2 -15, 30, 30);
    painter.drawLine(HalfHorizontalTop, HalfHorizontalBottom);

}











