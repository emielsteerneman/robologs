//
// Created by emiel on 17-12-19.
//

#include "ASCII.h"
#include <algorithm>

int posToPos(double x, double y, int FIELD_WIDTH, int FIELD_HEIGHT){
    double dx = (6000 + x) / 12000.;
    double dy = (4500 + y) / 9000.;

    double cx = std::clamp(dx, 0., 1.);
    double cy = std::clamp(dy, 0., 1.);

    int px = cx * FIELD_WIDTH;
    int py = cy * FIELD_HEIGHT;

//    std::cout << std::setprecision(4) << std::fixed;
//    std::cout << symbol << " " << x << " " << y << " | " << dx << " " << dy << " | " << px << " " << py << std::endl;

    return py * (FIELD_WIDTH * 3 + 1) + px * 3 + 1;
}

std::string ASCII::GamestateToString(const GameState &gameState, int FIELD_WIDTH, int FIELD_HEIGHT) {

    std::string FIELD = emptyField(FIELD_WIDTH, FIELD_HEIGHT);

    for(const Robot& bot : gameState.yellow.robots) {
        int position = posToPos(bot.x_buf.avg(), bot.y_buf.avg(), FIELD_WIDTH, FIELD_HEIGHT);
        FIELD[position] = 'Y'; // *(std::to_string(bot.id).c_str())
    }

    for(const Robot& bot : gameState.blue.robots){
        int position = posToPos(bot.x_buf.avg(), bot.y_buf.avg(), FIELD_WIDTH, FIELD_HEIGHT);
        FIELD[position] = 'B';
    }

    int position = posToPos(gameState.ball.x_buf.avg(), gameState.ball.y_buf.avg(), FIELD_WIDTH, FIELD_HEIGHT);
    FIELD[position] = '#';

    time_t epoch = gameState.timestamp;
    FIELD += asctime(gmtime(&epoch));
    FIELD += gameState.stage + " | " + gameState.command;

    return FIELD;
}

std::string ASCII::emptyField(int FIELD_WIDTH, int FIELD_HEIGHT){
    std::string FIELD;

    FIELD += " --";
    for (int w = 1; w < FIELD_WIDTH-1; w++)
        FIELD += "---";
    FIELD += "-- ";

    FIELD += "\n";

    for(int h = 1; h < FIELD_HEIGHT-1; h++) {
        FIELD += " | ";
        for (int w = 1; w < FIELD_WIDTH-1; w++) {
            int i = (h+w)%10;
            FIELD += "   ";
        }
        FIELD += " | ";
        FIELD += "\n";
    }

    FIELD += " --";
    for (int w = 1; w < FIELD_WIDTH-1; w++)
        FIELD += "---";
    FIELD += "-- ";

    FIELD += "\n";

    return FIELD;
}