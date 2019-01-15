#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "GameStateTracker.h"

GameStateTracker::GameStateTracker() {
    reset();
}

const GameState* GameStateTracker::get() {
    return &gameState;
}

void GameStateTracker::setInput(Reader *reader) {
    this->reader = reader;
}

void GameStateTracker::setHz(int hz) {
    this->hz = hz;
}

void GameStateTracker::reset(){
    gameState = {};
    gameState.isInitial = true;
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

bool GameStateTracker::processVision(const SSL_DetectionFrame& packet){
    // Todo don't need both lastInterval and nextInterval

    // Sanity check on Hz. Should be non-negative
    if(hz < 1){
        std::cout << "[GST][pV] Warning! Incorrect Hz. Hz = " << hz << std::endl;
        return false;
    }

    parsed++;
    gameState.timestamp = packet.t_capture();

    // Set next interval if state is initial
    if(gameState.isInitial){
        lastInterval = gameState.timestamp;
        nextInterval = lastInterval + 1.0 / hz;
        gameState.isInitial = false;
    }

    std::cout << std::setprecision(12) << std::fixed;
//    std::cout << "[GST][pV] Currently at  = " << gameState.timestamp << std::endl;
//    std::cout << "[GST][pV] Next interval = " << nextInterval << std::endl;

    // Two brackets simply for folding code
    {
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

                worldBot.rot = packetBot.orientation();

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

                worldBot.rot = packetBot.orientation();

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
    }

    // Check if interval has been reached
    if(nextInterval <= gameState.timestamp){
//        std::cout << "[GST][pV] Interval reached" << std::endl;
        lastInterval = nextInterval;
        nextInterval = lastInterval + 1.0 / hz;
        return true;
    }

    return false;
}

void GameStateTracker::processReferee(const SSL_Referee& packet){
    parsed++;

    gameState.stage = SSL_Referee_Stage_Name(packet.stage());

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


