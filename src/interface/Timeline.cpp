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
#include "../Utilities.h"

Timeline::Timeline(QWidget* parent) : QWidget(parent){
    std::cout << "[Timeline] New Timeline created" << std::endl;
    this->setStyleSheet("background-color: #999");
    wBackground = new TimelineBackground(this);
    wProgress = new TimelineProgress(this);

    lTimeStart = new QLabel(this);
    lTimeStart->setAlignment(Qt::AlignLeft);
    lTimeStart->setAttribute(Qt::WA_TranslucentBackground, true);

    lProgress = new QLabel(this);
    lProgress->setAlignment(Qt::AlignCenter);
    lProgress->setAttribute(Qt::WA_TranslucentBackground, true);

    lTimeStop = new QLabel(this);
    lTimeStop->setAlignment(Qt::AlignRight);
    lTimeStop->setAttribute(Qt::WA_TranslucentBackground, true);
}

void Timeline::resizeEvent(QResizeEvent *event){
    int dHeight = this->height() / 5;
    std::cout << "[Timeline] resizeEvent" << std::endl;
    wProgress->setGeometry(0, 0, this->width(), dHeight);
    wBackground->setGeometry(0, dHeight, this->width(), dHeight * 2);

    lTimeStart->setGeometry(0, dHeight * 3 + 1, this->width(), dHeight * 2);
    lTimeStop ->setGeometry(0, dHeight * 3 + 1, this->width(), dHeight * 2);
    lProgress ->setGeometry(0, dHeight * 3 + 1, this->width(), dHeight * 2);
}

void Timeline::setGameInfo(const GameInfo* gameInfo) {
    std::cout << "[Timeline] setGameInfo" << std::endl;
    this->gameInfo = gameInfo;
    wBackground->gameInfo = gameInfo;
    wBackground->update();

    lTimeStart->setText(QString::fromStdString(u::timeToString(gameInfo->t_start)));
    lTimeStop->setText(QString::fromStdString(u::timeToString(gameInfo->t_stop)));
}

void Timeline::setProgress(double progress) {
//    std::cout << "[Timeline] setProgress" << std::endl;
    wProgress->progress = progress;
    wProgress->update();

    std::string sProgress = u::durationToString(progress * (gameInfo->t_stop - gameInfo->t_start));

    lProgress->setText(QString::fromStdString(sProgress));
}

void Timeline::mouseReleaseEvent(QMouseEvent *event) {
    double d = event->pos().x() / (double)this->width();
    emit signalProgress(d);
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
    for(int i = gameInfo->timeline.size() - 1; 0 <= i; i--){ // Don't use auto! Casts to unsigned long, gives underflow wraparound bug
        const auto& obj = gameInfo->timeline.at(i);
        double start = std::get<0>(obj);
        const std::string& stage = std::get<1>(obj);
        const std::string& command = std::get<2>(obj);
        int nextWidth = (int)(width * (start - gameInfo->t_start) / duration);

        // Draw stage
        painter.setBrush(getStageColor(stage));
        painter.drawRect(nextWidth, 0, atWidth - nextWidth, height);

       // Draw command
        painter.setBrush(getCommandColor(command));
        painter.drawRect(nextWidth, height/6, atWidth - nextWidth, 4*height/6);

        atWidth = nextWidth;
    }
}

QColor TimelineBackground::getStageColor(std::string stage) {
    if(stage == "NORMAL_FIRST_HALF")        return {200, 200, 200};
    if(stage == "EXTRA_FIRST_HALF")         return {200, 200, 200};
    if(stage == "NORMAL_HALF_TIME")         return {100, 100, 100};
    if(stage == "NORMAL_SECOND_HALF")       return {200, 200, 200};
    if(stage == "EXTRA_SECOND_HALF")        return {200, 200, 200};

    return {100, 100, 100};
}

QColor TimelineBackground::getCommandColor(std::string command) {
    if(command == "STOP")                   return QColor(20, 20, 20);

    if(command == "NORMAL_START")           return QColor(200, 200, 200, 0);
    if(command == "DIRECT_FREE_YELLOW")     return QColor(200, 200, 200, 0);
    if(command == "INDIRECT_FREE_YELLOW")   return QColor(200, 200, 200, 0);
    if(command == "DIRECT_FREE_BLUE")       return QColor(200, 200, 200, 0);
    if(command == "INDIRECT_FREE_BLUE")     return QColor(200, 200, 200, 0);

    if(command == "TIMEOUT_YELLOW")         return QColor(255, 0, 0);
    if(command == "TIMEOUT_BLUE")           return QColor(0, 0, 255);

    return QColor(0, 0, 0);

}




TimelineProgress::TimelineProgress(QWidget* parent) : QWidget(parent) {
    std::cout << "[TimelineProgress] Created " << std::endl;
}

void TimelineProgress::paintEvent(QPaintEvent *event) {
//    std::cout << "[TimelineProgress] Drawing progress" << std::endl;

    if(progress < 0)
        return;

    QPainter painter(this);

    painter.setBrush(QBrush(Qt::white));
    const int width = this->width();
    const int height = this->height();

    // Draw progress triangle
    int arrowX = (int)(width * progress);

//    std::cout << "[TimelineProgress] progress=" << progress << " arrowX=" << arrowX << " arrowY=" << arrowY << std::endl;

    painter.setPen(QPen(Qt::white, 1));
    QPainterPath path;
    path.moveTo(arrowX - 5, 2);
    path.lineTo(arrowX, height-2);
    path.lineTo(arrowX + 5, 2);
    path.lineTo(arrowX - 5, 2);
//    painter.fillPath(path, QBrush(Qt::white));
    painter.drawPath(path);
    painter.setPen(Qt::NoPen);
}



