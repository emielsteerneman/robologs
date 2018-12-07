//
// Created by emiel on 4-12-18.
//

#include <stdio.h>
#include <iostream>

#include "GameStateTracker.h"

GameStateTracker::GameStateTracker(int hz) : hz(hz){
    std::cout << "[GST] Constructing new gameStateTracker" << std::endl;
}

GameStateTracker::~GameStateTracker(){
    std::cout << "[GST] Destructing gameStateTracker" << std::endl;
}

void GameStateTracker::setReader(Reader *reader) {
    this->reader = reader;
}

int GameStateTracker::tick() {
    // Read packets until the next Hz, so to say
    // Skip packets when state == SKIPPING. Only process referee packets
    //      Set isFresh = true
    //

    std::cout << std::endl << "[GST] Processing packet" << std::endl;
    // Read next packet
    int type = reader->next();

    if(type == MESSAGE_SSL_VISION_2010) {
        std::cout << "[GST] Vision packet read" << std::endl;
        if(wrapperPacket.ParseFromArray(reader->data, reader->dataHeader.messageSize)){
            if(wrapperPacket.has_detection()){
                processVision(wrapperPacket.detection());
            }
        }else{
            std::cout << "[GST] Warning! Could not parse vision packet" << std::endl;
        }
    }

    if(type == MESSAGE_SSL_REFBOX_2013){
        if (refereePacket.ParseFromArray(reader->data, reader->dataHeader.messageSize)) {
            processReferee(refereePacket);
        } else {
            std::cout << "[GST] Warning! Could not parse referee packet" << std::endl;
        }
    }

    if(type == -2){
        std::cout << "[GST] EOF reached" << std::endl;
    }

    return type;
}

void GameStateTracker::processVision(const SSL_DetectionFrame& packet){
    if(recordingState == RecordingState::SKIPPING){
        skipped++;
    }

    std::cout << "Blue robots = " << packet.robots_blue_size() << ", Yellow robots = " <<packet.robots_yellow_size() << std::endl;

    typedef google::protobuf::RepeatedPtrField<SSL_DetectionRobot> packetBots_t;
    typedef std::vector<Robot>* worldBots_t;
    typedef std::pair<packetBots_t, worldBots_t> packetWorldPair_t;

    std::vector<packetWorldPair_t> packetWorldPairs;

    packetWorldPairs.emplace_back(packet.robots_yellow(),  &gameState.yellow.robots);
    packetWorldPairs.emplace_back(packet.robots_blue(),  &gameState.blue.robots);

    packetBots_t packetBots;
    worldBots_t worldBots;

    for(auto combo : packetWorldPairs){
        std::cout << "[GST] working in pairs" << std::endl;
        std::tie(packetBots, worldBots) = combo;

        Robot* bot = nullptr;

        for(SSL_DetectionRobot packetBot : packetBots){
            std::cout << "[GST] Checking robot with id " << std::to_string(packetBot.robot_id()) << std::endl;

            // Find packetBot in worldBots
            for(Robot r : *worldBots)
                if(r.id == packetBot.robot_id())
                    bot = &r;

            // packetBot not yet in worldBots, add it
            if(bot == nullptr) {
                std::cout << "[GST] Adding new robot with id " << std::to_string(packetBot.robot_id()) << std::endl;
                worldBots->emplace_back(Robot());
                bot = &worldBots->back();
            }else{
                std::cout << "[GST]     Found robot with id " << std::to_string(packetBot.robot_id()) << std::endl;
            }

            // Update worldBot with data from packetBot
            bot->id = packetBot.robot_id();
            bot->x = packetBot.x();
            bot->y = packetBot.y();
            bot->rot = packetBot.orientation();

        }
    }

    recorded++;
}

void GameStateTracker::processReferee(const SSL_Referee& packet){
    std::cout << "[GST] Processing referee packet" << std::endl;
//    std::cout << std::to_string(packet.stage_time_left()) << std::endl;
    SSL_Referee_Stage stage = packet.stage();
    std::string stagename = SSL_Referee_Stage_Name(stage);
    std::cout << "[GST] Stagename = " << stagename << std::endl;

    if(packet.has_command()){
        SSL_Referee_Command cmd = packet.command();
//        std::string cmdName = SSL_Referee_Command_Name(cmd);
//        std::cout << "[GST] cmdName=" << cmdName << std::endl;

        if(cmd == SSL_Referee_Command_HALT
        || cmd == SSL_Referee_Command_STOP)
            recordingState =  RecordingState::SKIPPING;
        else
            recordingState = RecordingState::RECORDING;
    }

    packet.Command_descriptor();

}