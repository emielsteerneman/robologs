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
//    const int bufferSize = CIRCULAR_BUFFER_SIZE;
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

struct GameState {
    bool isInitial;
    double timestamp;
    std::string stage;
    std::string command;

    Team yellow;
    Team blue;
//    std::vector<Ball> balls;
    Ball ball;

    std::vector<std::tuple<double, std::string, std::string, RecordingState >> timeline;
};

class GameStateTracker {
public:
    Reader* reader;
    RecordingState recordingState = RecordingState::RECORDING;
    SSL_WrapperPacket wrapperPacket;
    SSL_Referee refereePacket;
    GameState gameState;

    int hz; // Hz at which the game should update
    double lastInterval;

    int parsed = 0;

    GameStateTracker(int hz);
    ~GameStateTracker();
    void setReader(Reader* reader);
    void processVision(const SSL_DetectionFrame& packet);
    void processReferee(const SSL_Referee& packet);
    bool tick();
    void addTimeline();
};


#endif //SSL_LOGTOOLS_GAMESTATETRACKER_H
