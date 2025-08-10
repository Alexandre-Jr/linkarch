#include "linkarch_platform_mcu_simActuator_clientConnection.h"




// Global connection
linkarch_simConnection_t linkarch_simConnection;




/* Simulation Socket */

// Initialization functions

bool linkarch_clientConnection_init()
{

    if (!linkarch_clientConnection_initClientSocket(&linkarch_simConnection.linkarch_socket, LINKARCH_SIM_ACTUATOR_LINKARCHSOCKET_PATH)) return false;
    
    if (!linkarch_clientConnection_initServerSocket(&linkarch_simConnection.linkserver_socket, LINKARCH_SIM_ACTUATOR_LINKSERVERSOCKET_PATH)) {
        close(linkarch_simConnection.linkarch_socket.socket_fd);
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

    linkarch_hal_throwDebugMessage("[linkarch client connection]: Linkarch socket initialized at %s\n", socket_toInit->socket_addr.sun_path);

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

    linkarch_hal_throwDebugMessage("[linkarch client connection]: Linkarch server socket initialized at %s\n", socket_toInit->socket_addr.sun_path);
    
    return true;

}

bool linkarch_clientConnection_initSocketType(linkarch_socket_t * socket_toInit, const char * socket_path)
{

    socket_toInit->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_toInit->socket_fd < 0) return false;

    memset(&socket_toInit->socket_addr, 0, sizeof(socket_toInit->socket_addr));
    socket_toInit->socket_addr.sun_family = AF_UNIX;
    strncpy(socket_toInit->socket_addr.sun_path, socket_path, sizeof(socket_toInit->socket_addr.sun_path) - 1);

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

bool linkarch_clientConnection_sendMessage(linkarch_socket_t * targetSocket, const linkarch_message_t * message)
{

    char * buffer = linkarch_messageToBuffer(message);

    if(send(targetSocket->socket_fd, buffer, message->message_length + LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES, 0) < 0) 
    {
    
        perror("[linkarch client connection]: Failed to send message");
        free(buffer);
        return false;
    
    }

    free(buffer);
    return true;

}

bool linkarch_clientConnection_receiveMessage(linkarch_socket_t * sourceSocket, linkarch_message_t * message)
{

    uint8_t header[LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES];

    ssize_t bytes_received = recv(sourceSocket->socket_fd, header, LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES, 0);
    if (bytes_received <= 0) 
    {
    
        perror("[linkarch client connection]: Failed to receive message header");
        return false;
    
    }

    message->message_type = header[0];
    message->message_length = header[1];

    if (message->message_length < 0) 
    {
    
        message->message_length = 0;
        message->message_data = NULL;
        return true;
    
    }

    message->message_data = (char *) malloc(message->message_length * sizeof(char));
    if (message->message_data == NULL) return false;

    bytes_received = recv(sourceSocket->socket_fd, (void *)message->message_data, message->message_length, 0);
    if (bytes_received <= 0) 
    {
        
        free((void *)message->message_data);
        return false;
        
    }

    return true;

}

bool linkarch_clientConnection_handshake()
{

    linkarch_message_t handshake_message = HANDSHAKE_MESSAGE_INIT(LINKARCH_SIM_ACTUATOR_HANDSHAKE_MESSAGE);

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &handshake_message)) 
    {
    
        linkarch_hal_throwDebugMessage("[linkarch client connection]: Handshake failed to send\n");
        return false;
    
    }

    return true;

}

bool linkarch_clientConnection_sendPUTMessage(const char * data)
{

    linkarch_message_t put_message = PUT_MESSAGE_INIT(data);

    if (!linkarch_clientConnection_sendMessage(&linkarch_simConnection.linkserver_socket, &put_message)) return false;

    return true;
}

bool linkarch_clientConnection_sendGETMessage(const char * data)
{

    linkarch_message_t get_message = GET_MESSAGE_INIT(data);

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

    linkarch_clientConnection_sendACKMessage();
    while(!linkarch_clientConnection_isReadyToRead()) 
    {

        linkarch_clientConnection_sendACKMessage();

    }

    linkarch_message_t message;
    if (!linkarch_clientConnection_receiveMessage(&linkarch_simConnection.linkserver_socket, &message)) 
    {
    
        linkarch_hal_throwDebugMessage("[linkarch client connection]: Failed to receive ACK message\n");
        return false;
    
    }

    linkarch_freeMessage(&message);
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
bool linkarch_clientConnection_PUT(const char * sendData)
{

    if (!linkarch_clientConnection_sendPUTMessage(sendData)) return false;

    return true;

}

// receivedData need to be free after use
bool linkarch_clientConnection_GET(const char * sendData, char ** receivedData)
{

    if (!linkarch_clientConnection_sendGETMessage(sendData)) return false;

    // Timeout
    linkarch_tick_t startTimeTick = linkarch_osal_getTickCount();
    while(!linkarch_clientConnection_isReadyToRead()) 
    {

        if (!linkarch_clientConnection_sendGETMessage(sendData)) return false;
        
        if(linkarch_osal_getTickCount() - startTimeTick >= linkarch_osal_getMsToTick(LINKARCH_MAX_TIME_TO_RECEIVE_RESPONSE_MS)) return false;

    }

    linkarch_message_t message;
    if (!linkarch_clientConnection_receiveMessage(&linkarch_simConnection.linkserver_socket, &message)) 
    {
    
        linkarch_hal_throwDebugMessage("[linkarch client connection]: Failed to receive GET response\n");
        return false;
    
    }

    if (message.message_type != LINKARCH_MESSAGE_TYPE_RES) 
    {
    
        linkarch_hal_throwDebugMessage("[linkarch client connection]: Received unexpected message type: %d\n", message.message_type);
        linkarch_freeMessage(&message);
        return false;
    
    }

    *receivedData = (char *) malloc((message.message_length + 1) * sizeof(char));
    if (*receivedData == NULL) 
    {
    
        perror("[linkarch client connection]: Failed to allocate memory for received data");
        linkarch_freeMessage(&message);
        return false;
    
    }

    strncpy(*receivedData, message.message_data, message.message_length);
    (*receivedData)[message.message_length] = '\0';

    linkarch_freeMessage(&message);
    return true;

}


// Message handling functions (buffer need to be free after use)

char * linkarch_messageToBuffer(const linkarch_message_t * message)
{

    char * buffer = (char *) malloc((message->message_length + LINKARCH_MESSAGE_HEADER_SIZE_IN_BYTES) * sizeof(char));

    if (buffer == NULL) {
        perror("[linkarch client connection]: Failed to allocate memory for message buffer");
        return NULL;
    }

    buffer[0] = message->message_type;
    buffer[1] = message->message_length;

    for(unsigned int i = 0; i < message->message_length; i++) {
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
    
    message->message_length = 0;
    message->message_type = 0;

    return true;

}
