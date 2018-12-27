#ifndef ROBOLOGS_INTERFACE_H
#define ROBOLOGS_INTERFACE_H


#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QLabel>

#include "Field.h"
#include "Timeline.h"
#include "game/GameInfoTracker.h"
#include "game/GameStateTracker.h"

class Interface : public QWidget {
Q_OBJECT
public:
    explicit Interface(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event) override;

signals:
    void signalProgress(double progress);

public slots:
    void updateGameState(const GameState* gameState);
    void setGameInfo(const GameInfo* gameInfo);

private:
    const GameInfo* gameInfo = nullptr;

    Field *field;
    Timeline *timeline;

    QLabel* lGameCommand;
    QLabel* lTeamYellow;
    QLabel* lTeamBlue;

    QLabel* lStats;


};

#endif // ROBOLOGS_INTERFACE_H
