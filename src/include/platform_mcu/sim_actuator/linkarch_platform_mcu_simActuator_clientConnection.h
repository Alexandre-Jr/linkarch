#ifndef LINKARCH_PLATFORM_MCU_SIMSOCKET
#define LINKARCH_PLATFORM_MCU_SIMSOCKET




// Linakrch Config
#include "linkarch_config.h"

// HAL Includes
#include "linkarch_hal.h"

// OSAL Includes
#include "linkarch_osal.h"

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>




// Defines 
#define LINKARCH_MESSAGETYPE_SIZE_IN_BYTES 1
#define LINKARCH_MESSAGELENGTH_SIZE_IN_BYTES 1
#define LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES (LINKARCH_MESSAGETYPE_SIZE_IN_BYTES + LINKARCH_MESSAGELENGTH_SIZE_IN_BYTES)

#define LINKARCH_MAX_TIME_TO_RECEIVE_RESPONSE_MS 100


/* Simulation Socket */
typedef struct
{

    uint8_t message_type;
    uint8_t message_length;
    const char * message_data;

} linkarch_message_t;

typedef enum
{

    LINKARCH_MESSAGE_TYPE_PUT,
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

} linkarch_simConnection_t;





/* Simulation Socket */

// Initialization functions

bool linkarch_clientConnection_init();
bool linkarch_clientConnection_initClientSocket(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_initServerSocket(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_initSocketType(linkarch_socket_t * socket_toInit, const char * socket_path);
bool linkarch_clientConnection_clientSocketBind(linkarch_socket_t * socket_toInit);
bool linkarch_clientConnection_serverSocketConnect(linkarch_socket_t * socket_toInit);


// Send and receive functions

bool linkarch_clientConnection_sendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message);

// Message need to be free after use
bool linkarch_clientConnection_receiveMessage(linkarch_socket_t * targetSocket, linkarch_message_t * message);

bool linkarch_clientConnection_handshake();
bool linkarch_clientConnection_sendPUTMessage(const char * data);
bool linkarch_clientConnection_sendGETMessage(const char * data);
bool linkarch_clientConnection_sendACKMessage();


// Status functions

bool linkarch_clientConnection_isConnected();
bool linkarch_clientConnection_isReadyToRead();


// User functions
bool linkarch_clientConnection_PUT(const char * sendData);

// receivedData need to be free after use
bool linkarch_clientConnection_GET(const char * sendData, char ** receivedData);


// Message handling functions (buffer need to be free after use)
char * linkarch_messageToBuffer(const linkarch_message_t * message);
bool linkarch_freeMessage(linkarch_message_t * message);



/* Macros */
#define LINKARCH_MESSAGE_INIT(type, data) \
    {\
        .message_type = type, \
        .message_length = strlen(data), \
        .message_data = data \
    }

#define HANDSHAKE_MESSAGE_INIT(data) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_HDS, data)

#define PUT_MESSAGE_INIT(data) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_PUT, data)

#define GET_MESSAGE_INIT(data) \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_GET, data)

#define ACK_MESSAGE_INIT() \
    LINKARCH_MESSAGE_INIT(LINKARCH_MESSAGE_TYPE_ACK, "")




#endif  // LINKARCH_PLATFORM_MCU_SIMSOCKET