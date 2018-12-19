#include <iostream>

#include "Player.h"
#include "Utilities.h"

Player::Player() : QObject(){
    std::cout << "[Player] New player created" << std::endl;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->setInterval(1000/60);

    reader.openFile("../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log");

    tracker.setHz(60);
}

void Player::start() {
    getInfo();
    std::cout << "[Player] Starting player" << std::endl;
//    timer->start();
}

void Player::getInfo(){
    std::cout << "[Player] Analyzing file" << std::endl;
    reader.reset();

    while(!reader.isEof()){
        int type = reader.next();
        infoTracker.process(reader.getDataHeader());

        if (type == MessageType::MESSAGE_SSL_VISION_2010)
            infoTracker.process(reader.getVision());
        if (type == MessageType::MESSAGE_SSL_REFBOX_2013)
            infoTracker.process(reader.getReferee());
    }
    std::cout << "[Player] File analyzed" << std::endl;
    double duration = infoTracker.get().t_stop - infoTracker.get().t_start;
    std::cout << u::durationToString(duration) << std::endl;
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
