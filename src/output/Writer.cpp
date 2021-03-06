//
// Created by emiel on 13-12-18.
//

#include "Writer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>

Writer::Writer(std::string filename) : filename(filename){
    std::cout << "[Writer] Created Writer. filename=" << filename << std::endl;

    out = std::ofstream(filename, std::ios_base::out);

    out << "[\n";
}

Writer::~Writer() {
    out.flush();
    out.close();
}

void Writer::write(const GameState* gameState){
    nFramesTotal++;

//    if(gameState->yellow.robots.size() != 8 || gameState->blue.robots.size() != 8) {
//        std::cout << "[Writer] Invalid amount of robots : yellow=" << gameState->yellow.robots.size() << ", blue=" << gameState->blue.robots.size() << std::endl;
//        return;
//    }

    if(gameState->stage == "NORMAL_FIRST_HALF") {
        if(gameState->command == "NORMAL_START"
        || gameState->command == "DIRECT_FREE_YELLOW"
        || gameState->command == "INDIRECT_FREE_YELLOW"
        || gameState->command == "DIRECT_FREE_BLUE"
        || gameState->command == "INDIRECT_FREE_BLUE"
        ) {

            if(inBreak){
                std::cout << "[Writer][write] Switched to " << gameState->stage << " | " << gameState->command << ", opening block." << std::endl;
                if(!isFirstBlock)
                    out << ", \n\n";
                out << "[";
                inBreak = false;
            }

            out << gameStateToJson(gameState);
            nFrames++;

            if (nFrames % 100 == 0)
                std::cout << "[Writer] Frame " << nFrames << "/" << nFramesTotal << " written" << std::endl;
        }else{
            if(!inBreak) {
                std::cout << "[Writer][write] Switched to " << gameState->stage << " | " << gameState->command << ", closing block." << std::endl;
                // Close block, start new block
                out << "]";
                isFirstBlock = false;
                isFirstGamestate = true;
                inBreak = true;
            }
        }
    }
}

void Writer::endJSON() {
    out << "]" << std::endl;
    out.close();
}


std::string Writer::gameStateToJson(const GameState* gameState){
    std::stringstream out;

    float scaleX = 6000;
    float scaleY = 4500;

    out << std::setprecision(10) << std::fixed;
    if(!isFirstGamestate)
        out << ",";
    isFirstGamestate = false;

    out << "{";
    out << " \"timestamp\" :" << gameState->timestamp;
    out << ", \"stage\" :\"" << gameState->stage << "\"";
    out << ", \"command\" :\"" << gameState->command << "\"";

    out << ", \"robots_yellow\" : [";
    for(int i = 0; i < gameState->yellow.robots.size(); i++){
        const Robot& robot = gameState->yellow.robots.at(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.id;
        out << ",\"x\" : "        << robot.x / scaleX;
        out << ",\"x_vel\" : "    << robot.x_buf.avgVel() / scaleX;
        out << ",\"x_orien\" : "    << std::cos(robot.rot);
        out << ",\"y\" : "        << robot.y / scaleY;
        out << ",\"y_vel\" : "    << robot.y_buf.avgVel() / scaleY;
        out << ",\"y_orien\" : "    << std::sin(robot.rot);
//        out << ",\"orientation\" : " << robot.rot;
        out << "}";
    }
    out << "]";

    out << ", \"robots_blue\" : [";
    for(int i = 0; i < gameState->blue.robots.size(); i++){
        const Robot& robot = gameState->blue.robots.at(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.id;
        out << ",\"x\" : "        << robot.x / scaleX;
        out << ",\"x_vel\" : "    << robot.x_buf.avgVel() / scaleX;
        out << ",\"x_orien\" : "    << std::cos(robot.rot);
        out << ",\"y\" : "        << robot.y / scaleY;
        out << ",\"y_vel\" : "    << robot.y_buf.avgVel() / scaleY;
        out << ",\"y_orien\" : "    << std::sin(robot.rot);
//        out << ",\"orientation\" : " << robot.rot;
        out << "}";
    }
    out << "]";

    out << ", \"balls\" : [";
    const Ball& ball = gameState->ball;
    out << "{";
    out << " \"x\" : " << ball.x / scaleX;
    out << ",\"x_vel\" : " << ball.x_buf.avgVel();
    out << ",\"y\" : " << ball.y / scaleY;
    out << ",\"y_vel\" : " << ball.y_buf.avgVel();
    out << "}";
    out << "]";

    out << "}";
    out << std::endl;

    return out.str();
}
