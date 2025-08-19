#include "linkarch_platform_mcu_simActuator_clientConnection.h"




// Global connection
linkarch_clientConnection_t linkarch_simConnection;




/* Simulation Socket */

// Initialization functions

bool linkarch_clientConnection_init()
{

    if (!linkarch_clientConnection_initClientSocket(&linkarch_simConnection.linkarch_socket, LINKARCH_SIM_ACTUATOR_LINKARCHSOCKET_PATH)) return false;
    
    if (!linkarch_clientConnection_initServerSocket(&linkarch_simConnection.linkserver_socket, LINKARCH_SIM_ACTUATOR_LINKSERVERSOCKET_PATH)) {
        close(linkarch_simConnection.linkarch_socket.socket_fd);
        return false;
    } 

    linkarch_simConnection.sendConnection_mutex = linkarch_osal_createMutex();
    if(linkarch_simConnection.sendConnection_mutex == LINKARCH_INVALID_MUTEX) {
        close(linkarch_simConnection.linkarch_socket.socket_fd);
        close(linkarch_simConnection.linkserver_socket.socket_fd);
        return false;
    }

    linkarch_simConnection.receiveConnection_mutex = linkarch_osal_createMutex();
    if(linkarch_simConnection.receiveConnection_mutex == LINKARCH_INVALID_MUTEX) {
        close(linkarch_simConnection.linkarch_socket.socket_fd);
        close(linkarch_simConnection.linkserver_socket.socket_fd);
        return false;
    }

    if (!linkarch_clientConnection_handshake()) {
        close(linkarch_simConnection.linkarch_socket.socket_fd);
        close(linkarch_simConnection.linkserver_socket.socket_fd);
        return false;
    }

    return true;
}

bool linkarch_clientConnection_initClientSocket(linkarch_socket_t * socket_toInit, const char * socket_path)
{

    linkarch_clientConnection_initSocketType(socket_toInit, socket_path);

    unlink(socket_toInit->socket_addr.sun_path);

    if (!linkarch_clientConnection_clientSocketBind(socket_toInit)) 
    {
        
        perror("[linkarch client connection]: Failed to bind linkarch socket");

        return false;

    }

    LINKARCH_MESSAGE("[linkarch client connection]: Linkarch socket initialized at %s", socket_toInit->socket_addr.sun_path);

    return true;

}

bool linkarch_clientConnection_initServerSocket(linkarch_socket_t * socket_toInit, const char * socket_path)
{

    linkarch_clientConnection_initSocketType(socket_toInit, socket_path);

    if(!linkarch_clientConnection_serverSocketConnect(socket_toInit)) 
    {

        perror("[linkarch client connection]: Failed to connect to linkarch server socket");

        return false;

    }

    LINKARCH_MESSAGE("[linkarch client connection]: Linkarch server socket initialized at %s", socket_toInit->socket_addr.sun_path);
    
    return true;

}

bool linkarch_clientConnection_initSocketType(linkarch_socket_t * socket_toInit, const char * socket_path)
{

    socket_toInit->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_toInit->socket_fd < 0) return false;

    memset(&socket_toInit->socket_addr, 0, sizeof(socket_toInit->socket_addr));
    socket_toInit->socket_addr.sun_family = AF_UNIX;
    strncpy(socket_toInit->socket_addr.sun_path, socket_path, sizeof(socket_toInit->socket_addr.sun_path) - 1);

    return true;
    
}

bool linkarch_clientConnection_clientSocketBind(linkarch_socket_t * socket_toInit)
{

    if (bind(socket_toInit->socket_fd, (struct sockaddr *)&socket_toInit->socket_addr, sizeof(socket_toInit->socket_addr)) < 0) 
    {
    
        close(socket_toInit->socket_fd);
        return false;
    
    }

    return true;

}

bool linkarch_clientConnection_serverSocketConnect(linkarch_socket_t * socket_toInit)
{

    if (connect(socket_toInit->socket_fd, (struct sockaddr *)&socket_toInit->socket_addr, sizeof(socket_toInit->socket_addr)) < 0) 
    {
        
        close(socket_toInit->socket_fd);
        return false;

    }

    return true;

}


// Send and receive functions

bool linkarch_clientConnection_unsafeSendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message)
{

    linkarch_msgData_t buffer = linkarch_messageToBuffer(message);

    if (send(targetSocket->socket_fd, buffer, message->message_dataSize + LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES, 0) < 0) 
    {
    
        perror("[linkarch client connection]: Failed to send message");
        free(buffer);
        return false;
    
    }

    free(buffer);
    return true;

}

bool linkarch_clientConnection_sendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message)
{

    if(!linkarch_clientConnection_takeSendConnectionMutex()) return false;

    if(!linkarch_clientConnection_unsafeSendMessage(targetSocket, message)) 
    {
    
        linkarch_clientConnection_giveSendConnectionMutex();
        return false;
    
    }

    linkarch_clientConnection_waitForAck();

    linkarch_clientConnection_giveSendConnectionMutex();

    return true;

}

bool linkarch_clientConnection_unsafeReceiveMessage(linkarch_socket_t * sourceSocket, linkarch_message_t * message)
{

    uint8_t header[LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES];

    if(linkarch_clientConnection_isReadyToRead() == false) 
    {
    
        return false;
    
    }

    ssize_t bytes_received = recv(sourceSocket->socket_fd, header, LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES, 0);
    if (bytes_received <= 0) 
    {
    
        perror("[linkarch client connection]: Failed to receive message header");

        return false;
    
    }
    
    message->message_type = header[0];
    if( message->message_type >= LINKARCH_NUMBER_OF_MESSAGE_TYPES) 
    {

        linkarch_clientConnection_clearSocketBuffer(sourceSocket);
        
        return false;
    
    }

    message->message_dataSize = header[1];

    if (message->message_dataSize == 0) 
    {
    
        message->message_dataSize = 0;
        message->message_data = NULL;

        return true;
    
    }

    message->message_data = (linkarch_msgData_t) malloc(message->message_dataSize * sizeof(linkarch_msgDataPart_t));
    if (message->message_data == NULL) 
    {

        return false;

    }

    bytes_received = recv(sourceSocket->socket_fd, (void *)message->message_data, message->message_dataSize, 0);
    if (bytes_received <= 0) 
    {
        
        free((void *)message->message_data);

        return false;
        
    }

    return true;

}

bool linkarch_clientConnection_receiveMessage(linkarch_socket_t * sourceSocket, linkarch_message_t * message)
{

    linkarch_clientConnection_takeReceiveConnectionMutex();

    if(!linkarch_clientConnection_unsafeReceiveMessage(sourceSocket, message)) 
    {

        linkarch_clientConnection_giveReceiveConnectionMutex();
        return false;

    }

    linkarch_clientConnection_giveReceiveConnectionMutex();
    return true;

}

bool linkarch_clientConnection_handshake()
{

    linkarch_message_t handshake_message = HANDSHAKE_MESSAGE_INIT(LINKARCH_SIM_ACTUATOR_HANDSHAKE_MESSAGE, LINKARCH_SIM_ACTUATOR_HANDSHAKE_MESSAGE_SIZE);

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &handshake_message)) return false;

    return true;

}

bool linkarch_clientConnection_sendPUTMessage(linkarch_msgData_t data, linkarch_msgDataSize_t dataSize)
{

    linkarch_message_t put_message = PUT_MESSAGE_INIT(data, dataSize);

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &put_message)) return false;

    return true;
}

bool linkarch_clientConnection_sendGETMessage(linkarch_msgData_t data, linkarch_msgDataSize_t dataSize)
{

    linkarch_message_t get_message = GET_MESSAGE_INIT(data, dataSize);

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &get_message)) return false;

    return true;

}

bool linkarch_clientConnection_sendACKMessage()
{

    linkarch_message_t ack_message = ACK_MESSAGE_INIT();

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &ack_message)) return false;

    return true;

}


// Status functions

bool linkarch_clientConnection_isConnected()
{

    if (linkarch_simConnection.linkserver_socket.socket_fd < 0) return false;

    if(!linkarch_clientConnection_sendACKMessage()) return false;    

    return true;

}

bool linkarch_clientConnection_isReadyToRead()
{

    fd_set read_fds;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_SET(linkarch_simConnection.linkserver_socket.socket_fd, &read_fds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    int result = select(linkarch_simConnection.linkserver_socket.socket_fd + 1, &read_fds, NULL, NULL, &timeout);

    if (result < 0) return false;
    return (result > 0 && FD_ISSET(linkarch_simConnection.linkserver_socket.socket_fd, &read_fds));

}


// User functions
bool linkarch_clientConnection_PUT(linkarch_msgData_t sendData, linkarch_msgDataSize_t dataSize)
{

    if (!linkarch_clientConnection_sendPUTMessage(sendData, dataSize)) return false;
    
    return true;

}

// receivedData need to be free after use
bool linkarch_clientConnection_GET(linkarch_msgData_t sendData, linkarch_msgDataSize_t sendDataSize, linkarch_msgData_t * receivedData, linkarch_msgDataSize_t * receivedDataSize)
{

    if(!linkarch_clientConnection_sendGETMessage(sendData, sendDataSize)) return false;
    
    // Timeout
    
    bool responseReceived = false;
    for (uint8_t numberOfRetransmission = 0; numberOfRetransmission < LINKARCH_MAX_NUMBER_OF_RETRANSMISSIONS; numberOfRetransmission++) 
    {

        linkarch_clientConnection_waitForResponse();

        if(linkarch_clientConnection_isReadyToRead()) responseReceived = true;

        if(responseReceived) break;

        if (!linkarch_clientConnection_sendGETMessage(sendData, sendDataSize)) return false;

    }
    
    if(!responseReceived) 
    {
    
        LINKARCH_MESSAGE("[linkarch client connection]: No response received after %d retransmissions", LINKARCH_MAX_NUMBER_OF_RETRANSMISSIONS);
        return false;
    
    }

    linkarch_message_t message;
    if (!linkarch_clientConnection_receiveMessage(&linkarch_simConnection.linkserver_socket, &message)) 
    {
    
        LINKARCH_MESSAGE("[linkarch client connection]: Failed to receive GET response");
        return false;
    
    }
    receivedDataSize[0] = message.message_dataSize;

    if (message.message_type != LINKARCH_MESSAGE_TYPE_RES) 
    {
    
        LINKARCH_MESSAGE("[linkarch client connection]: Received unexpected message type: %d", message.message_type);
        linkarch_freeMessage(&message);
        return false;
    
    }

    *receivedData = (linkarch_msgData_t) malloc((message.message_dataSize) * sizeof(linkarch_msgDataPart_t));
    if (*receivedData == NULL) 
    {
    
        perror("[linkarch client connection]: Failed to allocate memory for received data");
        linkarch_freeMessage(&message);
        return false;
    
    }

    for(unsigned int i = 0; i < message.message_dataSize; i++) 
    {

        (*receivedData)[i] = message.message_data[i];
    
    }

    linkarch_freeMessage(&message);
    return true;

}


// Message handling functions (buffer need to be free after use)

linkarch_msgData_t linkarch_messageToBuffer(const linkarch_message_t * message)
{

    linkarch_msgData_t buffer = (linkarch_msgData_t) malloc((message->message_dataSize + LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES) * sizeof(linkarch_msgDataPart_t));

    if (buffer == NULL) return NULL;

    buffer[0] = message->message_type;
    buffer[1] = message->message_dataSize;

    for(unsigned int i = 0; i < message->message_dataSize; i++) {
        buffer[i + LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES] = message->message_data[i];
    }

    return buffer;

}

bool linkarch_freeMessage(linkarch_message_t * message)
{

    if (message->message_data != NULL) {
        free((void *)message->message_data);
        message->message_data = NULL;
    }
    
    message->message_dataSize = 0;
    message->message_type = 0;

    return true;

}


// Mutex functions

bool linkarch_clientConnection_takeSendConnectionMutex()
{
    
    return linkarch_osal_takeMutex(linkarch_simConnection.sendConnection_mutex, LINKARCH_MAX_TIME_TO_GET_MUTEX_MS);

}

bool linkarch_clientConnection_giveSendConnectionMutex()
{

    return linkarch_osal_giveMutex(linkarch_simConnection.sendConnection_mutex);

}

bool linkarch_clientConnection_takeReceiveConnectionMutex()
{

    return linkarch_osal_takeMutex(linkarch_simConnection.receiveConnection_mutex, LINKARCH_MAX_TIME_TO_GET_MUTEX_MS);

}

bool linkarch_clientConnection_giveReceiveConnectionMutex()
{

    return linkarch_osal_giveMutex(linkarch_simConnection.receiveConnection_mutex);

}

// Socket Buffer functions

bool linkarch_clientConnection_clearSocketBuffer(linkarch_socket_t * socket_toClear)
{

    char buffer[1024];
    ssize_t bytes_received;

    while(linkarch_clientConnection_isReadyToRead()) 
    {

        bytes_received = recv(socket_toClear->socket_fd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) break;
    }

    return true;

}


// Waiting functions

bool linkarch_clientConnection_waitForResponse()
{

    linkarch_tick_t startTimeTick = linkarch_osal_getTickCount();

    while(!linkarch_clientConnection_isReadyToRead()) 
    {
        
        if(linkarch_osal_getTickCount() - startTimeTick >= linkarch_osal_getMsToTick(LINKARCH_MAX_TIME_TO_RECEIVE_RESPONSE_MS)) break;

    }

    return true;

}
bool linkarch_clientConnection_waitForAck()
{

    if(!linkarch_clientConnection_waitForResponse()) return false;
    if(!linkarch_clientConnection_isReadyToRead()) return false;

    linkarch_message_t message;

    if(!linkarch_clientConnection_receiveMessage(&linkarch_simConnection.linkserver_socket, &message)) return false;

    if(message.message_type != LINKARCH_MESSAGE_TYPE_ACK) 
    {
    
        linkarch_freeMessage(&message);
        return false;
    
    }

    linkarch_freeMessage(&message);
    return true;

}
