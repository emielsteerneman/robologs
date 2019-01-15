//
// Created by emiel on 19-12-18.
//

#include <QWidget>
#include <QPainter>
#include <QBrush>

#include "Field.h"
#include <iostream>
#include <math.h>

Field::Field(QWidget *parent) : QWidget(parent) {
    std::cout << "[Field] New field created" << std::endl;
    setFixedSize(800, 600);
}

void Field::setGameState(const GameState* gameState) {
    this->gameState = gameState;
}

void Field::paintEvent(QPaintEvent *event) {
//    std::cout <<"[Field] paintEvent" << std::endl;

    QPainter painter(this);

    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, this->width(), this->height());

    drawField(painter);

    if(this->gameState == nullptr)
        return;

    painter.setPen(QPen(Qt::white, 0));
    painter.setBrush(QBrush(Qt::red));
    for(const Robot& robot : gameState->yellow.robots){
        int x = (int)(this->width() * (robot.x_buf.avg() + 6000) / 12000);
        int y = (int)(this->height()* (robot.y_buf.avg() + 4500) / 9000);

        double xVel = robot.x_buf.avgVel();
        double yVel = robot.y_buf.avgVel();

        double xRot = std::cos(robot.rot) * 20;
        double yRot = std::sin(robot.rot) * 20;

        painter.drawEllipse(x, y, 10, 10);
//        painter.drawLine(x+5, y+5, x+5 + xVel, y+5);
//        painter.drawLine(x+5, y+5, x+5, y+5 + yVel);

        painter.drawLine(x+5, y+5, x+5 + xRot, y+5 + yRot);
    }

    painter.setBrush(QBrush(Qt::blue));
    for(const Robot& robot : gameState->blue.robots){
        int x = (int)(this->width() * (robot.x_buf.avg() + 6000) / 12000);
        int y = (int)(this->height()* (robot.y_buf.avg() + 4500) / 9000);

        double xVel = robot.x_buf.avgVel();
        double yVel = robot.y_buf.avgVel();

        double xRot = std::cos(robot.rot) * 20;
        double yRot = std::sin(robot.rot) * 20;

        painter.drawEllipse(x, y, 10, 10);
//        painter.drawLine(x+5, y+5, x+5 + xVel, y+5);
//        painter.drawLine(x+5, y+5, x+5, y+5 + yVel);

        painter.drawLine(x+5, y+5, x+5 + xRot, y+5 + yRot);
    }

    painter.setBrush(QBrush(Qt::white));
    int x = (int)(this->width() * (gameState->ball.x_buf.avg() + 6000) / 12000);
    int y = (int)(this->height()* (gameState->ball.y_buf.avg() + 4500) / 9000);
    painter.drawEllipse(x, y, 10, 10);

}

void Field::drawField(QPainter& painter){

    painter.setPen(QPen(Qt::white, 1));

    using namespace FieldGeometry;

    painter.drawLine(FieldTopLeft    , FieldTopRight);
    painter.drawLine(FieldTopRight   , FieldBottomRight);
    painter.drawLine(FieldBottomRight, FieldBottomLeft);
    painter.drawLine(FieldBottomLeft , FieldTopLeft);

    painter.drawLine(PenaltyLeftTopLeft    , PenaltyLeftTopRight);
    painter.drawLine(PenaltyLeftTopRight   , PenaltyLeftBottomRight);
    painter.drawLine(PenaltyLeftBottomRight, PenaltyLeftBottomLeft);

    painter.drawLine(PenaltyRightTopLeft    , PenaltyRightTopRight);
    painter.drawLine(PenaltyRightTopLeft    , PenaltyRightBottomLeft);
    painter.drawLine(PenaltyRightBottomRight, PenaltyRightBottomLeft);

    painter.drawLine(HalfHorizontalTop, HalfHorizontalBottom);
}











