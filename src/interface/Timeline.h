//
// Created by emiel on 20-12-18.
//

#ifndef ROBOLOGS_TIMELINE_H
#define ROBOLOGS_TIMELINE_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>

#include "../game/GameInfoTracker.h"

class TimelineBackground : public QWidget {
Q_OBJECT
public:
    const GameInfo* gameInfo = nullptr;
    explicit TimelineBackground(QWidget* parent);
    void paintEvent(QPaintEvent *event) override;
    QColor getStageColor(std::string stage);
    QColor getCommandColor(std::string command);
};



class TimelineProgress : public QWidget {
Q_OBJECT
public:
    double progress = -1;
    explicit TimelineProgress(QWidget* parent);
    void paintEvent(QPaintEvent *event) override;
};



class Timeline : public QWidget {
Q_OBJECT
private:
    const GameInfo* gameInfo = nullptr;

    TimelineBackground* wBackground;
    TimelineProgress *wProgress;

    QLabel* lTimeStart;
    QLabel* lTimeStop;
    QLabel* lProgress;

public:
    explicit Timeline(QWidget* parent);
    void setGameInfo(const GameInfo* gameInfo);
    void setProgress(double progress);
    void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void signalProgress(double progress);

};

#endif //ROBOLOGS_TIMELINE_H
