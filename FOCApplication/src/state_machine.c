/**
*   @file state_machine.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#include "state_machine.h"

pFunc_t pStateFuncTable[14][7] = {
    /* Actual State ->      'Init'       'Fault'      'Ready'    'Calib'     'Align'      'Run'		'Reset' */
    /* e_fault          */ {StateFault, StateFault, StateFault, StateFault, StateFault, StateFault, StateFault},
    /* e_fault_clear    */ {StateFault, StateInit, StateFault, StateFault, StateFault, StateFault, StateFault},
    /* e_init          	*/ {StateInit,  StateFault, StateFault, StateFault, StateFault, StateFault, StateFault},
    /* e_init_done      */ {StateReady, StateFault, StateFault, StateFault, StateFault, StateFault, StateFault},
    /* e_ready          */ {StateFault, StateFault, StateReady, StateFault, StateFault, StateFault, StateFault},
    /* e_app_on         */ {StateFault, StateFault, StateCalib, StateFault, StateFault, StateFault, StateFault},
    /* e_calib          */ {StateFault, StateFault, StateFault, StateCalib, StateFault, StateFault, StateFault},
    /* e_calib_done     */ {StateFault, StateFault, StateFault, StateAlign, StateFault, StateFault, StateFault},
    /* e_align          */ {StateFault, StateFault, StateFault, StateFault, StateAlign, StateFault, StateFault},
    /* e_align_done     */ {StateFault, StateFault, StateFault, StateFault, StateRun, StateFault, StateFault},
    /* e_run            */ {StateFault, StateFault, StateFault, StateFault, StateFault, StateRun, StateFault},
    /* e_app_off        */ {StateFault, StateFault, StateReady, StateInit, StateInit, StateInit, StateFault},
    /* e_reset          */ {StateFault, StateFault, StateReset, StateFault, StateFault, StateFault, StateReset},
    /* e_reset_done     */ {StateFault, StateFault, StateFault, StateFault, StateFault, StateFault, StateInit}};
