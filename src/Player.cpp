#include <iostream>

#include "Player.h"

Player::Player() : QObject(){
    std::cout << "[Player] New player created" << std::endl;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->setInterval(1000/60);

    reader.openFile("../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log");

    tracker.setHz(60);
}

void Player::start() {
    std::cout << "[Player] Starting player" << std::endl;
    timer->start();
}

void Player::tick() {
    std::cout << "[Player] Tick" << std::endl;

    bool nextStateReached = false;

    while(!reader.isEof() && !nextStateReached) {
        int type = reader.next();

        if (type == MessageType::MESSAGE_SSL_VISION_2010) {
            const SSL_WrapperPacket packet = reader.getVision();
            if (packet.has_detection())
                if (tracker.processVision(packet.detection())) {
                    std::cout << "[Player] Gamestate has reached new gameState!" << std::endl;
                    emit nextGameState(tracker.get());
                    nextStateReached = true;
                }
        } else if (type == MessageType::MESSAGE_SSL_REFBOX_2013)
            tracker.processReferee(reader.getReferee());
    }

    std::cout << std::endl;
}
