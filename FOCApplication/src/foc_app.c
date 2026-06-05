/**
*   @file foc_app.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#include "foc_app.h"

#define OPEN_LOOP
#define SENSER_CTRL

float fIncAngel = 0.01f; //递增角度 (0~1)
FOC_Driver_t tFocDrv = {
    .tAppState.tStatus = S_RESET,
    .tAppState.tEvent = E_RESET,
};
Data_Debug_t tDebugData = {};

int16_t i16Delay = 50;
int16_t i16DelayCnt = 0;

/* 生成去标幺化还原单位的数据 */
static void DATA_DebugDataGenerate(Data_Debug_t* tData, FOC_Driver_t* tFocDrv) {
    tData->tIuvwFbck.fArg1 = tFocDrv->tIuvwFbck.fArg1 * PU_IB;
    tData->tIuvwFbck.fArg2 = tFocDrv->tIuvwFbck.fArg2 * PU_IB;
    tData->tIuvwFbck.fArg3 = tFocDrv->tIuvwFbck.fArg3 * PU_IB;

    tData->tDqFbck.fArg1 = tFocDrv->tDqFbck.fArg1 * PU_IB;
    tData->tDqFbck.fArg2 = tFocDrv->tDqFbck.fArg2 * PU_IB;

    tData->tAlphaBetaFbck.fArg1 = tFocDrv->tAlphaBetaFbck.fArg1 * PU_IB;
    tData->tAlphaBetaFbck.fArg2 = tFocDrv->tAlphaBetaFbck.fArg2 * PU_IB;
    tData->fDcBusVoltage = tFocDrv->fDcBusVoltageFbck * PU_UB * F_SQRT_3;
}

void APP_CurrentRestruct(void) {
    tFocDrv.pi16RawADC[0] = hadc1.Instance->JDR1; // u phase current
    tFocDrv.pi16RawADC[1] = hadc2.Instance->JDR1; // v phase current
    tFocDrv.pi16RawADC[2] = hadc1.Instance->JDR3; // w phase current
    tFocDrv.pi16RawADC[3] = hadc1.Instance->JDR2; // v bus voltage
    // 实际测量得到的更应该成为端电压
    tFocDrv.pi16RawADC[4] = hadc1.Instance->JDR4; // u phase voltage
    tFocDrv.pi16RawADC[5] = hadc2.Instance->JDR4; // v phase voltage
    tFocDrv.pi16RawADC[6] = hadc2.Instance->JDR2; // w phase voltage
    tFocDrv.pi16RawADC[7] = hadc2.Instance->JDR3; // ntc voltage

    tFocDrv.tIuvwFbck.fArg1 = -(float)(tFocDrv.pi16RawADC[0] - tFocDrv.i16ADC1offset) / 2048.0f; // u phase current
    tFocDrv.tIuvwFbck.fArg2 = -(float)(tFocDrv.pi16RawADC[1] - tFocDrv.i16ADC2offset) / 2048.0f; // v phase current
    tFocDrv.tIuvwFbck.fArg3 = -(tFocDrv.tIuvwFbck.fArg1 + tFocDrv.tIuvwFbck.fArg2);              // w phase current

    tFocDrv.fDcBusVoltageFbck = (float)tFocDrv.pi16RawADC[3] / 4096.f;

    DATA_DebugDataGenerate(&tDebugData, &tFocDrv);
}

void APP_SetDuty(MATH_3SystF_t* tSwTime) {
    TIM1->CCR1 = (uint32_t)(tSwTime->fArg1 * (PWM_PERIOD_CYCLES));
    TIM1->CCR2 = (uint32_t)(tSwTime->fArg2 * (PWM_PERIOD_CYCLES));
    TIM1->CCR3 = (uint32_t)(tSwTime->fArg3 * (PWM_PERIOD_CYCLES));
}

// 在错误状态下关闭输出，下管全导通，上管全关闭
void APP_DisableOutput(void) {
    TIM1->CCR1 = (uint32_t)(0.5f * (PWM_PERIOD_CYCLES));
    TIM1->CCR2 = (uint32_t)(0.5f * (PWM_PERIOD_CYCLES));
    TIM1->CCR3 = (uint32_t)(0.5f * (PWM_PERIOD_CYCLES));
}

void APP_InitFocDrvParas(void) {
    tFocDrv.fAmp = 0.1f;

    /* IdCtrl Init */
    tFocDrv.tIdCtrl.fInErrK_1 = 0.f;
    tFocDrv.tIdCtrl.fIntePartK_1 = 0.f;
    tFocDrv.tIdCtrl.fUpperLimit = tFocParas.fCurUppLimit;
    tFocDrv.tIdCtrl.fLowerLimit = tFocParas.fCurLowLimit;
    // 除以ZB以参数标幺化
    tFocDrv.tIdCtrl.fKpGain = MOTOR_LS * tFocParas.fIdqCutoffFreq * 2 * PI / PU_ZB;
    tFocDrv.tIdCtrl.fKiGain = MOTOR_RS * tFocParas.fIdqCutoffFreq * 2 * PI * tFocParas.fTs / PU_ZB;
    tFocDrv.tIdCtrl.fKaGain = 1.f / tFocDrv.tIdCtrl.fKpGain;

    /* IqCtrl Init */
    tFocDrv.tIqCtrl.fInErrK_1 = 0.f;
    tFocDrv.tIqCtrl.fIntePartK_1 = 0.f;
    tFocDrv.tIqCtrl.fUpperLimit = tFocParas.fCurUppLimit;
    tFocDrv.tIqCtrl.fLowerLimit = tFocParas.fCurLowLimit;
    // 除以ZB以参数标幺化
    tFocDrv.tIqCtrl.fKpGain = MOTOR_LS * tFocParas.fIdqCutoffFreq * 2 * PI / PU_ZB;
    tFocDrv.tIqCtrl.fKiGain = MOTOR_RS * tFocParas.fIdqCutoffFreq * 2 * PI * tFocParas.fTs / PU_ZB;
    tFocDrv.tIqCtrl.fKaGain = 1.f / tFocDrv.tIqCtrl.fKpGain;

    tFocDrv.tIdqReq.fArg1 = 0;
    tFocDrv.tIdqReq.fArg2 = 0;

    /* Align Init */
    tFocDrv.fAlignVoltage = tFocParas.fAlignVoltage;
    tFocDrv.u32AlignCnt = 0;
    tFocDrv.u32AlignTime = tFocParas.u32AlignTime;
    tFocDrv.u8AlignFlag = 0;

    /* SpeedCtrl Init */
    tFocDrv.tSpeedCtrl.fKpGain = tFocParas.fSpeedKp;
    tFocDrv.tSpeedCtrl.fKiGain = tFocParas.fSpeedKi;
    tFocDrv.tSpeedCtrl.fKaGain = (1.f) / tFocParas.fSpeedKp;
    tFocDrv.tSpeedCtrl.fInErrK_1 = 0.f;
    tFocDrv.tSpeedCtrl.fIntePartK_1 = 0.f;
    tFocDrv.tSpeedCtrl.fUpperLimit = tFocParas.fCurUppLimit;
    tFocDrv.tSpeedCtrl.fLowerLimit = tFocParas.fCurLowLimit;

    /* Init SpeedLoop */
    tFocDrv.u16SpeedCtrlPeriod = tFocParas.u16SpeedLoopPeriod;
    tFocDrv.u16SpeedCtrlCnt = 0;
    tFocDrv.tPospeOpenLoop.fOLThetaInte.fTs = tFocParas.fTs;
    tFocDrv.tPospeOpenLoop.fOLThetaInte.fInte = 0;
    tFocDrv.tPospeOpenLoop.fIqLowerLimit = tFocParas.fSpeedOLLowerLimit;
    tFocDrv.tPospeOpenLoop.fIqUpperLimit = tFocParas.fSpeedOLUpperLimit;

    /* Init Ramp */
    tFocDrv.tSpeedRamp.fRampUp = tFocParas.fSpeedUp;
    tFocDrv.tSpeedRamp.fRampDown = tFocParas.fSpeedDown;
    tFocDrv.tSpeedRamp.fState = 0;

    tFocDrv.tPospeCtrl.fWRotElReq = 0;
}

// 在ADC中断中调用调度器函数
void APP_FocScheduler(void) {
    APP_CurrentRestruct();

    /* SPI磁编码器数据读取 */
    tFocDrv.tSns.u16SnsRawTheta = MT6701_GetAngle();

    pStateFuncTable[tFocDrv.tAppState.tEvent][tFocDrv.tAppState.tStatus]();
}

/* Fault Status */
void StateFault(void) {
    int16_t i16FnStatus = 0;
    tFocDrv.tAppState.tStatus = S_FAULT;

    // Disable OutPut
    APP_DisableOutput();
    if (!i16FnStatus) {
    } else {
        tFocDrv.tAppState.tEvent = E_FAULT_CLEAR;
    }
}

/* Init Status */
void StateInit(void) {
    int16_t bFnStatus = 1;

    tFocDrv.tAppState.tStatus = S_INIT;
    tFocDrv.tAppState.tEvent = E_INIT;

    /* turn off bridges*/
    APP_DisableOutput();
    /* initialize parameters*/
    APP_InitFocDrvParas();

    if (!bFnStatus) {
    } else {
        tFocDrv.tAppState.tEvent = E_INIT_DONE;
    }
}

/* Ready Status */
void StateReady(void) {
    tFocDrv.tAppState.tStatus = S_READY;
    // tFocDrv.tAppState.tEvent = E_READY; // 预留对顺逆风判断的接口
    tFocDrv.tAppState.tEvent = E_APP_ON;
}

/* Calibrate Status */
void StateCalib(void) {
    int16_t i16FnStatus = 0;
    tFocDrv.tAppState.tStatus = S_CALIB;
    tFocDrv.tAppState.tEvent = E_CALIB;

    /* Calibration ADC offset in 500 * 50us */
    if (tFocDrv.i16CalibCnt < 500) {
        APP_DisableOutput();

        // 对ADC1，ADC2分别校准，因为ADC与OPAMP各有差异，offset不同
        tFocDrv.i32ClibAdc1 += tFocDrv.pi16RawADC[0];
        tFocDrv.i32ClibAdc2 += tFocDrv.pi16RawADC[1];

        // 计算平均值offset
        tFocDrv.i16ADC1offset = tFocDrv.i32ClibAdc1 / (tFocDrv.i16CalibCnt + 1);
        tFocDrv.i16ADC2offset = tFocDrv.i32ClibAdc2 / (tFocDrv.i16CalibCnt + 1);

        tFocDrv.i16CalibCnt++;
    } else {
        i16FnStatus = 1;
    }

    if (i16FnStatus) {
        tFocDrv.tAppState.tEvent = E_CALIB_DONE;
    }
}

/* Align Status */
void StateAlign(void) {
    int16_t i16FnStatus = 0;
    tFocDrv.tAppState.tStatus = S_ALIGN;
    tFocDrv.tAppState.tEvent = E_ALIGN;

    if (tFocDrv.u32AlignCnt < tFocDrv.u32AlignTime) {
        if (tFocDrv.u8AlignFlag == 0) {
            /* d轴体电压对齐 */
            tFocDrv.tUdqReq.fArg1 = tFocDrv.fAlignVoltage;
            tFocDrv.tUdqReq.fArg2 = 0;
        }else {
            /* q轴体电压对齐 */
            tFocDrv.tUdqReq.fArg1 = 0;
            tFocDrv.tUdqReq.fArg2 = tFocDrv.fAlignVoltage;
        }

        /* Step8 InvPark Trans */
        FOC_InvPark(&tFocDrv.tUdqReq, &tFocDrv.tUAlphaBetaReq, tFocDrv.fAngle);

        /* DcBusVoltage Comp */
        FOC_DcbusComp(tFocDrv.fDcBusVoltageFbck, &tFocDrv.tUAlphaBetaReq, &tFocDrv.tUAlphaBetaCompReq);
        /* Step9 SVPWM*/
        FOC_Svpwm(&tFocDrv.tUAlphaBetaCompReq, &tFocDrv.tTimSw, &tFocDrv.tSvmDuty);
        /* Step10 Timer&PWM*/
        APP_SetDuty(&tFocDrv.tTimSw);

        tFocDrv.u32AlignCnt++;
    }else {
        switch (tFocDrv.u8AlignFlag) {
            default: case 0:
                tFocDrv.u8AlignFlag = 1;
                tFocDrv.u32AlignCnt = 0;
                /* theta_r offset */
                tFocDrv.tSns.u16InitTheta = tFocDrv.tSns.u16SnsRawTheta;
                break;
            case 1:
                tFocDrv.u8AlignFlag = 2;
                tFocDrv.u32AlignCnt = 0;
                break;
            case 2:
                SNS_Init(&tFocDrv.tSns);
                i16FnStatus = 1;
        }
    }

    if (i16FnStatus) {
        tFocDrv.tAppState.tEvent = E_ALIGN_DONE;
    }
}

// 速度环相较于电流环运行更慢（5KHz）
void FOC_SlowLoop(void) {
    tFocDrv.tIdqReq.fArg1 = 0; // Id = 0控制

    /* Position Loop */
    tFocDrv.tPospeCtrl.fWRotElReq = FOC_CtrlPIpBR(tFocDrv.tSns.fTargetMultiTurnPos-tFocDrv.tSns.fSnsMultiTurnThetaMch,
        &tFocDrv.tSns.tPosCtrl);
    // tFocDrv.tPospeCtrl.fWRotElReq = 0.5f;

    /* Senor Speed */
    SNS_MchSpeedCalc(&tFocDrv.tSns);

    /* Speed Loop */
    tFocDrv.tPospeCtrl.fWRotElReqRamp = FOC_Ramp(tFocDrv.tPospeCtrl.fWRotElReq, &tFocDrv.tSpeedRamp);
    tFocDrv.tPospeCtrl.fWRotElErr = tFocDrv.tPospeCtrl.fWRotElReqRamp - tFocDrv.tPospeCtrl.fWRotEl;
    tFocDrv.tIdqReq.fArg2 = FOC_CtrlPIpBR(tFocDrv.tPospeCtrl.fWRotElErr, &tFocDrv.tSpeedCtrl);
}

// 每一个周期都会运行一次的电流环控制（20KHz）
void FOC_FastLoop(void) {
    /* Step3 Clarke Trans */
    FOC_Clarke(&tFocDrv.tIuvwFbck, &tFocDrv.tAlphaBetaFbck);
    /* Step5 Park Trans */
    FOC_Park(&tFocDrv.tAlphaBetaFbck, &tFocDrv.tDqFbck, tFocDrv.tPospeCtrl.fThetaRotEl);

    /* Id VectorCircle Limitation */
    tFocDrv.tIdCtrl.fUpperLimit = tFocParas.fCurUppLimit * tFocDrv.fDcBusVoltageFbck;
    tFocDrv.tIdCtrl.fLowerLimit = tFocParas.fCurLowLimit * tFocDrv.fDcBusVoltageFbck;

    /* Step7 Id Control */
    tFocDrv.tIdqErr.fArg1 = tFocDrv.tIdqReq.fArg1 - tFocDrv.tDqFbck.fArg1;
    tFocDrv.tIdqErr.fArg2 = tFocDrv.tIdqReq.fArg2 - tFocDrv.tDqFbck.fArg2;

    /* Iq VectorCircle Limitation */
    tFocDrv.tIqCtrl.fUpperLimit = BM_FastSqrt(tFocDrv.tIdCtrl.fUpperLimit * tFocDrv.tIdCtrl.fUpperLimit
            - tFocDrv.tUdqReq.fArg1 * tFocDrv.tUdqReq.fArg1);
    tFocDrv.tIqCtrl.fLowerLimit = - tFocDrv.tIqCtrl.fUpperLimit;

    /* Step7 Iq Control */
    tFocDrv.tUdqReq.fArg1 = FOC_CtrlPIpBR(tFocDrv.tIdqErr.fArg1, &tFocDrv.tIdCtrl);
    tFocDrv.tUdqReq.fArg2 = FOC_CtrlPIpBR(tFocDrv.tIdqErr.fArg2, &tFocDrv.tIqCtrl);

    /* Step8 InvPark Trans */
    FOC_InvPark(&tFocDrv.tUdqReq, &tFocDrv.tUAlphaBetaReq, tFocDrv.tPospeCtrl.fThetaRotEl);

    /* DcBusVoltage Comp */
    FOC_DcbusComp(tFocDrv.fDcBusVoltageFbck, &tFocDrv.tUAlphaBetaReq, &tFocDrv.tUAlphaBetaCompReq);
    /* Step9 SVPWM*/
    FOC_Svpwm(&tFocDrv.tUAlphaBetaCompReq, &tFocDrv.tTimSw, &tFocDrv.tSvmDuty);
    /* Step10 Timer&PWM*/
    APP_SetDuty(&tFocDrv.tTimSw);
}

/* Run Status */
float fTempAngle;
int32_t i32Angle;
void StateRun(void) {
    tFocDrv.tAppState.tStatus = S_RUN;
    tFocDrv.tAppState.tEvent = E_RUN;

    /* OpenLoop */
    // fTempAngle = FOC_Integrate(tFocDrv.tPospeCtrl.fWRotElReqRamp * PU_FB, &tFocDrv.tPospeOpenLoop.fOLThetaInte);
    // i32Angle = (int32_t)fTempAngle;
    // tFocDrv.tPospeOpenLoop.fOLThetaInte.fInte -= i32Angle; // 防止积分溢出
    // tFocDrv.tPospeOpenLoop.fThetaRotEl = fTempAngle - i32Angle;
    //
    // // 开环状态下的当前速度由积分得到, 将状态量注入控制器
    // tFocDrv.tPospeCtrl.fThetaRotEl = tFocDrv.tPospeOpenLoop.fThetaRotEl;
    // tFocDrv.tPospeCtrl.fWRotEl = 0.f;
    // tFocDrv.tSpeedCtrl.fUpperLimit = tFocParas.fSpeedOLUpperLimit;
    // tFocDrv.tSpeedCtrl.fLowerLimit = tFocParas.fSpeedOLLowerLimit;

    /* SensorCtrl */
    SNS_Mch2ElAngle(&tFocDrv.tSns);
    tFocDrv.tPospeCtrl.fThetaRotEl = tFocDrv.tSns.fThetaEl;
    tFocDrv.tPospeCtrl.fWRotEl = tFocDrv.tSns.fWRotEl;
    tFocDrv.tSpeedCtrl.fUpperLimit = tFocParas.fSpeedUpperLimit;
    tFocDrv.tSpeedCtrl.fLowerLimit = tFocParas.fSpeedLowerLimit;

    /* Speed Loop */
    if (tFocDrv.u16SpeedCtrlCnt >= tFocDrv.u16SpeedCtrlPeriod) {
        FOC_SlowLoop();
        tFocDrv.u16SpeedCtrlCnt = 0;
    }
    tFocDrv.u16SpeedCtrlCnt ++;

    /* Current Loop */
    FOC_FastLoop();

}

void StateReset(void) {
    int16_t i16FnStatus = 1;
    tFocDrv.tAppState.tStatus = S_RESET;
    tFocDrv.tAppState.tEvent = E_RESET;

    APP_DisableOutput();

    if (i16FnStatus) {
        tFocDrv.tAppState.tEvent = E_RESET_DONE;
    }
}
