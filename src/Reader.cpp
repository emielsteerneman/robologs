//
// Created by emiel on 4-12-18.
//

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

Reader::Reader() {
    std::cout << "[Reader] Constructing new reader" << std::endl;
}

Reader::~Reader() {
    std::cout << "[Reader] Destructing reader" << std::endl;
}

void Reader::openFile(std::string filename) {
    this->filename = filename;

    // Open file for reading
    in = std::ifstream(filename, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open())
        std::cerr << "[Reader] Error opening log file \"" << filename << "\"!" << std::endl;
    std::cout << "[Reader] file opened : " << filename << std::endl;

    // Read header of file
    in.read((char*) &this->fileHeader, sizeof(this->fileHeader));
    // Log data is stored big endian, convert to host byte order
    fileHeader.version = be32toh(fileHeader.version);
    std::cout << "[Reader] File format version " << fileHeader.version << " detected." << std::endl;
}

int Reader::next(){
    if(!in.is_open()){
        std::cout << "[Reader] file is not open. Could not read next line." << std::endl;
        return -1;
    }

    if(in.eof()){
        std::cout << "[Reader] At end of file. Nothing more to read." << std::endl;
        return -2;
    }

    in.read((char*) &dataHeader, sizeof(dataHeader));
    // Log data is stored big endian, convert to host byte order
    dataHeader.timestamp = be64toh(dataHeader.timestamp);
    dataHeader.messageType = be32toh(dataHeader.messageType);
    dataHeader.messageSize = be32toh(dataHeader.messageSize);

    data = new char[dataHeader.messageSize];
    in.read(data, dataHeader.messageSize);

    return dataHeader.messageType;
}