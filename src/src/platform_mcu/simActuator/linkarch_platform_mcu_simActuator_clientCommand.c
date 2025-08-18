#include "linkarch_platform_mcu_simActuator_clientCommand.h"




/*  Command Functions */


// User functions

bool linkarch_command_sendPutTypeCommand(const linkarch_command_t * command)
{

    linkarch_message_t put_message;

    if (!linkarch_command_commandToMessage(command, &put_message)) return false;

    if (!linkarch_clientConnection_PUT(put_message.message_data, put_message.message_dataSize)) 
    {

        linkarch_freeMessage(&put_message);
        return false;

    }

    linkarch_freeMessage(&put_message);
    return true;

}

// receivedData and receivedDataSize need to be free after use
bool linkarch_command_sendGetTypeCommand(const linkarch_command_t * command, linkarch_msgData_t * receivedData, linkarch_msgDataSize_t * receivedDataSize)
{

    linkarch_message_t get_message;

    if (!linkarch_command_commandToMessage(command, &get_message)) return false;

    if (!linkarch_clientConnection_GET(get_message.message_data, get_message.message_dataSize, receivedData, receivedDataSize)) return false;

    linkarch_freeMessage(&get_message);
    return true;

}

// Conversion functions

// Message need to be free after use
bool linkarch_command_commandToMessage(const linkarch_command_t * command, linkarch_message_t * message)
{

    linkarch_message_t temp_message = LINKARCH_MESSAGE_INIT(
        (command->cmd_type == PUT_CMD_TYPE) ? LINKARCH_MESSAGE_TYPE_PUT : LINKARCH_MESSAGE_TYPE_GET,
        NULL,
        command->cmd_dataSize + 1
    );

    if (command->cmd_dataSize <= 0) 
    {

        temp_message.message_data = NULL;
        *message = temp_message;
        return true;

    }

    temp_message.message_data = (linkarch_msgData_t) malloc((command->cmd_dataSize + 1) * sizeof(linkarch_msgDataPart_t));

    if (temp_message.message_data == NULL) return false;

    temp_message.message_data[0] = command->cmd_id;

    for(linkarch_msgDataSize_t i = 1; i < command->cmd_dataSize + 1; i++) 
    {

        temp_message.message_data[i] = command->cmd_data[i - 1];

    }

    *message = temp_message;
    return true;

}

uint8_t linkarch_getIntegerPart(float value)
{

    return (uint8_t)value;

}

uint8_t linkarch_getDecimalPart(float value)
{

    return (uint8_t)((value - (uint8_t)value) * 100 + 0.5);

} 