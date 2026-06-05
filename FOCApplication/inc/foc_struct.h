/**
*   @file foc_struct.h
*   @brief 用于放置之后使用的外部include和变量
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_FOC_STRUCT_H
#define FOC_SPIING4_TEST_FOC_STRUCT_H

#include "stdint.h"
#include "stm32g4xx.h"
#include "base_math.h"
#include "foc_math.h"
#include "state_machine.h"
#include "sensor_control.h"
#include "tim.h"
#include "adc.h"
#include "cmsis_os.h"

typedef struct {
    MATH_3SystF_t tIuvwFbck;
    MATH_2SystF_t tAlphaBetaFbck;
    MATH_2SystF_t tDqFbck;
    float fDcBusVoltage;
} Data_Debug_t;

typedef struct
{
    float fThetaRotEl; // electrical theta
    float fWRotEl;    // electrical speed omega
    float fWRotElReq;  // required electrical speed omega
    float fWRotElReqRamp; // ramped speed omega
    float fWRotElErr; // the error of between real speed and required speed
}FOC_PospeCtrl_t;

typedef struct
{
    float fThetaRotEl;  // open loop theta
    float fWRotEl;      // open loop speed
    float fIqUpperLimit;  // open loop Iq current upper limit
    float fIqLowerLimit;  // open loop Iq current lower limit
    FOC_Integral_t fOLThetaInte; // integral: speed -> theta
}FOC_OpenLoop_t;


typedef struct {
    /* adc采集的原始值，其相关索引保存的数据关系为：
      0：U相电流 1：V相电流 2：W相电流 3：母线VBUS电压 4：U相相电压
      5：V相相电压 6：W相相电压 7：NTC采样电压*/
    int16_t pi16RawADC[8];
    /* ADC1 采样U相电流的偏置值，为了消除信号链路的偏置 */
    int16_t i16ADC1offset;
    /* ADC2 采样U相电流的偏置值，为了消除信号链路的偏置 */
    int16_t i16ADC2offset;
    /* 校准过程中的校准次数计数器 */
    int16_t i16CalibCnt;
    /* 校准过程中adc1采样数据的总和，最后除以校准次数，采用平均法保证校准精度 */
    int32_t i32ClibAdc1;
    /* 校准过程中adc2采样数据的总和，最后除以校准次数，采用平均法保证校准精度 */
    int32_t i32ClibAdc2;
    /* 对齐相关变量 */
    float fAlignVoltage;
    uint32_t u32AlignTime;
    uint32_t u32AlignCnt;
    uint8_t  u8AlignFlag;
    /* 反馈的实际母线电压 */
    float fDcBusVoltageFbck;
    /* 经过电流重构后的的实际电流 */
    MATH_3SystF_t tIuvwFbck;
    MATH_2SystF_t tAlphaBetaFbck;
    MATH_2SystF_t tDqFbck;
    /* 三相桥臂的换相时刻点 */
    MATH_3SystF_t tTimSw;
    /* 三相桥臂上面高电平的时间，仅做观察使用 */
    MATH_3SystF_t tSvmDuty;
    /* 作用于SVPWM调制的alpha,beta电压 */
    MATH_2SystF_t tUAlphaBetaReq;
    MATH_2SystF_t tUAlphaBetaCompReq;
    /* IdIq电流环控制相关 */
    FOC_CtrlPIpBR_t tIdCtrl;
    FOC_CtrlPIpBR_t tIqCtrl;
    MATH_2SystF_t tIdqErr;
    MATH_2SystF_t tIdqReq;
    MATH_2SystF_t tUdqReq; // dq电压输出

    FOC_PospeCtrl_t tPospeCtrl;
    FOC_OpenLoop_t tPospeOpenLoop;
    uint16_t u16SpeedCtrlPeriod; // 速度控制器周期
    uint16_t u16SpeedCtrlCnt;
    FOC_Ramp_t tSpeedRamp;
    FOC_CtrlPIpBR_t tSpeedCtrl;
    /* sensor ctrl */
    Sensor_t tSns;

    /* 状态转移变量*/
    APP_State_Transfer_t tAppState;
    float fAmp;   // vector amp for debug
    float fAngle; //Angle for Debug (0~1)
} FOC_Driver_t;

#endif //FOC_SPIING4_TEST_FOC_STRUCT_H
