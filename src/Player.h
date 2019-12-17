#ifndef ROBOLOGS_PLAYER_H
#define ROBOLOGS_PLAYER_H

#include <QObject>
#include <QTimer>

#include "input/Reader.h"
#include "game/GameInfoTracker.h"
#include "game/GameStateTracker.h"

class Player {
public:
    explicit Player(const std::string& filename);

signals:
    void signalGameState(const GameState* gameState);
    void signalGameInfo(const GameInfo* gameInfo);

public:
    void start();
    void tick();
    void findProgress(double progress);
    void findTimestamp(double timestamp);
public:
    bool reset();
    bool getInfo();
    std::string filename;
    QTimer *timer;
    Reader reader;
    GameInfoTracker infoTracker;
    GameStateTracker tracker;
};


#endif //ROBOLOGS_PLAYER_H
