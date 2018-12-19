//
// Created by emiel on 19-12-18.
//

#include <QWidget>
#include <QPainter>

#include "Field.h"
#include <iostream>

Field::Field(QWidget *parent) : QWidget(parent) {
    std::cout << "[Field] New field created" << std::endl;
    setFixedSize(1200, 900);
}

void Field::drawGameState(const GameState gameState){
    QPainter painter(this);

    painter.drawRect(10, 10, this->width() - 10, this->height() - 10);
}