
#include <iostream>
#include <math.h>
#include <QApplication>
#include <QString>
#include <QProgressBar>
#include <QSlider>

#include "Interface.h"
#include "../Utilities.h"

Interface::Interface(QWidget *parent) : QWidget(parent) {
    std::cout << "[Interface] New interface created" << std::endl;
    std::cout << "[Interface] width=" << this->width() << " height=" << this->height() << std::endl;

    this->setMinimumSize(690, 620);

    this->setStyleSheet("background-color: rgb(255, 255, 255); color: white;");

    lGameCommand = new QLabel(this);
    lGameCommand->setAlignment(Qt::AlignCenter);
    lGameCommand->setAttribute(Qt::WA_TranslucentBackground, true);

    lTeamYellow = new QLabel(this);
    lTeamYellow->setAlignment(Qt::AlignLeft);
    lTeamYellow->setAttribute(Qt::WA_TranslucentBackground, true);
    lTeamYellow->setStyleSheet("color: rgb(255, 196, 0);");

    lTeamBlue = new QLabel(this);
    lTeamBlue->setAlignment(Qt::AlignRight);
    lTeamBlue->setAttribute(Qt::WA_TranslucentBackground, true);
    lTeamBlue->setStyleSheet("color: rgb(0, 100, 255);");

    lStats = new QLabel(this);
    lStats->setStyleSheet("Font : 8pt");

    field = new Field(this);
    timeline = new Timeline(this);

    connect(timeline, SIGNAL(signalProgress(double)), this, SIGNAL(signalProgress(double)));
}

void Interface::resizeEvent(QResizeEvent *event) {
    std::cout << "[Interface][resizeEvent] width=" << this->width() << " height=" << this->height() << std::endl;

//    int height = this->height();
//    int width = this->width();

    int width = 670;

    timeline->setGeometry(10, 10, this->width() - 20, 50);

    field->move(std::max((width - field->width()) / 2, 0), 100);

    lGameCommand->setGeometry(field->x(), field->y() - 20, field->width(), 20);
    lTeamYellow ->setGeometry(field->x(), field->y() - 20, field->width(), 20);
    lTeamBlue   ->setGeometry(field->x(), field->y() - 20, field->width(), 20);

    lStats->setGeometry(field->x(), field->y() + field->height() + 5, field->width(), 10);

}

void Interface::setGameInfo(const GameInfo* gameInfo){
    this->gameInfo = gameInfo;
    std::cout << "[Interface] setGameInfo" << std::endl;

    timeline->setGameInfo(gameInfo);

    std::string sStats = "vision packets : " + std::to_string(gameInfo->vision_nPackets);
    sStats += "  |  referee packets : " + std::to_string(gameInfo->referee_nPackets);
    sStats += "  |  total duration : " + u::durationToString(gameInfo->t_stop - gameInfo->t_start);
    lStats->setText(QString::fromStdString(sStats));
}

void Interface::updateGameState(const GameState* gameState) {
//    std::cout << "[Interface] Updating game state" << std::endl;

    if(gameInfo != nullptr){
        double progress = (gameState->timestamp - gameInfo->t_start) / (gameInfo->t_stop - gameInfo->t_start);
        timeline->setProgress(progress);
    }

    lGameCommand->setText(QString::fromStdString(gameState->command));
    lTeamYellow->setText(QString::fromStdString(gameState->yellow.name));
    lTeamBlue->setText(QString::fromStdString(gameState->blue.name));

    field->setGameState(gameState);
    field->update();
}