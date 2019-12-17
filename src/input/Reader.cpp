#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <endian.h>

#include "Reader.h"

bool Reader::openFile(const std::string& filename) {
    // Open file for reading
    in = std::ifstream(filename, std::ios_base::in | std::ios_base::binary);
    // Check if the file is actually opened
    if (!in.is_open()) {
        std::cout << "[Reader] Error opening file \"" << filename << "\"!" << std::endl;
        std::cout << "[Reader] " << strerror(errno) << std::endl;
        return false;
    }
    std::cout << "[Reader] file opened : " << filename << std::endl;
    // Reset the reader
    return reset();
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

bool Reader::reset(){
    std::cout << "[Reader] Resetting" << std::endl;
    if(!in.is_open())
        return false;

    in.clear();                 // Resets the error state of the stream
    in.seekg(0, std::ios::beg); // Moves the pointer of the input stream to the beginning

    fileHeader = {};
    dataHeader = {};
    packetsRead = 0;
    // TODO clear data as well? How does that work
    // TODO clear wrapperPacket and refereePacket as well?

    readHeader();

    return true;
}

int Reader::next(){

    if(!in.is_open()){
        std::cout << "[Reader][next] File is not open. Could not read next line." << std::endl;
        return FILE_CLOSED;
    }

    // Read the dataheader of the packet
    in.read((char*) &dataHeader, sizeof(dataHeader));

    if(in.eof()){
        std::cout << "[Reader][next] EOF reached. Nothing more to read." << std::endl;
        return END_OF_FILE;
    }

    // Log data is stored big endian, convert to host byte order
    dataHeader.timestamp = be64toh(dataHeader.timestamp);
    dataHeader.messageType = be32toh(dataHeader.messageType);
    dataHeader.messageSize = be32toh(dataHeader.messageSize);

    // Read data from the file
    // TODO check if data does not cause memory leaks. Where does the previous data go?
    data = new char[dataHeader.messageSize];
    in.read(data, dataHeader.messageSize);
    packetsRead++;

    /** ====== data to object ====== **/
    int type = dataHeader.messageType;

    /* Parse Referee message */
    if(type == MESSAGE_SSL_REFBOX_2013){
        if (refereePacket.ParseFromArray(data, dataHeader.messageSize)) {
//            std::cout << "[Reader][next] Parsed MESSAGE_SSL_REFBOX_2013" << std::endl;
            return type;
        } else {
            std::cout << "[Reader][next] Warning! Could not parse referee packet" << std::endl;
            return MESSAGE_INVALID;
        }
    }

    /* Parse Vision or Geometry message */
    if(type == MESSAGE_SSL_VISION_2010 || type == MESSAGE_SSL_VISION_2014) {
        if(wrapperPacket.ParseFromArray(data, dataHeader.messageSize)){
//            if(type == MESSAGE_SSL_VISION_2010) std::cout << "[Reader][next] Parsed MESSAGE_SSL_VISION_2010" << std::endl;
//            if(type == MESSAGE_SSL_VISION_2014) std::cout << "[Reader][next] Parsed MESSAGE_SSL_VISION_2014" << std::endl;
            return type;
        }else{
            std::cout << "[Reader][next] Warning! Could not parse vision packet" << std::endl;
            return MESSAGE_INVALID;
        }
    }

    return MESSAGE_UNKNOWN;
}