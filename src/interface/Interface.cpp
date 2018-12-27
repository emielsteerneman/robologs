
#include <iostream>
#include <math.h>
#include <QApplication>
#include <QString>
#include <QProgressBar>
#include <QSlider>

#include "Interface.h"

Interface::Interface(QWidget *parent) : QWidget(parent) {
    std::cout << "[Interface] New interface created" << std::endl;
    std::cout << "[Interface] width=" << this->width() << " height=" << this->height() << std::endl;

    this->setStyleSheet("background-color: #333");

    gameStageCommand = new QLineEdit(this);
    gameStageCommand->setStyleSheet("color: white");

    field = new Field(this);
    timeline = new Timeline(this);
}

void Interface::resizeEvent(QResizeEvent *event) {
    std::cout << "[Interface][resizeEvent] width=" << this->width() << " height=" << this->height() << std::endl;

    int height = this->height();

    int fieldOffset = (height - field->height()) / 2;
    field->move(fieldOffset, fieldOffset);

    gameStageCommand->setGeometry(fieldOffset, fieldOffset-20, field->width(), 20);

    timeline->setGeometry(10, 10, this->width() - 20, 50);
}

void Interface::setGameInfo(const GameInfo* gameInfo){
    this->gameInfo = gameInfo;
    std::cout << "[Interface] setGameInfo" << std::endl;

    timeline->setGameInfo(gameInfo);
}

void Interface::updateGameState(const GameState* gameState) {
//    std::cout << "[Interface] Updating game state" << std::endl;

    if(gameInfo != nullptr){
        double progress = (gameState->timestamp - gameInfo->t_start) / (gameInfo->t_stop - gameInfo->t_start);
        timeline->setProgress(progress);
    }

    gameStageCommand->setText(QString::fromStdString(gameState->stage + " | " + gameState->command));

    field->setGameState(gameState);
    field->update();
}