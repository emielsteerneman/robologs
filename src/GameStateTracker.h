//
// Created by emiel on 4-12-18.
//

#ifndef SSL_LOGTOOLS_GAMESTATETRACKER_H
#define SSL_LOGTOOLS_GAMESTATETRACKER_H

#include "Reader.h"
#include <vector>
#include <tuple>

#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"

enum RecordingState {
    RECORDING = 0,
    SKIPPING = 1
};

/* TimeStamp
 * Ref command
 * Ref stage
 * For each ball:
 *  x, y, x_vel, y_vel, last_seen
 *
 * For each team:
 *   colour, score, name
 *   For each robot:
 *     x, y, rot, x_vel, y_vel, rot_vel, last_seen
 *
 */

struct Robot {
    int id;
    float x;
    float y;
    float rot;
    float x_vel;
    float y_vel;
    float rot_vel;
    float last_seen;
};

struct Ball {
    float x;
    float y;
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
    double timestamp;
    std::string stage;
    std::string command;

    Team yellow;
    Team blue;
    std::vector<Ball> balls;
};

class GameStateTracker {
public:
    Reader* reader;
    RecordingState recordingState = RecordingState::RECORDING;
    SSL_WrapperPacket wrapperPacket;
    SSL_Referee refereePacket;
    GameState gameState;

    int skipped = 0;
    int recorded = 0;

    GameStateTracker();
    ~GameStateTracker();
    void setReader(Reader* reader);
    void processVision(const SSL_DetectionFrame& packet);
    void processReferee(const SSL_Referee& packet);
    int tick();
};


#endif //SSL_LOGTOOLS_GAMESTATETRACKER_H
