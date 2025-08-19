#include "linkarch_platform_os_freertos.h"




/* OSAL functions */

// Tasks functions

bool linkarch_osal_startScheduler(void)
{
 
    vTaskStartScheduler();
    return true;

}

bool linkarch_osal_createTask(linkarch_taskFunction_t taskFunction, const char * const taskName, uint16_t stackSize, void * const parameters, linkarch_ubasetype_t priority, linkarch_taskhandle_t *taskHandle)
{
 
    return xTaskCreate(taskFunction, taskName, stackSize, parameters, priority, taskHandle) == pdPASS;

}

void linkarch_osal_setTaskCoreAffinity(linkarch_taskhandle_t taskHandle, uint8_t coreID)
{

    if(!configUSE_CORE_AFFINITY) return;
    if(coreID >= configNUMBER_OF_CORES) return;

    vTaskCoreAffinitySet(taskHandle, (1 << coreID));

}



// Queue functions

linkarch_queuehandle_t linkarch_osal_createQueue(uint16_t queueLength, uint16_t itemSize)
{
 
    return xQueueCreate(queueLength, itemSize);

}

bool linkarch_osal_queueSend(linkarch_queuehandle_t queue, const void *item, linkarch_tick_t ticksToWait)
{
 
    return xQueueSend(queue, item, ticksToWait) == pdTRUE;

}

bool linkarch_osal_queueRecieve(linkarch_queuehandle_t queue, void *item, linkarch_tick_t ticksToWait)
{
 
    return xQueueReceive(queue, item, ticksToWait) == pdTRUE;

}


// Mutex functions

linkarch_semaphorehandle_t linkarch_osal_createMutex(void)
{
 
    return xSemaphoreCreateMutex();

}

bool linkarch_osal_takeMutex(linkarch_semaphorehandle_t mutex, linkarch_tick_t ticksToWait)
{
 
    return xSemaphoreTake(mutex, ticksToWait) == pdTRUE;

}

bool linkarch_osal_giveMutex(linkarch_semaphorehandle_t mutex)
{
 
    return xSemaphoreGive(mutex) == pdTRUE;

}


// Time Functions

bool linkarch_osal_delayUntil(linkarch_tick_t *lastWakeTime, linkarch_tick_t period)
{
 
    return xTaskDelayUntil(lastWakeTime, period) == pdTRUE;

}

linkarch_tick_t linkarch_osal_getTickCount()
{

    return xTaskGetTickCount();

}

linkarch_tick_t linkarch_osal_getMsToTick(uint16_t ms)
{

    return pdMS_TO_TICKS(ms);

}

void linkarch_osal_delay(linkarch_tick_t ticks)
{

    vTaskDelay(ticks);

}

void linkarch_osal_delayMs(uint32_t ms)
{

    vTaskDelay(pdMS_TO_TICKS(ms));

}

// Standard freeRTOS functions

void vApplicationMallocFailedHook(void)
{

    linkarch_osal_aplicationMallocFailedHook();

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{

    (void)xTask;
    (void)pcTaskName;

    linkarch_osal_aplicationStackOverflowHook();

}

void vApplicationTickHook(void)
{

    linkarch_osal_aplicationTickHook();

}