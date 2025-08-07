#ifndef LINKARCH_PLATFORM_OS_POSIX
#define LINKARCH_PLATFORM_OS_POSIX




// Standard C libraries
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




// OSAL Defines
#define LINKARCH_MAX_DELAY      0xffffffff
#define LINKARCH_INVALID_TASK   ((pthread_t)0)
#define LINKARCH_INVALID_MUTEX  NULL

// Local Defines
#define NSEC_PER_MS 1000000
#define LINKARCH_TICK_MS 1




// Types
typedef pthread_t                    linkarch_taskhandle_t;
typedef void (*linkarch_taskFunction_t)(void *);

typedef struct {
    void *buffer;
    size_t itemSize;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;

    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
} linkarch_queue_t;

typedef linkarch_queue_t* linkarch_queuehandle_t;

typedef pthread_mutex_t*            linkarch_semaphorehandle_t;
typedef uint32_t                    linkarch_tick_t;
typedef unsigned int                linkarch_ubasetype_t;




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
void linkarch_osal_delay(linkarch_tick_t ticks);




#endif  // LINKARCH_PLATFORM_OS_POSIX