#ifndef LINKARCH_LOG_PRIVATE
#define LINKARCH_LOG_PRIVATE




// Include log parts
#include "linkarch_log_type.h"

// Include standard libraries
#include <stdio.h>
#include <stdarg.h>




/* Private log functions */

// Check functions

bool linkarch_log_isLogValid(const linkarch_log * log);

bool linkarch_log_isLogMessageValid(const linkarch_log_message_t * logMessage);


// Mutex functions

bool linkarch_log_takeLogMutex(linkarch_log * log);

bool linkarch_log_giveLogMutex(linkarch_log * log);


// Write functions

bool linkarch_log_writeMessage(linkarch_log * log, linkarch_log_severityLevel_t severityLevel, linkarch_log_messageContext_t context, const char * message);

bool linkarch_log_writeLogMessage(linkarch_log * log, const linkarch_log_message_t * logMessage);

bool linkarch_log_write(linkarch_log * log, linkarch_log_messageString_t logMessageString);


// Get functions

char * linkarch_log_getSeverityLevelString(linkarch_log_severityLevel_t severityLevel);

char * linkarch_log_getTimeString(void);




#endif // LINKARCH_LOG_PRIVATE