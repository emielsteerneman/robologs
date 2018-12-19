#ifndef ROBOLOGS_PLAYER_H
#define ROBOLOGS_PLAYER_H

#include <QObject>
#include <QTimer>

#include "input/Reader.h"
#include "game/GameInfoTracker.h"
#include "game/GameStateTracker.h"

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
    Reader reader;
    GameInfoTracker infoTracker;
    GameStateTracker tracker;

    void getInfo();
};


#endif //ROBOLOGS_PLAYER_H
