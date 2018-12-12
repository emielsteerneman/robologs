//
// Created by emiel on 8-12-18.
//

#include "Drawer.h"
#include <iostream>

#include <opencv2/opencv.hpp>

std::string getImageType(int number)
{
    // find type
    int imgTypeInt = number%8;
    std::string imgTypeString;

    switch (imgTypeInt)
    {
        case 0:
            imgTypeString = "8U";
            break;
        case 1:
            imgTypeString = "8S";
            break;
        case 2:
            imgTypeString = "16U";
            break;
        case 3:
            imgTypeString = "16S";
            break;
        case 4:
            imgTypeString = "32S";
            break;
        case 5:
            imgTypeString = "32F";
            break;
        case 6:
            imgTypeString = "64F";
            break;
        default:
            break;
    }

    // find channel
    int channel = (number/8) + 1;

    std::stringstream type;
    type<<"CV_"<<imgTypeString<<"C"<<channel;

    return type.str();
}

void Drawer::drawFieldOnImage(const cv::Mat& img){

    cv::Scalar lineColour(100, 100, 100);

    int x0 = 360;
    int y0 = 90;
    int x1 = 1920-360;
    int y1 = 1080-90;

    // Field
    cv::Point f_tl (x0, y0);
    cv::Point f_tr (x1, y0);
    cv::Point f_bl (x0, y1);
    cv::Point f_br (x1, y1);
    // Keeper Area Left
    cv::Point g_l_tl (x0    , y0+330);
    cv::Point g_l_tr (x0+120, y0+330);
    cv::Point g_l_bl (x0    , y0+570);
    cv::Point g_l_br (x0+120, y0+570);
    // Keeper Area Right
    cv::Point g_r_tl (x1-120, y0+330);
    cv::Point g_r_tr (x1    , y0+330);
    cv::Point g_r_bl (x1-120, y0+570);
    cv::Point g_r_br (x1    , y0+570);

    // Draw Field
    cv::line(img, f_tl, f_tr, lineColour, 1, cv::LINE_AA);
    cv::line(img, f_tr, f_br, lineColour, 1, cv::LINE_AA);
    cv::line(img, f_br, f_bl, lineColour, 1, cv::LINE_AA);
    cv::line(img, f_bl, f_tl, lineColour, 1, cv::LINE_AA);
    // Draw Keeper Area Left
    cv::line(img, g_l_tl, g_l_tr, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_l_tr, g_l_br, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_l_br, g_l_bl, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_l_bl, g_l_tl, lineColour, 1, cv::LINE_AA);
    // Draw Keeper Area Right
    cv::line(img, g_r_tl, g_r_tr, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_r_tr, g_r_br, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_r_br, g_r_bl, lineColour, 1, cv::LINE_AA);
    cv::line(img, g_r_bl, g_r_tl, lineColour, 1, cv::LINE_AA);

}

void Drawer::drawTimeline(cv::Mat& img, const GameState& gameState){

    double duration = std::get<0>(gameState.timeline.back()) - std::get<0>(gameState.timeline.front());
    double first = std::get<0>(gameState.timeline.front());

    std::vector<double> positions;

    for(const auto& tup : gameState.timeline){
        double d = (std::get<0>(tup) - first) / duration;
        positions.emplace_back(d);
    }

    int dy = 1;
    int y = 1030;
    for(int i = 0; i < positions.size() - 2; i++){
        double x0 = (int)(positions.at(i) * 1920);
        double x1 = (int)(positions.at(i+1) * 1920);
        cv::Rect rect(x0, y, x1-x0, y);
        cv::Point p1(x0, y);
        cv::Point p2(x1, y+10);

        cv::rectangle(img, p1, p2, cv::Scalar(127 + 127*dy, 0, 127-127*dy), CV_FILLED);
        dy *= -1;
    }

}

Drawer::Drawer() {
    std::cout << "Constructing Drawer" << std::endl;
//    const std::string filename = "../../video_out.avi";
//    if(!writer.open(filename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30.0, cv::Size(1920, 1080))){
    const std::string filename = "../../tiger_cmus_15_full_fast.mp4";
//    if(!writer.open(filename, cv::VideoWriter::fourcc('H', '2', '6', '4'), 30.0, cv::Size(1920, 1080))){
    if(!writer.open(filename, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), 30.0, cv::Size(1920, 1080))){
        std::cout << "[Drawer] Warning! Could not open writer" << std::endl;
    }
    counter = 0;

    cv::Mat kernel = cv::getGaussianKernel(kernelDiameter, 4);
    kernel = kernel * kernel.t();
    cv::normalize(kernel, kernel, 0.0, 600.0, cv::NORM_MINMAX, CV_32FC1);
    kernel.convertTo(kernel, CV_8UC1);

    cv::Mat black = cv::Mat::zeros(kernelDiameter, kernelDiameter, CV_8UC1);

    std::vector<cv::Mat> channelsRed;
    channelsRed.push_back(black);
    channelsRed.push_back(black);
    channelsRed.push_back(kernel);
    cv::merge(channelsRed, kernelRed);

    std::vector<cv::Mat> channelsBlue;
    channelsBlue.push_back(kernel);
    channelsBlue.push_back(black);
    channelsBlue.push_back(black);
    cv::merge(channelsBlue, kernelBlue);

    std::vector<cv::Mat> channelsOrange;
    channelsOrange.push_back(black);
    channelsOrange.push_back(kernel * 0.5);
    channelsOrange.push_back(kernel);
    cv::merge(channelsOrange, kernelOrange);

    img = cv::Mat::zeros(height, width, CV_8UC3);
}


void Drawer::drawGameState(const GameState& gameState){

//    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3); // =(height, width, CV_8UC3)

    drawFieldOnImage(img);

    for(const Robot& bot : gameState.yellow.robots){
        int x =  int(bot.x_buf.avg() / 10) + 600 + 360;     // [-6000, 6000] -> [360, 1560]
        int y =  int(bot.y_buf.avg() / 10) + 450 +  90;     // [-4500, 4500] -> [ 90,  990]
        cv::Rect roi(x - kernelRadius, y - kernelRadius, kernelDiameter, kernelDiameter);
        cv::Mat proi = img(roi);
        cv::max(proi, kernelRed, proi);
    }

    for(const Robot& bot : gameState.blue.robots){
        int x =  int(bot.x_buf.avg() / 10) + 600 + 360;     // [-6000, 6000] -> [360, 1560]
        int y =  int(bot.y_buf.avg() / 10) + 450 +  90;     // [-4500, 4500] -> [ 90,  990]
        cv::Rect roi(x - kernelRadius, y - kernelRadius, kernelDiameter, kernelDiameter);
        cv::Mat proi = img(roi);
        cv::max(proi, kernelBlue, proi);
    }

    {
        int x =  int(gameState.ball.x_buf.avg() / 10) + 600 + 360;     // [-6000, 6000] -> [360, 1560]
        int y =  int(gameState.ball.y_buf.avg() / 10) + 450 +  90;     // [-4500, 4500] -> [ 90,  990]
        cv::Rect roi(x - kernelRadius, y - kernelRadius, kernelDiameter, kernelDiameter);
        cv::Mat proi = img(roi);
        cv::max(proi, kernelOrange, proi);
    }

    time_t time = (time_t)gameState.timestamp;
    char buff[32];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));

    cv::putText(img, "frame=" + std::to_string(counter), cv::Point(30, 30) , cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    cv::putText(img, "stage=" + gameState.stage,         cv::Point(30, 60) , cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    cv::putText(img, "cmd  =" + gameState.command,       cv::Point(30, 90) , cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    cv::putText(img, "time =" + std::string(buff),       cv::Point(30, 120), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

    if(gameState.stage == "NORMAL_FIRST_HALF") {
        cv::putText(img, gameState.yellow.name, cv::Point(360, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        cv::putText(img, gameState.blue.name, cv::Point(1400, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }else{
        cv::putText(img, gameState.blue.name, cv::Point(360, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        cv::putText(img, gameState.yellow.name, cv::Point(1400, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }

    drawTimeline(img, gameState);

    if(writeCounter++ % 30 == 0)
        writer.write(img);
    counter++;
//    img = cv::Mat::zeros(height, width, CV_8UC3);
    img *= 0.9;
}
