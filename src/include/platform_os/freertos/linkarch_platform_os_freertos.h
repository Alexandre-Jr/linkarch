#ifndef LINKARCH_PLATFORM_OS_FREERTOS
#define LINKARCH_PLATFORM_OS_FREERTOS




// Standard C libraries
#include <stdint.h>
#include <stdbool.h>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"



// OSAL Defines

#define LINKARCH_MAX_DELAY portMAX_DELAY
#define LINKARCH_INVALID_TASK   NULL
#define LINKARCH_INVALID_MUTEX  NULL




// OSAL types

typedef TaskHandle_t linkarch_taskhandle_t;
typedef SemaphoreHandle_t linkarch_semaphorehandle_t;
typedef QueueHandle_t linkarch_queuehandle_t;
typedef TaskFunction_t linkarch_taskFunction_t;
typedef TickType_t linkarch_tick_t;
typedef UBaseType_t linkarch_ubasetype_t;




/* OSAL functions */

// Tasks functions

bool linkarch_osal_startScheduler(void);
bool linkarch_osal_createTask(linkarch_taskFunction_t taskFunction, const char * const taskName, uint16_t stackSize, void * const parameters, linkarch_ubasetype_t priority, linkarch_taskhandle_t *taskHandle);
void linkarch_osal_setTaskCoreAffinity(linkarch_taskhandle_t taskHandle, uint8_t coreID);


// Queue functions

linkarch_queuehandle_t linkarch_osal_createQueue(uint16_t queueLength, uint16_t itemSize);
bool linkarch_osal_queueSend(linkarch_queuehandle_t queue, const void *item, linkarch_tick_t ticksToWait);
bool linkarch_osal_queueRecieve(linkarch_queuehandle_t queue, void *item, linkarch_tick_t ticksToWait);


// Mutex functions

linkarch_semaphorehandle_t linkarch_osal_createMutex(void);
bool linkarch_osal_takeMutex(linkarch_semaphorehandle_t mutex, linkarch_tick_t ticksToWait);
bool linkarch_osal_giveMutex(linkarch_semaphorehandle_t mutex);


// Time Functions

bool linkarch_osal_delayUntil(linkarch_tick_t *lastWakeTime, linkarch_tick_t period);
linkarch_tick_t linkarch_osal_getTickCount();
linkarch_tick_t linkarch_osal_getMsToTick(uint16_t ms);
uint32_t linkarch_osal_getTickToMs(linkarch_tick_t ticks);
void linkarch_osal_delay(linkarch_tick_t ticks);
void linkarch_osal_delayMs(uint32_t ms);


// User replace functions
void linkarch_osal_aplicationStackOverflowHook();
void linkarch_osal_aplicationMallocFailedHook();
void linkarch_osal_aplicationTickHook();




#endif  // LINKARCH_PLATFORM_OS_FREERTOS