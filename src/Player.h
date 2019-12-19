#ifndef ROBOLOGS_PLAYER_H
#define ROBOLOGS_PLAYER_H

#include <QObject>
#include <QTimer>

#include "input/Reader.h"
#include "game/GameInfoTracker.h"
#include "game/GameStateTracker.h"

class Player : public QObject {
Q_OBJECT

private:
    GameStateTracker tracker;
    GameInfoTracker infoTracker;
    int hz;

public:
    Player(GameStateTracker& tracker, GameInfoTracker& infoTracker, int hz);
    bool reset();
    bool getInfo();
    std::string filename;
    QTimer *timer;

public slots:
    void start();
    void tick();
    void findProgress(double progress);
    void findTimestamp(double timestamp);

signals:
    void signalGameState(const GameState* gameState);
    void signalGameInfo(const GameInfo* gameInfo);


};


#endif //ROBOLOGS_PLAYER_H
