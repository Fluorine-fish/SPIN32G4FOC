/**
*   @file sensor_control.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/4
*   @version 1.0
*   @note
*/
#include "sensor_control.h"

void SNS_Init(Sensor_t* tSns) {
    tSns->fSpeedPeriod = tFocParas.u16SpeedLoopPeriod * MOTOR_TS;
    tSns->u16Npp = 5;

    tSns->u16SnsRawThetaN_1 = tSns->u16SnsRawTheta;
    tSns->fSnsThetaN_1 = (tSns->u16SnsRawTheta - tSns->u16InitTheta) / 65535.0f;
    SNS_Mch2ElAngle(tSns);

    tSns->tPosCtrl.fKpGain = 0.8f;
    tSns->tPosCtrl.fKiGain = 0.0f;
    tSns->tPosCtrl.fKaGain = 0.0f;
    tSns->tPosCtrl.fUpperLimit = 0.1f;
    tSns->tPosCtrl.fLowerLimit = -0.1f;
    tSns->tPosCtrl.fInErrK_1 = 0.f;
    tSns->tPosCtrl.fIntePartK_1 = 0.f;
}

void SNS_Mch2ElAngle(Sensor_t* tSns) {
    tSns->u16SnsCalibTheta = tSns->u16SnsRawTheta - tSns->u16InitTheta;
    tSns->fThetaEl = (float)((uint16_t)(tSns->u16SnsCalibTheta * tSns->u16Npp)) / 65535.0f;// 借助整数运算多圈取余
    if (tSns->fThetaEl < 0) {
        tSns->fThetaEl = 0;
    }
    if (tSns->fThetaEl > 1.0f) {
        tSns->fThetaEl = 1.0f;
    }

    /* MultiPos */
    tSns->fSnsTheta = (float)tSns->u16SnsRawTheta / 65535.f;
    tSns->fDeltaAngle = tSns->fSnsTheta - tSns->fSnsThetaN_1;
    if (fabsf(tSns->fDeltaAngle) > 0.8f) tSns->i32MultiTurnCnt -= (tSns->fDeltaAngle > 0 ? 1 : -1);
    tSns->fSnsThetaN_1 = tSns->fSnsTheta;

    tSns->fSnsMultiTurnThetaMch = (float)tSns->i32MultiTurnCnt + tSns->fSnsTheta;
}

void SNS_MchSpeedCalc(Sensor_t* tSns) {
    int16_t i16DeltaTheta = 0;// 使用i16可以计算u16值之差得到±值
    i16DeltaTheta = (int16_t)(tSns->u16SnsRawTheta - tSns->u16SnsRawThetaN_1);
    tSns->fSnsWRotMechRps = (float)i16DeltaTheta / 65535.0f / tSns->fSpeedPeriod;
    tSns->fWRotEl = tSns->fSnsWRotMechRps * (float)tSns->u16Npp / PU_FB; // 将电角频率Hz标幺化
    tSns->u16SnsRawThetaN_1 = tSns->u16SnsRawTheta;
    tSns->fSnsWRotMechRpm = (float)tSns->fSnsWRotMechRps * 60.0f;
}
