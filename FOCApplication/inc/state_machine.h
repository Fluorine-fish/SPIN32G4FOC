/**
*   @file state_machine.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_STATE_MACHINE_H
#define FOC_SPIING4_TEST_STATE_MACHINE_H

typedef enum {
    S_INIT  = 0,
    S_FAULT = 1,
    S_READY = 2,
    S_CALIB = 3,
    S_ALIGN = 4,
    S_RUN   = 5,
    S_RESET = 6
} AppStates; /* Application state identification user type*/

typedef enum {
    E_FAULT       = 0,
    E_FAULT_CLEAR = 1,
    E_INIT        = 2,
    E_INIT_DONE   = 3,
    E_READY       = 4,
    E_APP_ON      = 5,
    E_CALIB       = 6,
    E_CALIB_DONE  = 7,
    E_ALIGN       = 8,
    E_ALIGN_DONE  = 9,
    E_RUN         = 10,
    E_APP_OFF     = 11,
    E_RESET       = 12,
    E_RESET_DONE  = 13
} AppEvents; /* Application event identification user type*/

// 状态机结构体
typedef struct {
    AppStates tStatus;
    AppEvents tEvent;
} APP_State_Transfer_t;

typedef void (*pFunc_t)();

extern pFunc_t pStateFuncTable[14][7];

extern void StateFault(void);
extern void StateInit(void);
extern void StateReady(void);
extern void StateCalib(void);
extern void StateAlign(void);
extern void StateRun(void);
extern void StateReset(void);

#endif //FOC_SPIING4_TEST_STATE_MACHINE_H
