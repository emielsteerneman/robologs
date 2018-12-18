#ifndef ROBOLOGS_PLAYER_H
#define ROBOLOGS_PLAYER_H

#include <QObject>
#include <QTimer>

#include "Reader.h"
#include "GameStateTracker.h"

class Player : public QObject {
Q_OBJECT
public:
    explicit Player();
    void start();

signals:
    void nextGameState(const GameState& gameState);

public slots:
    void tick();

private:
    QTimer *timer;
    int timerCount = 0;

    Reader reader;
    GameStateTracker tracker;
};


#endif //ROBOLOGS_PLAYER_H
