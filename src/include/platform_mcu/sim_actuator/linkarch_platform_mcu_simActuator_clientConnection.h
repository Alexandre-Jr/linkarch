#ifndef LINKARCH_PLATFORM_MCU_SA_CLIENTCONNECTION
#define LINKARCH_PLATFORM_MCU_SA_CLIENTCONNECTION




// Linakrch Config
#include "linkarch_config.h"

// HAL Includes
#include "linkarch_hal.h"

// OSAL Includes
#include "linkarch_osal.h"

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>




/* Defines */

// Message defines
#define LINKARCH_MESSAGETYPE_SIZE_IN_BYTES 1
#define LINKARCH_MESSAGELENGTH_SIZE_IN_BYTES 1
#define LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES (LINKARCH_MESSAGETYPE_SIZE_IN_BYTES + LINKARCH_MESSAGELENGTH_SIZE_IN_BYTES)

// Socket defines
#define LINKARCH_MAX_TIME_TO_RECEIVE_RESPONSE_MS 10
#define LINKARCH_MAX_TRYS_TO_RECEIVE_GET_RESPONSE 5
#define LINKARCH_MAX_TIME_TO_GET_MUTEX_MS 10
#define LINKARCH_TIMEOUT_READYTOREAD_US 10000




/* Simulation Socket */

typedef uint8_t linkarch_msgDataPart_t;
typedef linkarch_msgDataPart_t * linkarch_msgData_t;
typedef uint8_t linkarch_msgDataSize_t;
typedef uint8_t linkarch_msgType_t;

typedef struct
{

    linkarch_msgType_t message_type;
    linkarch_msgDataSize_t message_dataSize;
    linkarch_msgData_t message_data;

}   linkarch_message_t;

typedef enum
{

    LINKARCH_MESSAGE_TYPE_PUT = 0,
    LINKARCH_MESSAGE_TYPE_GET,
    LINKARCH_MESSAGE_TYPE_ACK,
    LINKARCH_MESSAGE_TYPE_HDS,          // Handshake message
    LINKARCH_MESSAGE_TYPE_RES,
    LINKARCH_NUMBER_OF_MESSAGE_TYPES

} linkarch_messageTypes_t;

typedef struct 
{

    int socket_fd;
    struct sockaddr_un socket_addr;

} linkarch_socket_t;

typedef struct
{

    linkarch_socket_t linkarch_socket;
    linkarch_socket_t linkserver_socket;
    linkarch_semaphorehandle_t sendConnection_mutex;
    linkarch_semaphorehandle_t receiveConnection_mutex;

} linkarch_clientConnection_t;




/* Simulation Socket */

// Initialization functions

bool linkarch_clientConnection_init();
bool linkarch_clientConnection_initClientSocket(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_initServerSocket(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_initSocketType(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_clientSocketBind(linkarch_socket_t * socket_toInit);
bool linkarch_clientConnection_serverSocketConnect(linkarch_socket_t * socket_toInit);


// Send and receive functions

bool linkarch_clientConnection_unsafeSendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message);
bool linkarch_clientConnection_sendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message);

// Message need to be free after use
bool linkarch_clientConnection_receiveMessage(linkarch_socket_t * targetSocket, linkarch_message_t * message);  

bool linkarch_clientConnection_handshake();
bool linkarch_clientConnection_sendPUTMessage(linkarch_msgData_t data, linkarch_msgDataSize_t dataSize);
bool linkarch_clientConnection_sendGETMessage(linkarch_msgData_t data, linkarch_msgDataSize_t dataSize);
bool linkarch_clientConnection_sendACKMessage();


// Status functions

bool linkarch_clientConnection_isConnected();
bool linkarch_clientConnection_isReadyToRead();


// User functions
bool linkarch_clientConnection_PUT(linkarch_msgData_t sendData, linkarch_msgDataSize_t dataSize);

// receivedData need to be free after use
bool linkarch_clientConnection_GET(linkarch_msgData_t sendData, linkarch_msgDataSize_t sendDataSize, linkarch_msgData_t * receivedData, linkarch_msgDataSize_t * receivedDataSize); 


// Message handling functions (buffer need to be free after use)
linkarch_msgData_t linkarch_messageToBuffer(const linkarch_message_t * message);
bool linkarch_freeMessage(linkarch_message_t * message);


// Mutex functions

bool linkarch_clientConnection_takeSendConnectionMutex();
bool linkarch_clientConnection_giveSendConnectionMutex();
bool linkarch_clientConnection_takeReceiveConnectionMutex();
bool linkarch_clientConnection_giveReceiveConnectionMutex();

// Socket Buffer functions

bool linkarch_clientConnection_clearSocketBuffer(linkarch_socket_t * socket_toClear);


// Waiting functions

bool linkarch_clientConnection_waitForResponse();
bool linkarch_clientConnection_waitForAck();




/* Macros */
#define LINKARCH_MESSAGE_INIT(type, data, dataSize) \
    {\
        .message_type = type, \
        .message_dataSize = dataSize, \
        .message_data = data \
    }

#define HANDSHAKE_MESSAGE_INIT(data, dataSize) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_HDS, (linkarch_msgData_t) data, dataSize)

#define PUT_MESSAGE_INIT(data, dataSize) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_PUT, (linkarch_msgData_t) data, dataSize)

#define GET_MESSAGE_INIT(data, dataSize) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_GET, (linkarch_msgData_t) data, dataSize)

#define ACK_MESSAGE_INIT() \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_ACK, NULL, 0)




#endif  // LINKARCH_PLATFORM_MCU_SA_CLIENTCONNECTION