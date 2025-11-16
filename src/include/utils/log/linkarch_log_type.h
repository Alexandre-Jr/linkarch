#ifndef LINKARCH_LOG_TYPE
#define LINKARCH_LOG_TYPE




// Include OSAL
#include "linkarch_osal.h"

// Include standard libraries
#include <stdbool.h>
#include <stdint.h>




// Log Definitions

#define LINKARCH_LOG_MAX_MESSAGE_LENGTH 128
#define LINKARCH_LOG_MAX_CONTEXT_LENGTH 32
#define LINKARCH_LOG_MAX_SEVERITY_LEVEL_STRING_LENGTH 10
#define LINKARCH_LOG_MAX_FORMATTED_MESSAGE_LENGTH (LINKARCH_LOG_MAX_MESSAGE_LENGTH + LINKARCH_LOG_MAX_CONTEXT_LENGTH + LINKARCH_LOG_MAX_SEVERITY_LEVEL_STRING_LENGTH + 10)

#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_INFO "INFO"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_WARNING "WARNING"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_ERROR "ERROR"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_FATAL "FATAL"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_DEBUG "DEBUG"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_TRACE "TRACE"
#define LINKARCH_LOG_SEVERITY_LEVEL_STRING_UNKNOWN "UNKNOWN"

#define LINKARCH_LOG_MAX_DELAY_TO_GET_MUTEX 1000




// Log type

typedef const char * linkarch_log_messageString_t;
typedef const char * linkarch_log_messageContext_t;
typedef enum
{
    LINKARCH_LOG_SEVERITY_LEVEL_INFO = 0,
    LINKARCH_LOG_SEVERITY_LEVEL_WARNING,
    LINKARCH_LOG_SEVERITY_LEVEL_ERROR,
    LINKARCH_LOG_SEVERITY_LEVEL_FATAL,
    LINKARCH_LOG_SEVERITY_LEVEL_DEBUG,
    LINKARCH_LOG_SEVERITY_LEVEL_TRACE

} linkarch_log_severityLevel_t;

typedef bool (*linkarch_writeLogFunction)(linkarch_log_messageString_t logMessage);
typedef bool (*linkarch_openLogFunction)(void);
typedef bool (*linkarch_closeLogFunction)(void);

typedef struct linkarch_log_message_t
{
    
    linkarch_log_messageString_t message;
    linkarch_log_messageContext_t context;
    linkarch_log_severityLevel_t severityLevel;

} linkarch_log_message_t;

typedef struct linkarch_log
{
    
    bool isLogOpen;
    linkarch_writeLogFunction writeLogFunction;
    linkarch_openLogFunction openLogFunction;
    linkarch_closeLogFunction closeLogFunction;
    linkarch_semaphorehandle_t logMutex;

} linkarch_log;




// Macros

#define LINKARCH_LOG_MESSAGE_INIT(message, context, severityLevel) {message, context, severityLevel}




#endif // LINKARCH_LOG_TYPE