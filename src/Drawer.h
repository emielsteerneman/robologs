//
// Created by emiel on 8-12-18.
//

#ifndef ROBOLOGS_DRAWER_H
#define ROBOLOGS_DRAWER_H

#include "GameStateTracker.h"
#include <opencv2/opencv.hpp>

class Drawer {
public:
    Drawer();

    void drawFieldOnImage(const cv::Mat& img);
    void test();
    void drawGameState(const GameState& gameState);
    void drawTimeline(cv::Mat& img, const GameState& gameState);

    std::string previousCommand;

    cv::VideoCapture reader;
    cv::VideoWriter writer;
    int counter;
    int writeCounter = 0;

    cv::Mat kernelRed;
    cv::Mat kernelBlue;
    cv::Mat kernelOrange;

    cv::Mat img;
    cv::Mat frame;

    const int width = 1920;
    const int height = 1080;
    const int kernelRadius = 10;
    const int kernelDiameter = kernelRadius * 2 + 1;
};


#endif //ROBOLOGS_DRAWER_H
