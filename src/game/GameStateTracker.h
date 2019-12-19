#ifndef SSL_LOGTOOLS_GAMESTATETRACKER_H
#define SSL_LOGTOOLS_GAMESTATETRACKER_H

#define CIRCULAR_BUFFER_SIZE 20
#include <vector>
#include <tuple>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "protobuf/ssl_referee.pb.h"

#include "protobuf/messages_robocup_ssl_wrapper.pb.h"

#include "input/Reader.h"

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
    float avgVel() const {
        // First result is buffer[bufferAt]
        // Last result is buffer[bufferAt-1]

        int prev = bufferAt - 1;
        if(prev < 0) prev += CIRCULAR_BUFFER_SIZE;
        float distance = buffer[prev] - buffer[bufferAt];
        return distance / CIRCULAR_BUFFER_SIZE;
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
    bool isInitial = true;
    double timestamp;
    std::string stage;
    std::string command;

    Team yellow;
    Team blue;
    Ball ball;
};


class GameStateTracker {
private:
    GameState gameState;
    Reader& reader;
    int hz = 30; // Hz at which the game should update

    double lastInterval = 0; // Timestamp of the last interval. Used to provide consistent FPS
    double nextInterval = 0;

public:
    RecordingState recordingState = RecordingState::RECORDING;
    int parsed = 0;

    explicit GameStateTracker(Reader& _reader);

    const GameState& get();

    void setHz(int);

    void reset();

    bool processVision(const SSL_DetectionFrame& packet);
    void processReferee(const SSL_Referee& packet);
    bool tick();

    int getHz();
};

#endif //SSL_LOGTOOLS_GAMESTATETRACKER_H
