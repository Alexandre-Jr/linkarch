#ifndef LINKARCH_CONTROLLER_SCHEDULER
#define LINKARCH_CONTROLLER_SCHEDULER




// Include controller Parts
#include "linkarch_controller_type.h"




/* Private Squeduler controller functions */


// Tasks Functions

static bool linkarch_controller_scheduler_setTasks();
static bool linkarch_controller_scheduler_startTasks();
static bool linkarch_controller_scheduler_setModuleTask(linkarch_module_id_t moduleID);
static bool linkarch_controller_scheduler_setControllerTask();
static bool linkarch_controller_scheduler_setTasksCoreAffinity();
static bool linkarch_controller_scheduler_setModuleTaskCoreAffinity(linkarch_module_id_t moduleID);
static bool linkarch_controller_scheduler_setControllerCoreAffinity();

// Tasks functions types


static void linkarch_controller_scheduler_moduleTaskFunction_clocked(void *arg);
static void linkarch_controller_scheduler_moduleTaskFunction_unclocked(void *arg);

static void linkarch_controller_scheduler_controllerTaskFunction(void *arg);


// Mutexs functions

static bool linkarch_controller_scheduler_takeAllModuleLoopMutex();
static bool linkarch_controller_scheduler_releaseAllModuleLoopMutex();
static bool linkarch_controller_scheduler_takeModuleLoopMutex(linkarch_module_id_t moduleID);
static bool linkarch_controller_scheduler_releaseModuleLoopMutex(linkarch_module_id_t moduleID);




#endif  // LINKARCH_CONTROLLER_SCHEDULER
