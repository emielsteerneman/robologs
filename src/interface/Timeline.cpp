//
// Created by emiel on 20-12-18.
//

#include <iostream>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPainterPath>

#include "Timeline.h"

Timeline::Timeline(QWidget* parent) : QWidget(parent){
    std::cout << "[Timeline] New Timeline created" << std::endl;
    this->setStyleSheet("background-color: #999");
    wBackground = new TimelineBackground(this);
    wProgress = new TimelineProgress(this);
}

void Timeline::resizeEvent(QResizeEvent *event){
    int dHeight = this->height() / 3;
    std::cout << "[Timeline] resizeEvent" << std::endl;
    wProgress->setGeometry(0, 0, this->width(), dHeight);
    wBackground->setGeometry(0, dHeight, this->width(), dHeight * 2);
}

void Timeline::setGameInfo(const GameInfo* gameInfo) {
    std::cout << "[Timeline] setGameInfo" << std::endl;
    wBackground->gameInfo = gameInfo;
    wBackground->update();
}

void Timeline::setProgress(double progress) {
    std::cout << "[Timeline] setProgress" << std::endl;
    wProgress->progress = progress;
    wProgress->update();
}

TimelineBackground::TimelineBackground(QWidget* parent) : QWidget(parent){
    std::cout << "[TimelineBackground] Created " << std::endl;
}

void TimelineBackground::paintEvent(QPaintEvent *event) {
    std::cout << "[TimelineBackground] paintEvent" << std::endl;
    if(gameInfo == nullptr)
        return;

    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    // Fill background with a black square
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, this->width(), this->height());

    const double duration = gameInfo->t_stop - gameInfo->t_start;
    const int width = this->width();
    const int height = this->height();

    // Draw timeline backwards so that only one object is needed at all times
    int atWidth = width;
    for(auto i = gameInfo->timeline.size() - 1; 0 < i; i--){
        const auto& obj = gameInfo->timeline.at(i);
        double start = std::get<0>(obj);
        std::string stage = std::get<1>(obj);
        std::string command = std::get<2>(obj);

        int nextWidth = (int)(width * (start - gameInfo->t_start) / duration);

        // Draw stage
        painter.setBrush(getStageColor(stage));
        painter.drawRect(nextWidth, 0, atWidth - nextWidth, height);

       // Draw command
        painter.setBrush(getCommandColor(command));
        painter.drawRect(nextWidth, 5, atWidth - nextWidth, height - 10);

        atWidth = nextWidth;
    }
}

QColor TimelineBackground::getStageColor(std::string stage) {
    if(stage == "NORMAL_FIRST_HALF")        return QColor(100, 100, 0);
    if(stage == "EXTRA_FIRST_HALF")         return QColor(100, 100, 0);
    if(stage == "NORMAL_HALF_TIME")         return QColor(100, 100, 100);
    if(stage == "EXTRA_SECOND_HALF")       return QColor(0, 100, 100);

    return QColor(100, 100, 100);
}

QColor TimelineBackground::getCommandColor(std::string command) {
    if(command == "STOP")                   return QColor(100, 100, 100);

    if(command == "NORMAL_START")           return QColor(200, 200, 200);
    if(command == "DIRECT_FREE_YELLOW")     return QColor(200, 200, 200);
    if(command == "INDIRECT_FREE_YELLOW")   return QColor(200, 200, 200);
    if(command == "DIRECT_FREE_BLUE")       return QColor(200, 200, 200);
    if(command == "INDIRECT_FREE_BLUE")     return QColor(200, 200, 200);

    if(command == "TIMEOUT_YELLOW")         return QColor(255, 0, 0);
    if(command == "TIMEOUT_BLUE")           return QColor(0, 0, 255);

    return QColor(0, 0, 0);

}



TimelineProgress::TimelineProgress(QWidget* parent) : QWidget(parent) {
    std::cout << "[TimelineProgress] Created " << std::endl;
}

void TimelineProgress::paintEvent(QPaintEvent *event) {
//    std::cout << "[TimelineProgress] Drawing progress" << std::endl;

    QPainter painter(this);

    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, this->width(), this->height());

    const int width = this->width();
    const int height = this->height();

    // Draw progress triangle
    int arrowX = (int)(width * progress);

//    std::cout << "[TimelineProgress] progress=" << progress << " arrowX=" << arrowX << " arrowY=" << arrowY << std::endl;

    painter.setPen(QPen(Qt::white, 1));
    QPainterPath path;
    path.moveTo(arrowX - 7, 1);
    path.lineTo(arrowX, height-1);
    path.lineTo(arrowX + 7, 1);
    path.lineTo(arrowX - 7, 1);
//    painter.fillPath(path, QBrush(Qt::white));
    painter.drawPath(path);
    painter.setPen(Qt::NoPen);
}