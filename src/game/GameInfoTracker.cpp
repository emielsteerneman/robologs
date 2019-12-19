//
// Created by emiel on 19-12-18.
//

#include <iostream>
#include "GameInfoTracker.h"
#include <vector>
#include <tuple>

GameInfoTracker::GameInfoTracker(Reader& _reader) : reader(_reader){
    std::cout << "[GIT] New GameInfoTracker created" << std::endl;
    reset();
}

void GameInfoTracker::reset() {
    std::cout << "[GIT][reset] Resetting" << std::endl;
    gameInfo = {};
    isInitial = true;
}

bool GameInfoTracker::initTimeline(){
    reset();

    if(!reader.reset()){
        std::cout << "[GIT][initTimeline] Could not reset reader" << std::endl;
        return false;
    }

    while(!reader.isEof()){
        int type = reader.next();
        /* Pass Dataheader */
        process(reader.getDataHeader());
        /* Parse Referee message */
        if(type == MESSAGE_SSL_REFBOX_2013)
            process(reader.getReferee());
        /* Parse Vision or Geometry message */
        if(type == MESSAGE_SSL_VISION_2010 || type == MESSAGE_SSL_VISION_2014)
            process(reader.getVision());
    }

    if(!reader.reset()){
        std::cout << "[GIT][initTimeline] Could not reset reader" << std::endl;
        return false;
    }

    std::cout << "[GIT][initTimeline] Timeline created" << std::endl;
    return true;
}

const GameInfo* GameInfoTracker::get(){
    return &gameInfo;
}

void GameInfoTracker::process(const DataHeader& dataHeader){
    if(isInitial){
        gameInfo.t_start = dataHeader.timestamp / 1000000000;
        isInitial = false;
    }

    gameInfo.t_stop = dataHeader.timestamp / 1000000000;
}

void GameInfoTracker::process(const SSL_WrapperPacket& wrapperPacket){
    if(!wrapperPacket.has_detection())
        return;

    gameInfo.vision_nPackets++;

}

void GameInfoTracker::process(const SSL_Referee& refereePacket){
    gameInfo.referee_nPackets++;

    std::string stage = SSL_Referee_Stage_Name(refereePacket.stage());
    std::string command = SSL_Referee_Command_Name(refereePacket.command());

    if(gameInfo.timeline.empty()){
        gameInfo.timeline.emplace_back(gameInfo.t_stop, stage, command);
    }else{
        if(stage != getStage() || command != getCommand()){
//            std::cout << "[GIT] Change detected. "
//            << stage << "=>" << getStage() << " , "
//            << command << "=>" << getCommand() << std::endl;
            gameInfo.timeline.emplace_back(gameInfo.t_stop, stage, command);
        }
    }
}

std::string GameInfoTracker::getStage() {
    if(gameInfo.timeline.empty())
        return "No stage";
    else
        return std::get<1>(gameInfo.timeline.back());
}

std::string GameInfoTracker::getCommand() {
    if(gameInfo.timeline.empty())
        return "No command";
    else
        return std::get<2>(gameInfo.timeline.back());
}