//
// Created by emiel on 4-12-18.
//

#ifndef SSL_LOGTOOLS_READER_H
#define SSL_LOGTOOLS_READER_H

#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"
#include <fstream>

struct FileHeader
{
    char name[12]; // SSL_LOG_FILE
    int32_t version; // Default file format is version 1
};

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

class Reader {
    std::string filename;
    FileHeader fileHeader;
    DataHeader dataHeader;
    char* data = nullptr;
    std::ifstream in;

    void readHeader();

public:

    Reader();
    ~Reader();

    const char* getData();
    const DataHeader& getDataHeader();
    const FileHeader& getFileHeader();

    int read = 0;

    bool openFile(std::string filename);
    bool isOpen();
    bool isEof();
    void reset();
    int next();
};

#endif //SSL_LOGTOOLS_READER_H
