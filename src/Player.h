#ifndef ROBOLOGS_PLAYER_H
#define ROBOLOGS_PLAYER_H

#include <QObject>
#include <QTimer>

class Player : public QObject {
Q_OBJECT
public:
    explicit Player();
    void start();

signals:
    void nextGameState(int state);

public slots:
    void tick();
private:
    QTimer *timer;
    int timerCount = 0;
};


#endif //ROBOLOGS_PLAYER_H
