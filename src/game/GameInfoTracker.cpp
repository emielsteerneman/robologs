//
// Created by emiel on 19-12-18.
//

#include <iostream>
#include "GameInfoTracker.h"
#include <vector>
#include <tuple>

GameInfoTracker::GameInfoTracker() {
    std::cout << "[GIT] New GameInfoTracker created" << std::endl;
    reset();
}

void GameInfoTracker::reset() {
    std::cout << "[GIT][reset] Resetting" << std::endl;
    gameInfo = {};
    isInitial = true;
    lastCommand = "";
    lastState = "";
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

    const SSL_DetectionFrame& frame = wrapperPacket.detection();

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






//
//
//void GameStateTracker::addInfo(){
//
//    if(!reader->isOpen()) {
//        std::cout << "[GST][addInfo] Warning! Reader not ready!" << std::endl;
//        return;
//    }
//
//    /* Add timeline */
//    std::string stage;
//    std::string cmd;
//
//    // reset everything
//    reader->reset();
//    //Todo reset struct
//
//    while(!reader->isEof()){
//        int type = reader->next();
//        gameState.gameInfo.nPackets++;
//
//        // Referee packet
//        if(type == MESSAGE_SSL_REFBOX_2013) {
//            if (refereePacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize)) { // Parse packet
//                // Process referee packet
//                gameState.gameInfo.referee_nPackets++;
//                processReferee(refereePacket);
//                // Add stage/command change to timeline
//                if (stage != gameState.stage || cmd != gameState.command) {
//                    stage = gameState.stage;
//                    cmd = gameState.command;
//                    double timestamp = reader->getDataHeader().timestamp / 1000000000.0;
//                    gameState.gameInfo.timeline.emplace_back(timestamp, stage, cmd, recordingState);
//                }
//            }else{
//                gameState.gameInfo.invalid_nPackets++; // Parse failed
//            }
//        }else
//
//            // Vision packet
//        if(type == MESSAGE_SSL_VISION_2010){
//            if (wrapperPacket.ParseFromArray(reader->getData(), reader->getDataHeader().messageSize)) {
//                if(wrapperPacket.has_detection())
//                    gameState.gameInfo.vision_nPackets++;
//                if(wrapperPacket.has_geometry())
//                    gameState.gameInfo.geometry_nPackets++;
//            }else{
//                gameState.gameInfo.invalid_nPackets++; // Parse failed
//            }
//        }else
//
//            // Weird packet
//        if(type == MESSAGE_UNKNOWN || type == MESSAGE_BLANK){
//            gameState.gameInfo.invalid_nPackets++; // Parse failed
//        }
//    }
//
//    double timestamp = reader->getDataHeader().timestamp / 1000000000.0;
//    gameState.gameInfo.timeline.emplace_back(timestamp, "END", "END", RecordingState::SKIPPING);
//
////    std::cout << std::setprecision(15) << std::endl;
////    for(std::tuple<double, std::string, std::string> myTuple : gameState.timeline)
////        std::cout << std::get<0>(myTuple) << "::" << std::get<1>(myTuple) << "::"  << std::get<2>(myTuple) << std::endl;
//
//    double t_start = std::get<0>(gameState.gameInfo.timeline.front());
//    double t_end = std::get<0>(gameState.gameInfo.timeline.back());
//    double t_duration = t_end - t_start;
//
//    gameState.gameInfo.t_start = t_start;
//    gameState.gameInfo.t_stop = t_end;
//    gameState.gameInfo.t_duration = t_duration;
//
//    gameState.gameInfo.pps          = gameState.gameInfo.nPackets / t_duration;
//    gameState.gameInfo.referee_pps  = gameState.gameInfo.referee_nPackets  / t_duration;
//    gameState.gameInfo.vision_pps   = gameState.gameInfo.vision_nPackets   / t_duration;
//    gameState.gameInfo.geometry_pps = gameState.gameInfo.geometry_nPackets / t_duration;
//    gameState.gameInfo.invalid_pps  = gameState.gameInfo.invalid_nPackets  / t_duration;
//
//    reader->reset();
//}