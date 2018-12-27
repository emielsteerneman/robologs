#ifndef ROBOLOGS_GAMEINFOTRACKER_H
#define ROBOLOGS_GAMEINFOTRACKER_H

#include "../input/Reader.h"

struct GameInfo {
    double t_start = 0;
    double t_stop = 0;

    std::vector<std::tuple<double, std::string, std::string>> timeline;

    int nPackets = 0;
    int referee_nPackets = 0;
    int vision_nPackets = 0;
    int geometry_nPackets = 0;
};

class GameInfoTracker {
public:
    GameInfoTracker();
    const GameInfo* get();
    std::string getStage();
    std::string getCommand();

    void process(const SSL_WrapperPacket& wrapperPacket);
    void process(const SSL_Referee& refereePacket);
    void process(const DataHeader& dataHeader);

    void reset();

private:
    GameInfo gameInfo;
    bool isInitial;
    std::string lastCommand;
    std::string lastState;


};


#endif //ROBOLOGS_GAMEINFOTRACKER_H
