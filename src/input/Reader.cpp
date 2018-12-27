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

#include "Reader.h"

bool Reader::openFile(std::string filename) {

    // Open file for reading
    in = std::ifstream(filename, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        std::cerr << "[Reader] Error opening file \"" << filename << "\"!" << std::endl;
        return false;
    }
    std::cout << "[Reader] file opened : " << filename << std::endl;

    readHeader();
    return true;
}

void Reader::readHeader(){
    // Read header of file
    in.read((char*) &this->fileHeader, sizeof(this->fileHeader));
    // Log data is stored big endian, convert to host byte order
    fileHeader.version = be32toh(fileHeader.version);
    std::cout << "[Reader] File format version " << fileHeader.version << " detected." << std::endl;
}

bool Reader::isOpen(){
    return in.is_open();
}

bool Reader::isEof(){
    return in.is_open() && in.eof();
}

const SSL_WrapperPacket& Reader::getVision(){
    return wrapperPacket;
}

const SSL_Referee& Reader::getReferee() {
    return refereePacket;
}

const char* Reader::getData(){
    return data;
}

const DataHeader& Reader::getDataHeader(){
    return dataHeader;
}

const FileHeader& Reader::getFileHeader(){
    return fileHeader;
}

void Reader::reset(){
    std::cout << "[Reader] Resetting" << std::endl;
    if(!in.is_open()) {
        return;
    }

    in.clear();
    in.seekg(0, std::ios::beg);
    readHeader();

    packetsRead = 0;
}

int Reader::next(){

    if(!in.is_open()){
        std::cout << "[Reader] File is not open. Could not read next line." << std::endl;
        return FILE_CLOSED;
    }

    if(in.eof()){
        std::cout << "[Reader] EOF reached. Nothing more to read." << std::endl;
        return END_OF_FILE;
    }

    // Read the Data Header of the packet
    in.read((char*) &dataHeader, sizeof(dataHeader));
    // Log data is stored big endian, convert to host byte order
    dataHeader.timestamp = be64toh(dataHeader.timestamp);
    dataHeader.messageType = be32toh(dataHeader.messageType);
    dataHeader.messageSize = be32toh(dataHeader.messageSize);
    // Read the Data of the packet
    data = new char[dataHeader.messageSize];
    in.read(data, dataHeader.messageSize);

    /* === data to object === */
    int type = dataHeader.messageType;
    /* Parse Referee message */
    if(type == MESSAGE_SSL_REFBOX_2013){
        if (refereePacket.ParseFromArray(data, dataHeader.messageSize)) {
            return type;
        } else {
            std::cout << "[Reader] Warning! Could not parse referee packet" << std::endl;
            return MESSAGE_INVALID;
        }
    }

    /* Parse Vision or Geometry message */
    if(type == MESSAGE_SSL_VISION_2010) {
        if(wrapperPacket.ParseFromArray(data, dataHeader.messageSize)){
            if(wrapperPacket.has_detection()){
                return type;
            }
        }else{
            std::cout << "[Reader] Warning! Could not parse vision packet" << std::endl;
            return MESSAGE_INVALID;
        }
    }

    packetsRead++;
    return MESSAGE_UNKNOWN;

}