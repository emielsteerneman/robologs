#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <iostream>

#include <opencv2/opencv.hpp>

#include "GameStateTracker.h"

GameStateTracker::GameStateTracker(){
    std::cout << "[GIT] New GameStateTracker created" << std::endl;
    reset();
}

const GameState& GameStateTracker::get() {
    return gameState;
}

void GameStateTracker::setReader(Reader *_reader) {
    reader = _reader;
}

void GameStateTracker::setHz(int _hz) {
    hz = _hz;
    std::cout << "[GST][setHz] World set to " << hz << "hz" << std::endl;
}

void GameStateTracker::reset(){
    std::cout << "[GST][reset] Resetting" << std::endl;
    gameState = {};
    gameState.isInitial = true;
    lastInterval = 0.;
    nextInterval = 0.;
}

/* Processes all packets within the next interval and accumulates data in the gameState */
bool GameStateTracker::tick() {
    if(reader == nullptr){
        std::cout << "[GST][tick] Reader not set" << std::endl;
        return false;
    }

    if(reader->isEof()){
        std::cout << "[GST][tick] EOF reached" << std::endl;
        return false;
    }

    /* If the recordingState is on skipping, process referee packages until the recordingState is on recording */
    while(!reader->isEof() && recordingState == RecordingState::SKIPPING) {
        gameState.isInitial = true;
        if (reader->next() == MESSAGE_SSL_REFBOX_2013)
                processReferee(reader->getReferee());
    }

    /* If the gameState is in its initial state, set it to the first vision packet in the log file */
    if(gameState.isInitial){
        std::cout << "[GST][tick] Searching for initial state.." << std::endl;
        int packetsNow = reader->packetsRead;
        while(!reader->isEof()){ // While there are packets
            // Read next packet
            int messageType = reader->next();
            // If the last packet was a vision packet
            if(messageType == MESSAGE_SSL_VISION_2010 || messageType == MESSAGE_SSL_VISION_2014){
                // If the last packet has vision data (not just geometry)
                if(reader->getVision().has_detection()) {
                    // If t_capture isn't 0 (first packet has that for some reason)
                    if (1 < reader->getVision().detection().t_capture()) {
                        gameState.isInitial = false;
                        lastInterval = reader->getVision().detection().t_capture();
                        break;
                    }
                }
            }
        }
        std::cout << "[GST][tick] Initial state found after " << (reader->packetsRead - packetsNow) << " packets" << std::endl;
    }

    if(reader->isEof()){
        std::cout << "[GST][tick] EOF reached" << std::endl;
        return false;
    }

    /* Process packets until the next interval is reached */
    double interval = 1.0 / hz; // interval in seconds
    double nextInterval = lastInterval + interval;
    bool intervalReached = false;


    int nPackets = 0;
    // Safeguard to prevent infinite while loops. Set to 2 * number of expected packets
    int iSafeguard = 20 * 480 / hz;

    // While there are still packed and the next interval has not been reached
    while(!reader->isEof() && !intervalReached){
        // Read next packet
        int type = reader->next();

        /* Parse Referee message */
        if(type == MESSAGE_SSL_REFBOX_2013)
            processReferee(reader->getReferee());

        /* Parse Vision or Geometry message */
        if(type == MESSAGE_SSL_VISION_2010 || type == MESSAGE_SSL_VISION_2014) {
            if (reader->getVision().has_detection())
                processVision(reader->getVision().detection());
        }

        nPackets++;

        intervalReached = nextInterval < gameState.timestamp || iSafeguard < nPackets;
        if(iSafeguard < nPackets) {
            std::cout << "[GST][tick] Safeguard overflow! Breaking out after " << nPackets << " packets" << std::endl;
            std::cout << "[GST][tick] Safeguard overflow!     Next interval : " << nextInterval << std::endl;
            std::cout << "[GST][tick] Safeguard overflow! Current timestamp : " << gameState.timestamp << std::endl;
            break;
        }
    }
//    std::cout << "[GST][tick] Packets read for next interval : " << nPackets << std::endl;
    lastInterval = nextInterval;
    return true;
}

bool GameStateTracker::processVision(const SSL_DetectionFrame& packet){
//    std::cout << "[GST][pV]" << std::endl;
//    Todo don't need both lastInterval and nextInterval

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

//    std::cout << std::setprecision(12) << std::fixed;
//    std::cout << "[GST][pV] Currently at  = " << gameState.timestamp << std::endl;
//    std::cout << "[GST][pV] Next interval = " << nextInterval << std::endl;

    // Two brackets simply for folding code
    {
    /* Yellow Team */
    // For each detected robot
    for(const SSL_DetectionRobot& packetBot : packet.robots_yellow()){

        // Check if the robot is already in the world
        bool found = false;
        for(Robot& worldBot : gameState.yellow.robots)
            if(worldBot.id == packetBot.robot_id())
                found = true;
        // If the robot is not yet in the world, add it
        if(!found){
            gameState.yellow.robots.emplace_back(Robot());
            gameState.yellow.robots.back().id = packetBot.robot_id();
        }
        // For each robot currently in the world
        for(Robot& worldBot : gameState.yellow.robots){
            if(worldBot.id == packetBot.robot_id()){    // If it is the robot in the detection packet
                worldBot.x = packetBot.x();         // set x-position
                worldBot.x_buf.put(worldBot.x);     // update circular buffer

                worldBot.y = packetBot.y();         // set y-position
                worldBot.y_buf.put(worldBot.y);     // update circular buffer

                worldBot.rot = packetBot.orientation();

                worldBot.last_seen = gameState.timestamp;
            }
        }
    }

    // Remove all yellow robots that have been out of view for more than a second
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


