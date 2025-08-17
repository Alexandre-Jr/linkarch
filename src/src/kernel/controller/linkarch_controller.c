#include "linkarch_controller.h"
#include "linkarch_controller_private.h"


/* Global variable */
fsm_state_t controllerFSM_listOfStates[LINKARCH_CONTROLLER_NUMBER_OF_MODES];
fsm_event_t controllerFSM_listOfEvents[LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS];
fsm_t controllerFSM = FSM_INIT(controllerFSM_listOfStates, controllerFSM_listOfEvents, 0, LINKARCH_CONTROLLER_NUMBER_OF_MODES, LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS); 
linkarch_controller_t * linkarch_controller = INVALID_CONTROLLER;

#if (LINKARCH_DEBUG_MODE)
linkarch_semaphorehandle_t linkarch_controller_debugMessageMutex;
#endif


/* Controller public functions */

// User functions

bool linkarch_controller_config(linkarch_controller_t * controller, linkarch_mode_id_t initialModeID)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;
    linkarch_controller = controller;

    if(!linkarch_controller_isModeIDValidInTheController(controller, initialModeID)) return false;

    linkarch_setGlobalFSMToControllerFSM(controller, initialModeID);

    if(!linkarch_controller_configModeTrasitionQueue(controller)) return false;

    if(!linkarch_controller_configModulesMutex(controller)) return false;

    #if (LINKARCH_DEBUG_MODE)
    linkarch_controller_debug_setDebugMessageMutex();
    #endif

    LINKARCH_DEBUG_MESSAGE("[linkarch controller]: Controller configured successfully");

    return true;

}

bool linkarch_controller_startSystem()
{

    if (!linkarch_controller_isControllerValid(linkarch_controller)) return false;

    if (!linkarch_controller_configModules(linkarch_controller)) return false; 

    if (!linkarch_controller_scheduler_setTasks()) return false;

    if (!linkarch_controller_scheduler_setTasksCoreAffinity()) return false;

    LINKARCH_DEBUG_MESSAGE("[linkarch_controller]: Controller tasks configured successfully");

    return linkarch_controller_scheduler_startTasks();
    
}

bool linkarch_controller_requestHandleModeTransition(linkarch_modeTransition_id_t modeTrasitionID)
{

    LINKARCH_DEBUG_FUNCTION(linkarch_controller_debug_modeTransitionRequestMessage(modeTrasitionID));

    if (!linkarch_controller_isControllerValid(linkarch_controller)) return false;

    if (!linkarch_controller_isModeTrasitionValidInTheController(linkarch_controller, modeTrasitionID)) return false;

    if (!linkarch_osal_queueSend(linkarch_controller->modeTransition_queue, &linkarch_controller->listOfModeTransitions[modeTrasitionID], LINKARCH_MAX_DELAY)) return false;    

    return true;

}

bool linkarch_controller_isTheModeChanged(linkarch_mode_id_t localActualModeID)
{

    if (!linkarch_controller_isControllerValid(linkarch_controller)) return false;

    if (!linkarch_controller_isControllerValid(linkarch_controller)) return false;

    return (localActualModeID != linkarch_controller_getActualModeID());

}



/* Private controller functions */


// FSM functions

static bool linkarch_controller_handleModeTransition(linkarch_modeTransition_id_t modeTrasitionID)
{
    
    if (!linkarch_controller_isModeTrasitionValidInTheController(linkarch_controller, modeTrasitionID)) return false;

    bool statusHandleEvent = linkarch_fsm_handleEvent(linkarch_controller->linkarch_controller_fsm, modeTrasitionID);

    LINKARCH_DEBUG_FUNCTION(linkarch_controller_debug_modeTransitionMessage(statusHandleEvent, modeTrasitionID));

    return statusHandleEvent;

}


// Config functions

static void linkarch_setGlobalFSMToControllerFSM(linkarch_controller_t * controller, linkarch_mode_id_t initialModeID)
{

    if (!linkarch_controller_isControllerValid(controller)) return;

    if (!linkarch_controller_isModeIDValidInTheController(controller, initialModeID)) return;


    controller->linkarch_controller_fsm = &controllerFSM;
    controller->linkarch_controller_fsm->actualStateID = initialModeID;

    for(uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODES; i++) 
    {
    
        controllerFSM.listOfStates[i].fsm_state_id = controller->listOfModes[i].mode_id;
        strcpy(controllerFSM.listOfStates[i].fsm_state_name, controller->listOfModes[i].mode_name);
    
    }

    controllerFSM.numberOfStates = LINKARCH_CONTROLLER_NUMBER_OF_MODES;

    for(uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS; i++) 
    {

        controllerFSM.listOfEvents[i].fsm_event_id = controller->listOfModeTransitions[i].modeTransition_id;
        strcpy(controllerFSM.listOfEvents[i].fsm_event_name, controller->listOfModeTransitions[i].modeTransition_name);
        controllerFSM.listOfEvents[i].sourceStateId = controller->listOfModeTransitions[i].sourceModeID;
        controllerFSM.listOfEvents[i].targetStateId = controller->listOfModeTransitions[i].targetModeID;
        controllerFSM.listOfEvents[i].beforeTransitionAction = controller->listOfModeTransitions[i].startTransitionFunction;
        controllerFSM.listOfEvents[i].afterTransitionAction = controller->listOfModeTransitions[i].endTransitionFunction;

    }

}

static bool linkarch_controller_configModules(const linkarch_controller_t * controller)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {
    
        if (!linkarch_controller_configModule(controller, i)) return false;
    
    }

    return true;

}

static bool linkarch_controller_configModule(const linkarch_controller_t * controller, linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    linkarch_module_t * module = linkarch_controller_getModuleByID(controller, moduleID);
    if (module == INVALID_MODULE) return false;
    if (module->startModuleConfig == NULL) return false;

    return module->startModuleConfig();

}

static bool linkarch_controller_configModeTrasitionQueue(linkarch_controller_t * controller)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    controller->modeTransition_queue = linkarch_osal_createQueue(1, sizeof(linkarch_modeTransition_id_t));

    return true;

}

static bool linkarch_controller_configModulesMutex(linkarch_controller_t * controller)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    for(uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {
    
        linkarch_module_t * module = linkarch_controller_getModuleByID(controller, i);
        if (module == INVALID_MODULE) return false;

        module->loopModuleSemaphore = linkarch_osal_createMutex();
        if (module->loopModuleSemaphore == LINKARCH_INVALID_MUTEX) return false;

    }

    return true;

}


// Get functions

static linkarch_mode_id_t linkarch_controller_getActualModeID()
{

    if (!linkarch_controller_isControllerValid(linkarch_controller)) return INVALID_MODE_ID;

    linkarch_mode_t * actualMode = linkarch_controller_getActualMode(linkarch_controller);
    return actualMode->mode_id;

}

static linkarch_module_t * linkarch_controller_getModuleByID(const linkarch_controller_t * controller, linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isModuleIDValidInTheController(controller, moduleID)) return INVALID_MODULE;
    return &controller->listOfModules[moduleID];

}

static linkarch_mode_t * linkarch_controller_getModeByID(const linkarch_controller_t * controller, linkarch_mode_id_t modeID)
{

    if (!linkarch_controller_isModeIDValidInTheController(controller, modeID)) return INVALID_MODE;
    return &controller->listOfModes[modeID];

}

static linkarch_mode_t * linkarch_controller_getActualMode(const linkarch_controller_t * controller)
{

    if (!linkarch_controller_isControllerValid(controller)) return INVALID_MODE;

    return linkarch_controller_getModeByID(controller, controller->linkarch_controller_fsm->actualStateID);
}


// Validation functions

static bool linkarch_controller_isControllerValid(const linkarch_controller_t * controller)
{

    return (controller != INVALID_CONTROLLER);

}

static bool linkarch_controller_isModeIDValidInTheController(const linkarch_controller_t * controller, linkarch_mode_id_t modeID)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    if (modeID >= LINKARCH_CONTROLLER_NUMBER_OF_MODES) return false;
    return true;

}

static bool linkarch_controller_isModuleIDValidInTheController(const linkarch_controller_t * controller, linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isControllerValid(controller)) return false;

    if (moduleID >= LINKARCH_CONTROLLER_NUMBER_OF_MODULES) return false;
    return true;

}

static bool linkarch_controller_isModeTrasitionValidInTheController(const linkarch_controller_t * controller, linkarch_modeTransition_id_t modeTrasitionID)
{

    return (linkarch_controller_isControllerValid(controller) && modeTrasitionID < LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS);

}


// Module functions

static void linkarch_controller_setupModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode)  
{
    
    if (mode == INVALID_MODULE || mode->listOfModeConfigToModule == NULL) return;
    linkarch_controller_executeModeFunction(mode->listOfModeConfigToModule[mooduleID].setup_function);

}

static void linkarch_controller_loopModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode)
{

    if (mode == INVALID_MODULE || mode->listOfModeConfigToModule == NULL) return;
    linkarch_controller_executeModeFunction(mode->listOfModeConfigToModule[mooduleID].loop_function);

}

static void linkarch_controller_shutdownModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode)
{

    if (mode == INVALID_MODULE || mode->listOfModeConfigToModule == NULL) return;
    linkarch_controller_executeModeFunction(mode->listOfModeConfigToModule[mooduleID].shutdown_function);

}

static void linkarch_controller_executeModeFunction(modeFunctions function)
{

    if (function == NULL) return;
    function();

}


// Tasks Functions

static bool linkarch_controller_scheduler_setTasks()
{

    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {

        if (!linkarch_controller_scheduler_setModuleTask(i)) return false;

    }

    if(!linkarch_controller_scheduler_setControllerTask()) return false;

    return true;

}

static bool linkarch_controller_scheduler_startTasks()
{
    return linkarch_osal_startScheduler();

}

static bool linkarch_controller_scheduler_setModuleTask(linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isModuleIDValidInTheController(linkarch_controller, moduleID)) return false;
    linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, moduleID);
    

    switch (module->moduleType)
    {
    case LINKARCH_MODULETYPE_LOOP_CLOCKED:
        if (linkarch_osal_createTask(linkarch_controller_scheduler_moduleTaskFunction_clocked, module->module_name, module->moduleStackSize, (void*)&module->module_id, module->modulePriority, &module->moduleTaskHandle)) return true;
        break;
    case LINKARCH_MODULETYPE_LOOP_UNCLOCKED:
        if (linkarch_osal_createTask(linkarch_controller_scheduler_moduleTaskFunction_unclocked, module->module_name, module->moduleStackSize, (void*)&module->module_id, module->modulePriority, &module->moduleTaskHandle)) return true;
        break;
    default:
        return false;
        break;
    }

    return false;

}

static bool linkarch_controller_scheduler_setControllerTask()
{

    if (linkarch_osal_createTask(linkarch_controller_scheduler_controllerTaskFunction, "ControllerTask", LINKARCH_CONTROLLER_TASK_STACK_SIZE, NULL, LINKARCH_CONTROLLER_TASK_PRIORITY, &linkarch_controller->controllerTaskHandle)) return true;
    return true;

}

static bool linkarch_controller_scheduler_setTasksCoreAffinity()
{

    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {

        if (!linkarch_controller_scheduler_setModuleTaskCoreAffinity(i)) return false;

    }

    if(!linkarch_controller_scheduler_setControllerCoreAffinity()) return false;

    return true;

}

static bool linkarch_controller_scheduler_setModuleTaskCoreAffinity(linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isModuleIDValidInTheController(linkarch_controller, moduleID)) return false;

    linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, moduleID);
    if (module == INVALID_MODULE) return false;

    if (module->isCoreAffinity) linkarch_osal_setTaskCoreAffinity(module->moduleTaskHandle, module->coreNumber);

    return true;

}

static bool linkarch_controller_scheduler_setControllerCoreAffinity()
{

    linkarch_osal_setTaskCoreAffinity(linkarch_controller->controllerTaskHandle, LINKARCH_CONTROLLERTASKCORE);
    return true;

}


// Tasks functions types


static void linkarch_controller_scheduler_moduleTaskFunction_clocked(void *arg)
{

    linkarch_module_id_t * moduleID_prt = (linkarch_module_id_t*) arg;
    linkarch_module_id_t moduleID = moduleID_prt[0];
    linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, moduleID);

    linkarch_mode_t * localActualMode = linkarch_controller_getActualMode(linkarch_controller);
    linkarch_controller_setupModule(moduleID, localActualMode);

    while(1)
    {
        if (!linkarch_controller_scheduler_takeModuleLoopMutex(moduleID)) continue;

        if(linkarch_controller_isTheModeChanged(localActualMode->mode_id)) 
        {
            linkarch_controller_shutdownModule(moduleID, localActualMode);
            localActualMode = linkarch_controller_getActualMode(linkarch_controller);
            linkarch_controller_setupModule(moduleID, localActualMode);
        }

        linkarch_controller_loopModule(moduleID, localActualMode);
        
        linkarch_controller_scheduler_releaseModuleLoopMutex(moduleID);

        linkarch_osal_delayUntil(&module->lastWakeTime, linkarch_osal_getMsToTick(module->modulePeriodInMS)); 
        module->lastWakeTime = linkarch_osal_getTickCount();

    }

}

static void linkarch_controller_scheduler_moduleTaskFunction_unclocked(void *arg)
{

    linkarch_module_id_t * moduleID_prt = (linkarch_module_id_t*) arg;
    linkarch_module_id_t moduleID = moduleID_prt[0];

    linkarch_mode_t * localActualMode = linkarch_controller_getActualMode(linkarch_controller);
    linkarch_controller_setupModule(moduleID, localActualMode);

    while(1)
    {
        
        if (!linkarch_controller_scheduler_takeModuleLoopMutex(moduleID)) continue;

        if(linkarch_controller_isTheModeChanged(localActualMode->mode_id)) 
        {
            linkarch_controller_shutdownModule(moduleID, localActualMode);
            localActualMode = linkarch_controller_getActualMode(linkarch_controller);
            linkarch_controller_setupModule(moduleID, localActualMode);
        }
        linkarch_controller_loopModule(moduleID, localActualMode);

        linkarch_controller_scheduler_releaseModuleLoopMutex(moduleID);

    }

}

static void linkarch_controller_scheduler_controllerTaskFunction(void *arg)
{
    
    (void) arg;

    linkarch_modeTransition_id_t modeTransitionID;

    while(1)
    {
        
        if (linkarch_osal_queueRecieve(linkarch_controller->modeTransition_queue, &modeTransitionID, LINKARCH_MAX_DELAY))
        {
            linkarch_controller_scheduler_takeAllModuleLoopMutex();
            linkarch_controller_handleModeTransition(modeTransitionID);
            linkarch_controller_scheduler_releaseAllModuleLoopMutex();
        }

    }


}


// Mutexs functions

static bool linkarch_controller_scheduler_takeAllModuleLoopMutex()
{

    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {

        linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, i);
        if (module == INVALID_MODULE) return false;

        if (module->loopModuleSemaphore == LINKARCH_INVALID_MUTEX) return false;

        if (!linkarch_osal_takeMutex(module->loopModuleSemaphore, LINKARCH_MAX_DELAY)) return false;

    }

    return true;

}

static bool linkarch_controller_scheduler_releaseAllModuleLoopMutex()
{

    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODULES; i++) 
    {

        linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, i);
        if (module == INVALID_MODULE) return false;

        if (module->loopModuleSemaphore == LINKARCH_INVALID_MUTEX) return false;

        if (!linkarch_osal_giveMutex(module->loopModuleSemaphore)) return false;

    }

    return true;

}

static bool linkarch_controller_scheduler_takeModuleLoopMutex(linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isModuleIDValidInTheController(linkarch_controller, moduleID)) return false;

    linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, moduleID);
    if (module == INVALID_MODULE) return false;

    if (module->loopModuleSemaphore == LINKARCH_INVALID_MUTEX) return false;

    return linkarch_osal_takeMutex(module->loopModuleSemaphore, LINKARCH_MAX_DELAY);;

}

static bool linkarch_controller_scheduler_releaseModuleLoopMutex(linkarch_module_id_t moduleID)
{

    if (!linkarch_controller_isModuleIDValidInTheController(linkarch_controller, moduleID)) return false;

    linkarch_module_t * module = linkarch_controller_getModuleByID(linkarch_controller, moduleID);
    if (module == INVALID_MODULE) return false;

    if (module->loopModuleSemaphore == LINKARCH_INVALID_MUTEX) return false;

    return linkarch_osal_giveMutex(module->loopModuleSemaphore);

}





/* Debugging functions for controller */

void linkarch_controller_debug_setDebugMessageMutex()
{

    #if (LINKARCH_DEBUG_MODE)
    linkarch_controller_debugMessageMutex = linkarch_osal_createMutex();
    #endif

}

bool linkarch_controller_debug_takeDebugMessageMutex()
{

    #if (LINKARCH_DEBUG_MODE)
    if (linkarch_controller_debugMessageMutex == LINKARCH_INVALID_MUTEX) return false;
    return linkarch_osal_takeMutex(linkarch_controller_debugMessageMutex, LINKARCH_MAX_DELAY);
    #else
    return true;
    #endif

}

bool linkarch_controller_debug_giveDebugMessageMutex()
{

    #if (LINKARCH_DEBUG_MODE)
    if (linkarch_controller_debugMessageMutex == LINKARCH_INVALID_MUTEX) return false;
    return linkarch_osal_giveMutex(linkarch_controller_debugMessageMutex);
    #else
    return true;
    #endif

}

void linkarch_controller_debug_controllerInfos(const linkarch_controller_t *controller)
{

    if (!linkarch_controller_isControllerValid(controller)) 
    {
        linkarch_hal_throwDebugMessage("Controller: NULL\n");
        return;
    }

    linkarch_hal_throwDebugMessage("Controller Number of Modules: %d\n", LINKARCH_CONTROLLER_NUMBER_OF_MODES);
    linkarch_hal_throwDebugMessage("Controller Number of Modes: %d\n", LINKARCH_CONTROLLER_NUMBER_OF_MODES);
    linkarch_hal_throwDebugMessage("Controller Number of Mode Transitions: %d\n", LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS);

    linkarch_hal_throwDebugMessage("\nList of Modules:\n");
    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODES; i++) 
    {
    
        linkarch_module_debug_moduleInfos(&controller->listOfModules[i]);

    }

    linkarch_hal_throwDebugMessage("\nList of Modes:\n");
    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODES; i++) 
    {
        
        linkarch_mode_debug_modeInfos(&controller->listOfModes[i]);

    }

    linkarch_hal_throwDebugMessage("\nList of Mode Transitions:\n");
    for (uint8_t i = 0; i < LINKARCH_CONTROLLER_NUMBER_OF_MODETRANSITIONS; i++) 
    {
    
        linkarch_controller_debug_modeTransitionInfos(&controller->listOfModeTransitions[i]);
    
    }

    linkarch_hal_throwDebugMessage("\nController FSM:\n");
    linkarch_fsm_debug_fsmInfos(controller->linkarch_controller_fsm);

}

void linkarch_controller_debug_modeTransitionInfos(const linkarch_controller_modeTransition_t *modeTransition)
{

    if (modeTransition == INVALID_MODULETRANSITION) 
    {
    
        linkarch_hal_throwDebugMessage("Mode Transition: NULL\n");
        return;
    
    }

    linkarch_hal_throwDebugMessage("Mode Transition ID: %d\n", modeTransition->modeTransition_id);
    linkarch_hal_throwDebugMessage("Mode Transition Name: %s\n", modeTransition->modeTransition_name);
    linkarch_hal_throwDebugMessage("Source Mode ID: %d\n", modeTransition->sourceModeID);
    linkarch_hal_throwDebugMessage("Target Mode ID: %d\n", modeTransition->targetModeID);

    if (modeTransition->endTransitionFunction != NULL) 
    {
    
        linkarch_hal_throwDebugMessage("End Transition Function: Defined\n");
    
    } else 
    {
    
        linkarch_hal_throwDebugMessage("End Transition Function: Not Defined\n");
    
    }

    if (modeTransition->startTransitionFunction != NULL) 
    {
    
        linkarch_hal_throwDebugMessage("Start Transition Function: Defined\n");
    
    } else 
    {
    
        linkarch_hal_throwDebugMessage("Start Transition Function: Not Defined\n");
    
    }

}

void linkarch_controller_debug_modeTransitionRequestMessage(linkarch_modeTransition_id_t modeTransitionID)
{

    if (!linkarch_controller_isModeTrasitionValidInTheController(linkarch_controller, modeTransitionID)) 
    {
        linkarch_hal_throwDebugMessage("Mode Transition ID: %d is invalid\n", modeTransitionID);
        return;
    }

    linkarch_controller_modeTransition_t * modeTransition = &linkarch_controller->listOfModeTransitions[modeTransitionID];
    
    linkarch_hal_throwDebugMessage("[linkarch controller]: Mode Transition Requested: {ID: %d, Name: %s, Source Mode ID: %d, Target Mode ID: %d}\n", modeTransition->modeTransition_id, modeTransition->modeTransition_name, modeTransition->sourceModeID, modeTransition->targetModeID);
    

}

void linkarch_controller_debug_modeTransitionMessage(bool statusHandleModeTransition, linkarch_modeTransition_id_t modeTransitionID)
{

    if (!linkarch_controller_isModeTrasitionValidInTheController(linkarch_controller, modeTransitionID)) 
    {
        linkarch_hal_throwDebugMessage("Mode Transition ID: %d is invalid\n", modeTransitionID);
        return;
    }

    linkarch_controller_modeTransition_t * modeTransition = &linkarch_controller->listOfModeTransitions[modeTransitionID];

    if(statusHandleModeTransition) linkarch_hal_throwDebugMessage("[linkarch controller]: Mode Transition Accepted: {ID: %d, Name: %s, Source Mode ID: %d, Target Mode ID: %d}\n", modeTransition->modeTransition_id, modeTransition->modeTransition_name, modeTransition->sourceModeID, modeTransition->targetModeID);
    else linkarch_hal_throwDebugMessage("[linkarch controller]: Mode Transition Rejected: {ID: %d, Name: %s, Source Mode ID: %d, Target Mode ID: %d}\n", modeTransition->modeTransition_id, modeTransition->modeTransition_name, modeTransition->sourceModeID, modeTransition->targetModeID);

}