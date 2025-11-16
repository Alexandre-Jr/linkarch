#ifndef LINKARCH_LOG
#define LINKARCH_LOG




// Include log parts
#include "linkarch_log_type.h"




/* Log functions */

// Initialization and configuration

bool linkarch_log_init(linkarch_log * log, linkarch_writeLogFunction writeFunction, linkarch_openLogFunction openFunction, linkarch_closeLogFunction closeFunction);


// Log destination functions

bool linkarch_log_open(linkarch_log * log);
bool linkarch_log_close(linkarch_log * log);


// Log message functions

bool linkarch_log_info(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);
bool linkarch_log_warning(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);
bool linkarch_log_error(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);
bool linkarch_log_fatal(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);
bool linkarch_log_debug(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);
bool linkarch_log_trace(linkarch_log * log, linkarch_log_messageContext_t context, const char * message);




#endif // LINKARCH_LOG