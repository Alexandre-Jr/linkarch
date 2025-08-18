#ifndef LINKARCH_PLATFORM_MCU_CLIENTCOMMAND
#define LINKARCH_PLATFORM_MCU_CLIENTCOMMAND



/* Includes  */

// Client Connection
#include "linkarch_platform_mcu_simActuator_clientConnection.h"

// Command List
#include "linkarch_platform_mcu_simActuator_clientCommandList.h"




/* Defines */

// Command

typedef enum 
{

    PUT_CMD_TYPE = 0,
    GET_CMD_TYPE,
    NUMBER_OF_TYPE

} CommandTypeList;

typedef uint8_t linkarch_command_type_t;

typedef struct 
{

    linkarch_command_id_t cmd_id;
    linkarch_command_type_t cmd_type;
    linkarch_msgDataSize_t cmd_dataSize;
    linkarch_msgData_t cmd_data;

} linkarch_command_t;

// Sizes

#define LINKARCH_BOOL_SIZE 1
#define LINKARCH_UINT8_SIZE 1
#define LINKARCH_UINT16_SIZE 2
#define LINKARCH_UINT32_SIZE 4
#define LINKARCH_UINT64_SIZE 8
#define LINKARCH_FLOAT_SIZE 2





/*  Command Functions */


// User functions

bool linkarch_command_sendPutTypeCommand(const linkarch_command_t * command);

// receivedData and receivedDataSize need to be free after use
bool linkarch_command_sendGetTypeCommand(const linkarch_command_t * command, linkarch_msgData_t * receivedData, linkarch_msgDataSize_t * receivedDataSize);

// Conversion functions

// Message need to be free after use
bool linkarch_command_commandToMessage(const linkarch_command_t * command, linkarch_message_t * message);

uint8_t linkarch_getIntegerPart(float value);

uint8_t linkarch_getDecimalPart(float value);

float linkarch_getFloatFromParts(uint8_t integerPart, uint8_t decimalPart);


/* Macros */

#define COMMAND_INIT(id, type, dataSize, data) \
    { \
        .cmd_id = id, \
        .cmd_type = type, \
        .cmd_dataSize = dataSize,\
        .cmd_data = data \
    }

#define PUT_COMMAND_INIT(id, dataSize, data) \
    COMMAND_INIT(id, PUT_CMD_TYPE, dataSize, (linkarch_msgData_t) data)

#define GET_COMMAND_INIT(id, dataSize, data) \
    COMMAND_INIT(id, GET_CMD_TYPE, dataSize, (linkarch_msgData_t) data)




#endif  // LINKARCH_PLATFORM_MCU_CLIENTCOMMAND