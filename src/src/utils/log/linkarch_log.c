#include "linkarch_log.h"
#include "linkarch_log_private.h"




/* Log functions */

// Initialization and configuration

bool linkarch_log_init(linkarch_log * log, linkarch_writeLogFunction writeFunction, linkarch_openLogFunction openFunction, linkarch_closeLogFunction closeFunction)
{

    log->isLogOpen = false;
    log->writeLogFunction = writeFunction;
    log->openLogFunction = openFunction;
    log->closeLogFunction = closeFunction;
    log->logMutex = linkarch_osal_createMutex();

    return linkarch_log_isLogValid(log);

}


// Log destination functions

bool linkarch_log_open(linkarch_log * log)
{
    
    if (!linkarch_log_isLogValid(log)) return false;

    if (log->isLogOpen) return true;

    if (!linkarch_log_takeLogMutex(log)) return false;

    log->isLogOpen = log->openLogFunction();

    linkarch_log_giveLogMutex(log);

    return log->isLogOpen;

}

bool linkarch_log_close(linkarch_log * log)
{
    
    if (!linkarch_log_isLogValid(log)) return false;

    if (!log->isLogOpen) return true;

    if (!linkarch_log_takeLogMutex(log)) return false;

    log->isLogOpen = !log->closeLogFunction();

    linkarch_log_giveLogMutex(log);

    return !log->isLogOpen;

}


// Log message functions

bool linkarch_log_info(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{

    if (!linkarch_log_isLogValid(log)) return false;

    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_INFO, context, format);

}

bool linkarch_log_warning(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{

    if (!linkarch_log_isLogValid(log)) return false;

    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_WARNING, context, format);

}

bool linkarch_log_error(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{
    
    if (!linkarch_log_isLogValid(log)) return false;
    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_ERROR, context, format);

}

bool linkarch_log_fatal(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{

    if (!linkarch_log_isLogValid(log)) return false;
    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_FATAL, context, format);

}

bool linkarch_log_debug(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{

    if (!linkarch_log_isLogValid(log)) return false;
    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_DEBUG, context, format);

}

bool linkarch_log_trace(linkarch_log * log, linkarch_log_messageContext_t context, const char * format, ...)
{

    if (!linkarch_log_isLogValid(log)) return false;
    return linkarch_log_writeMessage(log, LINKARCH_LOG_SEVERITY_LEVEL_TRACE, context, format);
    
}




/* Private log functions */

// Check functions

bool linkarch_log_isLogValid(const linkarch_log * log)
{

    if (log == NULL) return false;
    if (log->writeLogFunction == NULL) return false;
    if (log->openLogFunction == NULL) return false;
    if (log->closeLogFunction == NULL) return false;
    if (log->logMutex == LINKARCH_INVALID_MUTEX) return false;

    return true;

}

bool linkarch_log_isLogMessageValid(const linkarch_log_message_t * logMessage)
{

    if (logMessage == NULL) return false;
    if (logMessage->message == NULL) return false;
    if (logMessage->context == NULL) return false;

    return true;

}


// Mutex functions

bool linkarch_log_takeLogMutex(linkarch_log * log)
{

    return linkarch_osal_takeMutex(log->logMutex, linkarch_osal_getMsToTick(LINKARCH_LOG_MAX_DELAY_TO_GET_MUTEX));

}

bool linkarch_log_giveLogMutex(linkarch_log * log)
{

    return linkarch_osal_giveMutex(log->logMutex);

}


// Format functions

void linkarch_log_formatLogMessageString(char * buffer, size_t bufferSize, const char * format, ...)
{

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, bufferSize, format, args);
    va_end(args);

}


// Write functions

bool linkarch_log_writeMessage(linkarch_log * log, linkarch_log_severityLevel_t severityLevel, linkarch_log_messageContext_t context, const char * format, ...)
{

    char messageBuffer[LINKARCH_LOG_MAX_MESSAGE_LENGTH];
    linkarch_log_formatLogMessageString(messageBuffer, sizeof(messageBuffer), format);
    
    linkarch_log_message_t logMessage = LINKARCH_LOG_MESSAGE_INIT(messageBuffer, context, severityLevel);

    return linkarch_log_writeLogMessage(log, &logMessage);

}

bool linkarch_log_writeLogMessage(linkarch_log * log, const linkarch_log_message_t * logMessage)
{

    if (!linkarch_log_isLogValid(log)) return false;
    if (!linkarch_log_isLogMessageValid(logMessage)) return false;

    char * severityLevelString = linkarch_log_getSeverityLevelString(logMessage->severityLevel);
    char * timeString = linkarch_log_getTimeString();
    
    char formattedMessage[LINKARCH_LOG_MAX_FORMATTED_MESSAGE_LENGTH];
    snprintf(formattedMessage, sizeof(formattedMessage), "[%s - %s] %s: %s\n", timeString, severityLevelString, logMessage->context, logMessage->message);

    return linkarch_log_write(log, formattedMessage);

}

bool linkarch_log_write(linkarch_log * log, linkarch_log_messageString_t logMessageString)
{

    if (!log->isLogOpen) return false;

    if (!linkarch_log_takeLogMutex(log)) return false;

    bool statusWrite = log->writeLogFunction(logMessageString);

    linkarch_log_giveLogMutex(log);

    return statusWrite;

}


// Get functions

char * linkarch_log_getSeverityLevelString(linkarch_log_severityLevel_t severityLevel)
{

    switch (severityLevel)
    {
        case LINKARCH_LOG_SEVERITY_LEVEL_INFO:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_INFO;
        case LINKARCH_LOG_SEVERITY_LEVEL_WARNING:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_WARNING;
        case LINKARCH_LOG_SEVERITY_LEVEL_ERROR:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_ERROR;
        case LINKARCH_LOG_SEVERITY_LEVEL_FATAL:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_FATAL;
        case LINKARCH_LOG_SEVERITY_LEVEL_DEBUG:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_DEBUG;
        case LINKARCH_LOG_SEVERITY_LEVEL_TRACE:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_TRACE;
        default:
            return LINKARCH_LOG_SEVERITY_LEVEL_STRING_UNKNOWN;
    }

}

char * linkarch_log_getTimeString(void)
{

    uint32_t msSinceStart = linkarch_osal_getTickToMs(linkarch_osal_getTickCount());
    static char timeString[13];
    uint32_t hours = msSinceStart / 3600000;
    uint32_t minutes = (msSinceStart % 3600000) / 60000;
    uint32_t seconds = (msSinceStart % 60000) / 1000;
    uint32_t milliseconds = msSinceStart % 1000;
    snprintf(timeString, sizeof(timeString), "%02u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
    return timeString;

}