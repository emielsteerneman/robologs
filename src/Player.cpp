#include <iostream>

#include "Player.h"

Player::Player() : QObject(){
    std::cout << "[Player] New player created" << std::endl;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->setInterval(1000);

}

void Player::start() {
    std::cout << "[Player] Starting player" << std::endl;
    timer->start();
}

void Player::tick() {
    std::cout << "[Player] Tick" << std::endl;
    timerCount++;
    emit nextGameState(timerCount);
}
