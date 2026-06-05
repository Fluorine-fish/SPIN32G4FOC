/**
*   @file sensor_control.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/4
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_SENSOR_CONTROL_H
#define FOC_SPIING4_TEST_SENSOR_CONTROL_H

#include "base_math.h"
#include "foc_math.h"
#include "foc_paras.h"
#include <math.h>

typedef struct {
    uint16_t u16InitTheta; // Sensor angle offset
    uint16_t u16SnsRawTheta;
    uint16_t u16SnsRawThetaN_1; // 上一次编码器值
    uint16_t u16Npp;            // 极对数
    uint16_t u16SnsCalibTheta;  // 校准后的编码器角度
    float fThetaEl;             // 电角度(0~1)

    float fSnsWRotMechRps; // 机械转速
    float fSnsWRotMechRpm;
    float fWRotEl;
    float fSpeedPeriod; // 计算转速的平均周期

    float fSnsMultiTurnThetaMch;
    float fSnsTheta;
    float fSnsThetaN_1;      // 用于判断过圈的上一次变量
    float fDeltaAngle;
    int32_t i32MultiTurnCnt; // 圈数

    FOC_CtrlPIpBR_t tPosCtrl;
    float fTargetMultiTurnPos; // 目标多圈位置
} Sensor_t;

/* 有感结构体初始化 */
void SNS_Init(Sensor_t* tSns);

/* 机械角度转换到电角度 */
void SNS_Mch2ElAngle(Sensor_t* tSns);

/* 机械速度计算 */
void SNS_MchSpeedCalc(Sensor_t* tSns);

#endif //FOC_SPIING4_TEST_SENSOR_CONTROL_H
