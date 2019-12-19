#include <iostream>

#include "Player.h"
#include "Utilities.h"

Player::Player(GameStateTracker& _tracker, GameInfoTracker& _infoTracker, int _hz)
    : tracker(_tracker), hz(_hz), infoTracker(_infoTracker){
    std::cout << "[Player] New player created" << std::endl;
}

bool Player::reset(){

    infoTracker.reset();
    tracker.reset();

    return true;
}

void Player::start() {
    std::cout << "[Player] Starting player" << std::endl;
    // Create new timer
    timer = new QTimer(this);
    // Connect timer to tick()
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Set speed of timer
    timer->setInterval(1000/hz);
    // Start timer
    timer->start();
}

void Player::findProgress(double progress){
    timer->stop();
    std::cout << "[Player] going to progress " << progress << std::endl;
    double duration = infoTracker.get()->t_stop - infoTracker.get()->t_start;
    double timestamp = infoTracker.get()->t_start + progress * duration;
    findTimestamp(timestamp);
}

void Player::findTimestamp(double timestamp){
    std::cout << "[Player][findTimestamp] Going to timestamp " << u::timeToString(timestamp) << std::endl;

    timer->stop();
    // If timestamp is in the past, reset tracker to beginning of file
    if(timestamp < tracker.get().timestamp)
        tracker.reset();

    int _hz = tracker.getHz();
    tracker.setHz(1); // Search in steps of 1 second
    // Go through all the gameStates until the timestamp is reached
    while(tracker.tick() && tracker.get().timestamp < timestamp);
    tracker.setHz(_hz);

    timer->start();
}

void Player::tick(){
    tracker.tick();
    emit signalGameState(&tracker.get());
}