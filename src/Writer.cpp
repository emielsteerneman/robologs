//
// Created by emiel on 13-12-18.
//

#include "Writer.h"
#include <iostream>
#include <iomanip>

Writer::Writer(std::string filename) : filename(filename){
    std::cout << "Created Writer. filename=" << filename << std::endl;

    out = std::ofstream(filename, std::ios_base::out);

    out << "[\n";
}

Writer::~Writer(){
    out << "]\n";
}

void Writer::write(const GameState& gameState){

    if(gameState.yellow.robots.size() != 8 || gameState.blue.robots.size() != 8)
        return;

    out << gameStateToJson(gameState);



}

std::string Writer::gameStateToJson(const GameState& gameState){
    std::stringstream out;

    float scaleX = 6000;
    float scaleY = 4500;

    out << std::setprecision(10) << std::fixed;
    if(!isFirst)
        out << ",";
    isFirst = false;

    out << "{";
    out << " \"timestamp\" :" << gameState.timestamp;
    out << ", \"stage\" :\"" << gameState.stage << "\"";
    out << ", \"command\" :\"" << gameState.command << "\"";

    out << ", \"robots_yellow\" : [";
    for(int i = 0; i < gameState.yellow.robots.size(); i++){
        const Robot& robot = gameState.yellow.robots.at(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.id;
        out << ",\"x\" : " << robot.x / scaleX;
        out << ",\"x_vel\" : " << robot.x_buf.avg() / scaleX;
        out << ",\"y\" : " << robot.y / scaleY;
        out << ",\"y_vel\" : " << robot.y_buf.avg() / scaleY;
        out << ",\"orientation\" : " << robot.rot;
        out << "}";
    }
    out << "]";

    out << ", \"robots_blue\" : [";
    for(int i = 0; i < gameState.blue.robots.size(); i++){
        const Robot& robot = gameState.blue.robots.at(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.id;
        out << ",\"x\" : " << robot.x / scaleX;
        out << ",\"x_vel\" : " << robot.x_buf.avg() / scaleX;
        out << ",\"y\" : " << robot.y / scaleY;
        out << ",\"y_vel\" : " << robot.y_buf.avg() / scaleY;
        out << ",\"orientation\" : " << robot.rot;
        out << "}";
    }
    out << "]";

    out << ", \"balls\" : [";
    const Ball& ball = gameState.ball;
    out << "{";
    out << " \"x\" : " << ball.x / scaleX;
    out << ",\"x_vel\" : " << ball.x_buf.avg() / scaleX;
    out << ",\"y\" : " << ball.y / scaleY;
    out << ",\"y_vel\" : " << ball.y_buf.avg() / scaleY;
    out << "}";
    out << "]";

    out << "}";
    out << std::endl;

    return out.str();
}