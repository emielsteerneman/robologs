//
// Created by emiel on 4-12-18.
//

#ifndef SSL_LOGTOOLS_GAMESTATETRACKER_H
#define SSL_LOGTOOLS_GAMESTATETRACKER_H

#define CIRCULAR_BUFFER_SIZE 20
#include "Reader.h"
#include <vector>
#include <tuple>
#include <time.h>
#include <iostream>
#include <iomanip>

#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"

enum RecordingState {
    INVALID = -1,
    RECORDING = 0,
    SKIPPING = 1,

};

class CircularBuffer {
    int bufferAt = 0;
    int bufferTotal = 0;
    float buffer[CIRCULAR_BUFFER_SIZE];

public:
    void put(float f){
        buffer[bufferAt] = f;
        bufferAt = (bufferAt + 1) % CIRCULAR_BUFFER_SIZE;
        bufferTotal++;
    }
    void reset(){
        bufferAt = 0;
        bufferTotal = 0;
    }
    float avg() const {
        float total = 0;
        if(bufferTotal < CIRCULAR_BUFFER_SIZE) {
            for (int i = 0; i < bufferAt; i++)
                total += buffer[i];
            total /= bufferAt;
        }else {
            for (int i = 0; i < CIRCULAR_BUFFER_SIZE; i++)
                total += buffer[i];
            total /= CIRCULAR_BUFFER_SIZE;
        }

        return total;
    }
};

struct Robot {
    int id;
    float x;
    CircularBuffer x_buf;
    float y;
    CircularBuffer y_buf;
    float rot;
    CircularBuffer rot_buf;
    float x_vel;
    float y_vel;
    float rot_vel;
    double last_seen;
};

struct Ball {
    float x;
    CircularBuffer x_buf;
    float y;
    CircularBuffer y_buf;
    float x_vel;
    float y_vel;
    float last_seen;
};

struct Team {
    int score;
    std::string name;
    std::vector<Robot> robots;
};

struct GameInfo {
    double t_start = 0;
    double t_stop = 0;
    double t_duration = 0;

    std::vector<std::tuple<double, std::string, std::string, RecordingState >> timeline;

    int nPackets = 0;
    int referee_nPackets = 0;
    int vision_nPackets = 0;
    int geometry_nPackets = 0;
    int invalid_nPackets = 0;

    double pps;
    double referee_pps;
    double vision_pps = 0;
    double geometry_pps;
    double invalid_pps;
};

struct GameState {
    bool isInitial;
    double timestamp;
    std::string stage;
    std::string command;

    Team yellow;
    Team blue;
//    std::vector<Ball> balls;
    Ball ball;

    GameInfo gameInfo;
};


class GameStateTracker {

    SSL_WrapperPacket wrapperPacket;
    SSL_Referee refereePacket;
    GameState gameState;

    int hz; // Hz at which the game should update
    double lastInterval = true; // Timestamp of the last interval. Used to provide consistent FPS

public:

    Reader* reader;
    RecordingState recordingState = RecordingState::RECORDING;
    int parsed = 0;

    GameStateTracker();

    void setInput(Reader *reader);
    void setHz(int);

    void processVision(const SSL_DetectionFrame& packet);
    void processReferee(const SSL_Referee& packet);
    bool tick();

    void addInfo();

    const GameState& get();
    const GameInfo& getInfo();
};


#endif //SSL_LOGTOOLS_GAMESTATETRACKER_H
