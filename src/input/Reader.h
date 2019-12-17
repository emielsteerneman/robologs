/**
 * The sole responsibility of the reader is to convert the data from a .log file to protobuf objects.
 * The reader stores the latest package read, which can given using the corresponding get-functions
 */

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
    int64_t timestamp = 0;   // Timestamp in ns
    int32_t messageType = 0; // Message type
    int32_t messageSize = 0; // Size of protobuf message in bytes
};

enum MessageType
{
    MESSAGE_INVALID = -3,
    FILE_CLOSED = -2,
    END_OF_FILE = -1,
    MESSAGE_BLANK = 0,
    MESSAGE_UNKNOWN = 1,
    MESSAGE_SSL_VISION_2010 = 2,
    MESSAGE_SSL_REFBOX_2013 = 3,
    MESSAGE_SSL_VISION_2014 = 4
};

class Reader {

    std::ifstream in;       // File stream used to read the file
    FileHeader fileHeader;  // FileHeader from the file most recently opened
    DataHeader dataHeader;  // DataHeader from the packet most recently read
    char* data = nullptr;   // Data of the packet most recently read

    SSL_WrapperPacket wrapperPacket; // Most recently parsed vision packet
    SSL_Referee refereePacket; // Most recently parsed referee packet

    void readHeader();      // Reads the FileHeader of the file stream

public:

    int packetsRead = 0;

    /** @return the FileHeader from the file most recently opened */
    const FileHeader& getFileHeader();

    /** @return the DataHeader from the most recently packet read */
    const DataHeader& getDataHeader();

    /** @return the data from the packet most recently read */
    const char* getData();

    /** @return the most recently SSL_WrapperPacket read */
    const SSL_WrapperPacket& getVision();

    /** @return the most recently SSL_Referee packet read */
    const SSL_Referee& getReferee();

    /** @param filename : The path of the file to be opened and read
     * @return true if the file has been opened successfully
     */
    bool openFile(const std::string& filename);

    /** @return true if the file is opened and the reader is reset, false if not */
    bool isOpen();

    /** @return true if the file is open, false if not */
    bool isEof();

    /** Resets Reader to initial state, as if it has been opened for the first time
     * @return true if the reader has been successfully reset
     * */
    bool reset();

    /** Reads the next packet in the file stream, stores the data and dataheader,
     * and tries to parse the data into either SSL_WrapperPacket or SSL_Referee
     * @return MessageType, indicating if the packet was read, and of what type the packet is
     */
    int next();
};

#endif //SSL_LOGTOOLS_READER_H











