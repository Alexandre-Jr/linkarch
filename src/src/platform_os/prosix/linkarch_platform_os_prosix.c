#include "linkarch_platform_os_prosix.h"




/* OSAL functions */

// Tasks functions

bool linkarch_osal_createTask(linkarch_taskFunction_t taskFunction, const char * const taskName,
                              uint16_t stackSize, void * const parameters,
                              linkarch_ubasetype_t priority, linkarch_taskhandle_t *taskHandle)
{
    (void)taskName;
    (void)stackSize;
    (void)priority;
    
    linkarch_taskhandle_t * newTaskHandle_ptr;
    linkarch_taskhandle_t newTaskHandle;    
    if (taskHandle == NULL) newTaskHandle_ptr = &newTaskHandle;
    else newTaskHandle_ptr = taskHandle;

    return pthread_create(taskHandle, NULL, (void*(*)(void*)) taskFunction, parameters) == 0;
}

bool linkarch_osal_startScheduler(void)
{
    return true;
}

void linkarch_osal_setTaskCoreAffinity(linkarch_taskhandle_t taskHandle, uint8_t coreID)
{
    (void)taskHandle;
    (void)coreID;
}


// Queue functions

static void getTimeoutTime(struct timespec *ts, uint32_t msTimeout)
{
    clock_gettime(CLOCK_REALTIME, ts);
    ts->tv_sec += msTimeout / 1000;
    ts->tv_nsec += (msTimeout % 1000) * 1000000;

    if (ts->tv_nsec >= 1000000000L) {
        ts->tv_sec += 1;
        ts->tv_nsec -= 1000000000L;
    }
}

linkarch_queuehandle_t linkarch_osal_createQueue(uint16_t queueLength, uint16_t itemSize)
{
    linkarch_queue_t *q = malloc(sizeof(linkarch_queue_t));
    if (!q) return NULL;

    q->buffer = malloc(queueLength * itemSize);
    if (!q->buffer) {
        free(q);
        return NULL;
    }

    q->itemSize = itemSize;
    q->capacity = queueLength;
    q->head = q->tail = q->count = 0;

    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->notEmpty, NULL);
    pthread_cond_init(&q->notFull, NULL);

    return q;
}

bool linkarch_osal_queueSend(linkarch_queuehandle_t q, const void *item, uint32_t timeout_ms)
{
    struct timespec ts;
    getTimeoutTime(&ts, timeout_ms);

    pthread_mutex_lock(&q->mutex);

    while (q->count == q->capacity) {
        if (pthread_cond_timedwait(&q->notFull, &q->mutex, &ts) == ETIMEDOUT) {
            pthread_mutex_unlock(&q->mutex);
            return false;
        }
    }

    memcpy((char*)q->buffer + (q->tail * q->itemSize), item, q->itemSize);
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;

    pthread_cond_signal(&q->notEmpty);
    pthread_mutex_unlock(&q->mutex);
    return true;
}

bool linkarch_osal_queueRecieve(linkarch_queuehandle_t q, void *item, uint32_t timeout_ms)
{
    struct timespec ts;
    getTimeoutTime(&ts, timeout_ms);

    pthread_mutex_lock(&q->mutex);

    while (q->count == 0) {
        if (pthread_cond_timedwait(&q->notEmpty, &q->mutex, &ts) == ETIMEDOUT) {
            pthread_mutex_unlock(&q->mutex);
            return false;
        }
    }

    memcpy(item, (char*)q->buffer + (q->head * q->itemSize), q->itemSize);
    q->head = (q->head + 1) % q->capacity;
    q->count--;

    pthread_cond_signal(&q->notFull);
    pthread_mutex_unlock(&q->mutex);
    return true;
}


// Mutex functions

linkarch_semaphorehandle_t linkarch_osal_createMutex(void)
{
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    if (mutex && pthread_mutex_init(mutex, NULL) == 0)
        return mutex;

    free(mutex);
    return NULL;
}

bool linkarch_osal_takeMutex(linkarch_semaphorehandle_t mutex, linkarch_tick_t ticksToWait)
{
    (void)ticksToWait;
    return pthread_mutex_lock(mutex) == 0;
}

bool linkarch_osal_giveMutex(linkarch_semaphorehandle_t mutex)
{
    return pthread_mutex_unlock(mutex) == 0;
}


// Time Functions

bool linkarch_osal_delayUntil(linkarch_tick_t *lastWakeTime, linkarch_tick_t period)
{
    linkarch_tick_t current = linkarch_osal_getTickCount();
    linkarch_tick_t next = *lastWakeTime + period;

    if (next > current)
        usleep((next - current) * 1000 / LINKARCH_TICK_MS);

    *lastWakeTime = linkarch_osal_getTickCount();
    return true;
}

linkarch_tick_t linkarch_osal_getTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (linkarch_tick_t)((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}

linkarch_tick_t linkarch_osal_getMsToTick(uint16_t ms)
{
    return (linkarch_tick_t) LINKARCH_TICK_MS * ms;
}