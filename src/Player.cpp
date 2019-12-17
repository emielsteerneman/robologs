#include <iostream>

#include "Player.h"
#include "Utilities.h"

Player::Player(const std::string& filename) : filename(filename) {
    std::cout << "[Player] New player created for file " << filename << std::endl;
    reader.openFile(filename);
}

bool Player::reset(){
    // Reset the reader
    if(!reader.openFile(filename)){
        std::cout << "[Player][reset] Error while resetting reader" << std::endl;
        return false;
    }

    infoTracker.reset();
    tracker.reset();

    return true;
}

void Player::start() {
    std::cout << "[Player] Starting player" << std::endl;

//    reader.openFile("../../2018-06-20_21-21_TIGERs_Mannheim-vs-CMμs.log");
    reader.openFile(filename);

    getInfo();

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
//    timer->setInterval(1000/60);
//    timer->setInterval(1);
//    tracker.setHz(30);
//    timer->start();
}

bool Player::getInfo(){
    std::cout << "[Player] Creating Game Info" << std::endl;

    if(!reader.isOpen()) {
        if (!reader.openFile(filename)) {
            std::cout << "[Player][getInfo] Reader did not open file" << std::endl;
            return false;
        }
    }

    reader.reset();

    while(!reader.isEof()){
        int type = reader.next();

        infoTracker.process(reader.getDataHeader());

        if (type == MessageType::MESSAGE_SSL_VISION_2010) {
            infoTracker.process(reader.getVision());
//            std::cout << "[Player] MESSAGE_SSL_VISION_2010 detected" << std::endl;
        }
        else if (type == MessageType::MESSAGE_SSL_REFBOX_2013) {
            infoTracker.process(reader.getReferee());
//            std::cout << "[Player] MESSAGE_SSL_REFBOX_2013 detected" << std::endl;
        }
        else if (type == MessageType::MESSAGE_SSL_VISION_2014) {
            infoTracker.process(reader.getVision());
//            std::cout << "[Player] MESSAGE_SSL_VISION_2014 detected" << std::endl;
        }
        else if(type == MessageType::MESSAGE_INVALID)
            std::cout << "[Player] MESSAGE_INVALID detected" << std::endl;
        else if(type == MessageType::MESSAGE_UNKNOWN)
            std::cout << "[Player] MESSAGE_UNKNOWN detected" << std::endl;
        else if(type == MessageType::MESSAGE_BLANK)
            std::cout << "[Player] MESSAGE_BLANK detected" << std::endl;
        else
            std::cout << "[Player] Unknown message type detected" << std::endl;

    }
    std::cout << "[Player] Game Info created" << std::endl;
    reader.reset();

    return true;
//    emit signalGameInfo(infoTracker.get());
}

void Player::findProgress(double progress){
    timer->stop();
    std::cout << "[Player] going to progress " << progress << std::endl;
    double duration = infoTracker.get()->t_stop - infoTracker.get()->t_start;
    double timestamp = infoTracker.get()->t_start + progress * duration;
    findTimestamp(timestamp);
}

void Player::findTimestamp(double timestamp){
    std::cout << "[Player] going to timestamp " << u::timeToString(timestamp) << std::endl;
//    std::cout << " at " << u::timeToString(reader.getDataHeader().timestamp / 1000000000) << std::endl;
    timer->stop();
    reader.reset();


    std::cout << "[Player] searching.. " << std::endl;
    while(!reader.isEof() && (reader.getDataHeader().timestamp / 1000000000) < timestamp)
        reader.next();

    std::cout << "[Player] found" << std::endl;

    timer->start();
}

void Player::tick(){
    // Holds whether the next game state has been reached. Depends on the Hz of the tracker.
    bool nextStateReached = false;

    while(!reader.isEof() && !nextStateReached) {
        // Let the reader read the next package, and get the type
        int type = reader.next();

        if (type == MessageType::MESSAGE_SSL_VISION_2010 || type == MessageType::MESSAGE_SSL_VISION_2014) {
            const SSL_WrapperPacket packet = reader.getVision();
            if (packet.has_detection()) {
                if (tracker.processVision(packet.detection())) {
//                    std::cout << " at " << u::timeToString(reader.getDataHeader().timestamp / 1000000000) << std::endl;
//                    emit signalGameState(tracker.get());
                    nextStateReached = true;
                }
            }
        } else if (type == MessageType::MESSAGE_SSL_REFBOX_2013)
            tracker.processReferee(reader.getReferee());
    }

    if(reader.isEof()) {
        std::cout << "[Player] End of file reached!" << std::endl;
//        timer->stop();
    }
}
