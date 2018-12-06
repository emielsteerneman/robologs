#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <stdint.h>
#include <endian.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <set>

struct FileHeader
{
    char name[12]; // SSL_LOG_FILE
    int32_t version; // Default file format is version 1
};

const char* DEFAULT_FILE_HEADER_NAME = "SSL_LOG_FILE";
const int32_t DEFAULT_FILE_VERSION = 1;

struct DataHeader
{
    int64_t timestamp; // Timestamp in ns
    int32_t messageType; // Message type
    int32_t messageSize; // Size of protobuf message in bytes
};

enum MessageType
{
    MESSAGE_BLANK = 0,
    MESSAGE_UNKNOWN = 1,
    MESSAGE_SSL_VISION_2010 = 4,
    MESSAGE_SSL_REFBOX_2013 = 3
};

std::ofstream out;

void writeVisionPacket(const SSL_DetectionFrame& frame){
    return;
    out << std::setprecision(32);
    out << "{";
    out << " \"t_capture\" :" << frame.t_capture();
    out << ",\"camera_id\" :" << frame.camera_id();

    out << ", \"robots_yellow\" : [";
    for(int i = 0; i < frame.robots_yellow_size(); i++){
        SSL_DetectionRobot robot = frame.robots_yellow(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.robot_id();
        out << ",\"x\" : " << robot.x();
        out << ",\"y\" : " << robot.y();
        out << ",\"orientation\" : " << robot.orientation();
        out << ",\"confidence\" : " << robot.confidence();
        out << "}";
    }
    out << "]";

    out << ", \"robots_blue\" : [";
    for(int i = 0; i < frame.robots_blue_size(); i++){
        SSL_DetectionRobot robot = frame.robots_blue(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"robot_id\" : " << robot.robot_id();
        out << ",\"x\" : " << robot.x();
        out << ",\"y\" : " << robot.y();
        out << ",\"orientation\" : " << robot.orientation();
        out << ",\"confidence\" : " << robot.confidence();
        out << "}";
    }
    out << "]";

    out << ", \"balls\" : [";
    for(int i = 0; i < frame.balls_size(); i++){
        SSL_DetectionBall ball = frame.balls(i);
        if(i != 0)
            out << ",";
        out << "{";
        out << " \"x\" : " << ball.x();
        out << ",\"y\" : " << ball.y();
        out << ",\"confidence\" : " << ball.confidence();
        out << "}";
    }
    out << "]";

    out << "}";
    out << std::endl;
}

void writeRefereePacket(const SSL_Referee& frame){
    std::cout << "Writing referee packet" << std::endl;

    SSL_Referee_Stage stage = frame.stage();
    int timeleft = frame.stage_time_left();
    std::cout << std::setprecision(32) << timeleft << std::endl;

    SSL_Referee_TeamInfo blue = frame.yellow();
    std::cout << blue.score() << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "No input file name specified!" << std::endl;
        std::cout << "Please run \"examplereader <log file>\"." << std::endl;

        return -1;
    }

    const char *filename = argv[1];

    std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);

    if (!in.is_open()) {
        std::cerr << "Error opening log file \"" << filename << "\"!" << std::endl;
    }

    // Open write file
    std::string filenameOut = std::string(filename);
    filenameOut = filenameOut.substr(0, filenameOut.find_last_of('.')) + ".txt";
    out = std::ofstream(filenameOut);

    std::cout << "filenameOut=" << filenameOut.c_str() << std::endl;

    FileHeader fileHeader;
    in.read((char*) &fileHeader, sizeof(fileHeader));
    // Log data is stored big endian, convert to host byte order
    fileHeader.version = be32toh(fileHeader.version);

    int counter = 0;

    if (strncmp(fileHeader.name, DEFAULT_FILE_HEADER_NAME, sizeof(fileHeader.name)) == 0) {
        std::cout << "File format version " << fileHeader.version << " detected." << std::endl;

        if (fileHeader.version == DEFAULT_FILE_VERSION) {
            unsigned long refereePackets = 0;
            unsigned long visionPackets = 0;


            // For each message in the file
            while (!in.eof()) {
//                std::cout << filenameOut << "           "  << counter++ << std::endl;
//                if(100 < counter++) break;

                DataHeader dataHeader;
                in.read((char*) &dataHeader, sizeof(dataHeader));
                // Log data is stored big endian, convert to host byte order
                dataHeader.timestamp = be64toh(dataHeader.timestamp);
                dataHeader.messageType = be32toh(dataHeader.messageType);
                dataHeader.messageSize = be32toh(dataHeader.messageSize);

                char* data = new char[dataHeader.messageSize];
                in.read(data, dataHeader.messageSize);

//                std::cout << "message type=" << dataHeader.messageType << std::endl;

                if (dataHeader.messageType == MESSAGE_SSL_VISION_2010) {
                    continue;
                    SSL_WrapperPacket packet;
                    if (packet.ParseFromArray(data, dataHeader.messageSize)) {

//                        std::cout << packet.detection().frame_number() << std::endl;

                        writeVisionPacket(packet.detection());

                        visionPackets++;
//                        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) dataHeader.timestamp / 1e9
//                                  << ": Read " << visionPackets << " vision packets and " << refereePackets << " referee packets!" << std::endl;
                    } else {
                        std::cerr << "Error parsing vision packet!" << std::endl;
                    }
                } else if (dataHeader.messageType == MESSAGE_SSL_REFBOX_2013) {
                    SSL_Referee packet;
                    if (packet.ParseFromArray(data, dataHeader.messageSize)) {

                        writeRefereePacket(packet);

                        refereePackets++;
//                        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) dataHeader.timestamp / 1e9
//                                  << ": Read " << visionPackets << " vision packets and " << refereePackets << " referee packets!" << std::endl;
                    } else {
                        std::cerr << "Error parsing vision packet!" << std::endl;
                    }
                }

                delete data;
            }
        }
    } else {
        std::cerr << "Error log file is unsupported or corrupted!" << std::endl;
    }

    out.close();

    return 0;
}
