/* Reads .log files using a stream. Extracted data can be parsed into protobuf objects */

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
    MESSAGE_INVALID = -3,
    FILE_CLOSED = -2,
    END_OF_FILE = -1,
    MESSAGE_BLANK = 0,
    MESSAGE_UNKNOWN = 1,
    MESSAGE_SSL_VISION_2010 = 4,
    MESSAGE_SSL_REFBOX_2013 = 3
};

class Reader {

    std::ifstream in;       // File stream used to read the file
    FileHeader fileHeader;  // FileHeader from the file last opened
    DataHeader dataHeader;  // DataHeader from the packet last read
    char* data = nullptr;   // Data of the packet last read

    SSL_WrapperPacket wrapperPacket;
    SSL_Referee refereePacket;

    void readHeader();      // Reads the FileHeader of the file stream

public:

    /** @return the FileHeader from the file last opened */
    const FileHeader& getFileHeader();
    /** @return the DataHeader from the last packet read */
    const DataHeader& getDataHeader();
    /** @return the data from the packet last read */
    const char* getData();
    /** @return the last SSL_WrapperPacket read */
    const SSL_WrapperPacket& getVision();
    /** @return the last SSL_Referee packet read */
    const SSL_Referee& getReferee();

    int packetsRead = 0;

    /** @param filename : The path of the file to be opened and read
     * @return true if the file has been opened successfully
     */
    bool openFile(std::string filename);

    /** @return true if the file is open, false if not */
    bool isOpen();

    /** @return true if the file is open, false if not */
    bool isEof();

    /** Resets Reader to initial state, as if it has been opened for the first time */
    void reset();

    /** Reads the next packet in the file stream, and stores the data header and the data
     * @return MessageType, indicating if the packet was read, and of what type the packet is
     */
    int next();
};

#endif //SSL_LOGTOOLS_READER_H











