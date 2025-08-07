#ifndef LINKARCH_CONTROLLER_PRIVATE
#define LINKARCH_CONTROLLER_PRIVATE




// Include controller Parts
#include "linkarch_controller_type.h"
#include "linkarch_controller_scheduler.h"




/* Private controller functions */


// FSM functions

static bool linkarch_controller_handleModeTransition(linkarch_modeTransition_id_t modeTrasitionID);


// Config functions

static void linkarch_setGlobalFSMToControllerFSM(linkarch_controller_t * controller, linkarch_mode_id_t initialModeID);
static bool linkarch_controller_configModules();
static bool linkarch_controller_configModule(const linkarch_controller_t * controller, linkarch_module_id_t moduleID);
static bool linkarch_controller_configModeTrasitionQueue(linkarch_controller_t * controller);
static bool linkarch_controller_configModulesMutex(linkarch_controller_t * controller);


// Get functions

static linkarch_mode_id_t linkarch_controller_getActualModeID();
static linkarch_mode_t * linkarch_controller_getActualMode(const linkarch_controller_t * controller);
static linkarch_mode_t * linkarch_controller_getModeByID(const linkarch_controller_t * controller, linkarch_mode_id_t modeID);
static linkarch_module_t * linkarch_controller_getModuleByID(const linkarch_controller_t * controller, linkarch_module_id_t moduleID);


// Validation functions

inline static bool linkarch_controller_isControllerValid(const linkarch_controller_t * controller);
inline static bool linkarch_controller_isModeIDValidInTheController(const linkarch_controller_t * controller, linkarch_mode_id_t modeID);
inline static bool linkarch_controller_isModuleIDValidInTheController(const linkarch_controller_t * controller, linkarch_module_id_t moduleID);
inline static bool linkarch_controller_isModeTrasitionValidInTheController(const linkarch_controller_t * controller, linkarch_modeTransition_id_t modeTrasitionID);


// Module functions

static void linkarch_controller_setupModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode);
static void linkarch_controller_loopModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode);
static void linkarch_controller_shutdownModule(const linkarch_module_id_t mooduleID, const linkarch_mode_t * mode);
static void linkarch_controller_executeModeFunction(modeFunctions function);




#endif  // LINKARCH_CONTROLLER_PRIVATE
