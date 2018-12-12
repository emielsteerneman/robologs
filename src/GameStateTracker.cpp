//
// Created by emiel on 4-12-18.
//

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "GameStateTracker.h"

GameStateTracker::GameStateTracker(int hz) : hz(hz){
    std::cout << "[GST] Constructing new gameStateTracker" << std::endl;
    gameState.isInitial = true;
}

GameStateTracker::~GameStateTracker(){
    std::cout << "[GST] Destructing gameStateTracker" << std::endl;
}

void GameStateTracker::setReader(Reader *reader) {
    this->reader = reader;
}

/* Processes all packets within the next interval and accumulates data in the gameState */
bool GameStateTracker::tick() {
    if(reader->isEof())
        return false;

    /* If the recordingState is on skipping, process referee packages until the recordingState is on recording */
    while(!reader->isEof() && recordingState == RecordingState::SKIPPING) {
        gameState.isInitial = true;
        if (reader->next() == MESSAGE_SSL_REFBOX_2013)
            if (refereePacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize))
                processReferee(refereePacket);
    }

    /* If the gameState is in it initial state, set it to the first vision packet */
    if(gameState.isInitial){
        while(!reader->isEof()){ // While there are packets
            if(reader->next() == MESSAGE_SSL_VISION_2010    // If wrapper packet
            && wrapperPacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize) // If parsed successfully
            && wrapperPacket.has_detection()                // If it has vision data
            && 1 < wrapperPacket.detection().t_capture()){ // If t_capture isn't 0 (first packet has that for some reason)
                gameState.isInitial = false;
                lastInterval = wrapperPacket.detection().t_capture();
//                std::cout << "[GST][tick][initial] Initial timestamp set to " << std::setprecision(12) << lastInterval << std::endl;
                break;
            }
        }
    }

    if(reader->isEof()){
        std::cout << "[GST][tick][initial] EOF reached" << std::endl;
        return false;
    }

    /* Process packets until the next interval is reached */
    double interval = 1.0 / hz; // interval in seconds
    double nextInterval = lastInterval + interval;
    bool intervalReached = false;

    int iParsed = 0;
    int iSafeguard = 2 * 480 / hz; // Set safeguard to 2 * number of expected packets

    while(!reader->isEof() && !intervalReached){

        int type = reader->next(); // Read next packet

        /* Parse Referee message */
        if(type == MESSAGE_SSL_REFBOX_2013){
            if (refereePacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize)) {
                processReferee(refereePacket);
            } else {
                std::cout << "[GST] Warning! Could not parse referee packet" << std::endl;
            }
        }

        /* Parse Vision or Geometry message */
        if(type == MESSAGE_SSL_VISION_2010) {
            if(wrapperPacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize)){
                if(wrapperPacket.has_detection()){
                    processVision(wrapperPacket.detection());
                }
            }else{
                std::cout << "[GST][tick] Warning! Could not parse vision packet" << std::endl;
            }
        }

        intervalReached = nextInterval < gameState.timestamp || iSafeguard < iParsed;
        if(iSafeguard < iParsed)
            std::cout << "[GST][tick] Warning! Safeguard overflow!" << std::endl;

    }

    lastInterval = nextInterval;
    return true;
}

void GameStateTracker::processVision(const SSL_DetectionFrame& packet){
    parsed++;
    gameState.timestamp = packet.t_capture();

    /* Yellow Team */
    for(const SSL_DetectionRobot& packetBot : packet.robots_yellow()){
        bool found = false;
        for(Robot& worldBot : gameState.yellow.robots)
            if(worldBot.id == packetBot.robot_id())
                found = true;

        if(!found){
            gameState.yellow.robots.emplace_back(Robot());
            gameState.yellow.robots.back().id = packetBot.robot_id();
        }

        for(Robot& worldBot : gameState.yellow.robots){
            if(worldBot.id == packetBot.robot_id()){
                worldBot.x = packetBot.x();
                worldBot.x_buf.put(worldBot.x);

                worldBot.y = packetBot.y();
                worldBot.y_buf.put(worldBot.y);

                worldBot.last_seen = gameState.timestamp;
            }
        }
    }

    for (auto it = gameState.yellow.robots.begin(); it != gameState.yellow.robots.end(); /* NOTHING */){
        if (1 < gameState.timestamp - (*it).last_seen) {
            it = gameState.yellow.robots.erase(it);
        }else
            ++it;
    }

    /* Blue Team */
    for(const SSL_DetectionRobot& packetBot : packet.robots_blue()){
        bool found = false;
        for(Robot& worldBot : gameState.blue.robots)
            if(worldBot.id == packetBot.robot_id())
                found = true;

        if(!found){
            gameState.blue.robots.emplace_back(Robot());
            gameState.blue.robots.back().id = packetBot.robot_id();
        }

        for(Robot& worldBot : gameState.blue.robots){
            if(worldBot.id == packetBot.robot_id()){
                worldBot.x = packetBot.x();
                worldBot.x_buf.put(worldBot.x);

                worldBot.y = packetBot.y();
                worldBot.y_buf.put(worldBot.y);

                worldBot.last_seen = gameState.timestamp;
            }
        }
    }

    for (auto it = gameState.blue.robots.begin(); it != gameState.blue.robots.end(); /* NOTHING */){
        if (1 < gameState.timestamp - (*it).last_seen) {
            it = gameState.blue.robots.erase(it);
        }else
            ++it;
    }

    /* Ball */
    if(0 < packet.balls_size()){
        gameState.ball.x = packet.balls(0).x();
        gameState.ball.x_buf.put(gameState.ball.x);

        gameState.ball.y = packet.balls(0).y();
        gameState.ball.y_buf.put(gameState.ball.y);
    }

    return;


//    typedef google::protobuf::RepeatedPtrField<SSL_DetectionRobot> packetBots_t;
//    typedef std::vector<Robot>* worldBots_t;
//    typedef std::pair<packetBots_t, worldBots_t> packetWorldPair_t;
//
//    std::vector<packetWorldPair_t> packetWorldPairs;
//
//    packetWorldPairs.emplace_back(packet.robots_yellow(),  &gameState.yellow.robots);
//    packetWorldPairs.emplace_back(packet.robots_blue(),    &gameState.blue.robots);
//
//    packetBots_t packetBots;
//    worldBots_t worldBots;
//
//    std::cout << std::endl << std::endl;
//    for(Robot r : gameState.yellow.robots)
//        std::cout << "\tid=" << r.id << " x=" << r.x << std::endl;
//
//    for(auto combo : packetWorldPairs){
//        std::tie(packetBots, worldBots) = combo;
//
//        Robot* bot = nullptr;
//
//        for(SSL_DetectionRobot packetBot : packetBots){
//
//            // Find packetBot in worldBots
//            for(Robot r : *worldBots)
//                if(r.id == packetBot.robot_id())
//                    bot = &r;
//
//            // packetBot not yet in worldBots, add it
//            if(bot == nullptr) {
//                worldBots->emplace_back(Robot());
//                bot = &worldBots->back();
//            }
//
//            // Update worldBot with data from packetBot
//            bot->id = packetBot.robot_id();
//            bot->last_seen = gameState.timestamp;
//
//            float before = bot->x;
//            bot->x = packetBot.x();
//            float after = bot->x;
//
//            std::cout << std::setprecision(12) << "id=" << bot->id << " \tbefore=" << before << " \tpacket=" << packetBot.x() << " \tafter=" << after << std::endl;
//
//            bot->y = packetBot.y();
//            bot->rot = packetBot.orientation();
//
//        }
//
//        // Delete old robots
//        for (auto it = worldBots->begin(); it != worldBots->end(); /* NOTHING */){
//            if (1000 < gameState.timestamp - (*it).last_seen) {
//                std::cout << "[GST][pV] removing old bot with id=" << (*it).id << std::endl;
//                std::cout << gameState.timestamp << " " << (*it).last_seen << " " << (gameState.timestamp - (*it).last_seen) << std::endl;
//                it = worldBots->erase(it);
//            }else
//                ++it;
//        }
//    }
//
//    for(Robot r : gameState.yellow.robots)
//        std::cout << "\tid=" << r.id << " x=" << r.x << std::endl;
//
//    recorded++;

}

void GameStateTracker::processReferee(const SSL_Referee& packet){
    parsed++;
    SSL_Referee_Stage stage = packet.stage();
    gameState.stage = SSL_Referee_Stage_Name(stage);

    if(packet.has_command()){
        SSL_Referee_Command cmd = packet.command();
        gameState.command = SSL_Referee_Command_Name(cmd);

        if(cmd == SSL_Referee_Command_HALT)
            recordingState =  RecordingState::SKIPPING;
        else
            recordingState = RecordingState::RECORDING;
    }

    gameState.yellow.name = packet.yellow().name();
    gameState.blue.name = packet.blue().name();
}

void GameStateTracker::addTimeline(){

    std::string stage;
    std::string cmd;

    gameState.timeline.clear();
    while(!reader->isEof()){
        int type = reader->next();
        if(type == MESSAGE_SSL_REFBOX_2013) {
            if (refereePacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize)) {
                processReferee(refereePacket);
                if (stage != gameState.stage || cmd != gameState.command) {
                    stage = gameState.stage;
                    cmd = gameState.command;
                    double timestamp = reader->getDataHeader().timestamp / 1000000000.0;
                    gameState.timeline.emplace_back(timestamp, stage, cmd, recordingState);
                }
            }
        }
    }

    double timestamp = reader->getDataHeader().timestamp / 1000000000.0;
    gameState.timeline.emplace_back(timestamp, "END", "END", RecordingState::SKIPPING);

//    std::cout << std::setprecision(15) << std::endl;
//    for(std::tuple<double, std::string, std::string> myTuple : gameState.timeline)
//        std::cout << std::get<0>(myTuple) << "::" << std::get<1>(myTuple) << "::"  << std::get<2>(myTuple) << std::endl;

    reader->reset();
}