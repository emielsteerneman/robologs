//
// Created by emiel on 4-12-18.
//

#include <stdio.h>
#include <iostream>

#include "GameStateTracker.h"

GameStateTracker::GameStateTracker(){
    std::cout << "[GST] Constructing new gameStateTracker" << std::endl;
}

GameStateTracker::~GameStateTracker(){
    std::cout << "[GST] Destructing gameStateTracker" << std::endl;
}

void GameStateTracker::setReader(Reader *reader) {
    this->reader = reader;
}

int GameStateTracker::tick() {
    std::cout << std::endl << "[GST] Processing packet" << std::endl;
    // Read next packet
    int type = reader->next();

    if(type == MESSAGE_SSL_VISION_2010) {
        std::cout << "[GST] Vision packet read" << std::endl;
        if(wrapperPacket.ParseFromArray(reader->data, reader->dataHeader.messageSize)){
            processVision(wrapperPacket.detection());
        }
    }

    if(type == MESSAGE_SSL_REFBOX_2013){
        if (refereePacket.ParseFromArray(reader->data, reader->dataHeader.messageSize)) {
            processReferee(refereePacket);
        } else {
            std::cerr << "Error parsing vision packet!" << std::endl;
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


    std::vector<std::tuple<google::protobuf::RepeatedPtrField<SSL_DetectionRobot>&, std::vector<Robot>&>> SSL_botToGamestateBot;
//    SSL_botToGamestateBot.emplace_back(packet.robots_yellow(),  gameState.yellow.robots);
//    SSL_botToGamestateBot.emplace_back(packet.robots_blue(),  gameState.blue.robots);

    // Update all robots of yellow team
    for(SSL_DetectionRobot SSL_bot : packet.robots_yellow()){

        std::vector<Robot>& bots = gameState.yellow.robots;
        Robot* bot = nullptr;

        for(Robot r : bots)
            if(r.id == SSL_bot.robot_id())
                bot = &r;

        if(bot == nullptr) {
            std::cout << "[GST] Adding new robot with id " << std::to_string(SSL_bot.robot_id()) << std::endl;
            bots.emplace_back(Robot());
            bot = &bots.back();
        }else{
            std::cout << "[GST] Found robot with id " << std::to_string(SSL_bot.robot_id()) << std::endl;
        }

        bot->id = SSL_bot.robot_id();
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