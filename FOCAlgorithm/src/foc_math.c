/**
*   @file foc_math.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/

#include "foc_math.h"

uint16_t FOC_Svpwm(
    MATH_2SystF_t* tAlphaBeta,
    MATH_3SystF_t* tSwTim,
    MATH_3SystF_t* t3PhDuty) {

    float fVref1, fVref2, fVref3;
    int16_t A, B, C, N;
    float X, Y, Z;
    float T4, T6;
    uint16_t u16Sec;
    float  Ta, Tb, Tc;

    // Step1
    fVref1 = tAlphaBeta->fArg2;
    fVref2 = (F_SQRT_3 / 2.f) * tAlphaBeta->fArg1 - .5f * tAlphaBeta->fArg2;
    fVref3 = (- F_SQRT_3 / 2.f) * tAlphaBeta->fArg1 - .5f * tAlphaBeta->fArg2;

    A = (fVref1 > 0) ? 1 : 0;
    B = (fVref2 > 0) ? 1 : 0;
    C = (fVref3 > 0) ? 1 : 0;

    // Step2
    N = 4 * C + 2 * B + A;

    // Step3 Ts在满占空比下为1用于更好周期转换， u_alpha u_beta根据相电压标幺化之后Uph = 1 Udc = sqrt_3 * Uph
    X = tAlphaBeta->fArg2;
    Y = .5f * (F_SQRT_3 * tAlphaBeta->fArg1 + tAlphaBeta->fArg2);
    Z = .5f * ( - F_SQRT_3 * tAlphaBeta->fArg1 + tAlphaBeta->fArg2);

    // Step4
    switch (N) {
        case 1:
            T4 = Z; T6 = Y; u16Sec = 2;
            break;
        case 2:
            T4 = Y; T6 = -X; u16Sec = 6;
            break;
        case 3:
            T4 = -Z; T6 = X; u16Sec = 1;
            break;
        case 4:
            T4 = -X; T6 = Z; u16Sec = 4;
            break;
        case 5:
            T4 = X; T6 = -Y; u16Sec = 3;
            break;
        case 6:
            T4 = -Y; T6 = -Z; u16Sec = 5;
            break;
        default:
            u16Sec = 0;
            break;
    }

    Ta = (1.f - T4 - T6) / 4.f;
    Tb = Ta + T4 / 2.f;
    Tc = Tb + T6 / 2.f;

    switch (N) {
        case 1:
            // PWM Switch timing
            tSwTim->fArg1 = Tb;
            tSwTim->fArg2 = Ta;
            tSwTim->fArg3 = Tc;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        case 2:
            // PWM Switch timing
            tSwTim->fArg1 = Ta;
            tSwTim->fArg2 = Tc;
            tSwTim->fArg3 = Tb;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        case 3:
            // PWM Switch timing
            tSwTim->fArg1 = Ta;
            tSwTim->fArg2 = Tb;
            tSwTim->fArg3 = Tc;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        case 4:
            // PWM Switch timing
            tSwTim->fArg1 = Tc;
            tSwTim->fArg2 = Tb;
            tSwTim->fArg3 = Ta;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        case 5:
            // PWM Switch timing
            tSwTim->fArg1 = Tc;
            tSwTim->fArg2 = Ta;
            tSwTim->fArg3 = Tb;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        case 6:
            // PWM Switch timing
            tSwTim->fArg1 = Tb;
            tSwTim->fArg2 = Tc;
            tSwTim->fArg3 = Ta;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
        default: //
            // PWM Switch timing
            tSwTim->fArg1 = 0;
            tSwTim->fArg2 = 0;
            tSwTim->fArg3 = 0;

            // Duty = 1 - 2Sw (采用Mode2的中心对称生成PWM）
            t3PhDuty->fArg1 = 1.f - 2.f * tSwTim->fArg1;
            t3PhDuty->fArg2 = 1.f - 2.f * tSwTim->fArg2;
            t3PhDuty->fArg3 = 1.f - 2.f * tSwTim->fArg3;
            break;
    }

    return u16Sec;
}

int16_t FOC_DecompAlphaBeta(float fVecAmp, float fAngel, MATH_2SystF_t* tAlphaBeta) {
    float sin, cos;
    sin = BM_FastSin(fAngel);
    cos = BM_FastCos(fAngel);

    tAlphaBeta->fArg1 = fVecAmp * cos; // Alpha
    tAlphaBeta->fArg2 = fVecAmp * sin; // Beta

    return 1;
}

void FOC_Clarke(const MATH_3SystF_t* const tUvw, MATH_2SystF_t* tAlphaBeta) {
    tAlphaBeta->fArg1 = tUvw->fArg1; //ia + ib + ic = 0 化简得到
    tAlphaBeta->fArg2 = (1.f / F_SQRT_3) * (tUvw->fArg2 - tUvw->fArg3);
}

void FOC_Park(const MATH_2SystF_t* const tAlphaBeta, MATH_2SystF_t* tDq, float fAngel) {
    float fSin = BM_FastSin(fAngel);
    float fCos = BM_FastCos(fAngel);

    tDq->fArg1 = tAlphaBeta->fArg1 * fCos + tAlphaBeta->fArg2 * fSin; // d
    tDq->fArg2 = tAlphaBeta->fArg2 * fCos - tAlphaBeta->fArg1 * fSin; // q
}

void FOC_InvPark(const MATH_2SystF_t* const tDq, MATH_2SystF_t* tAlphaBeta, float fAngel) {
    float fSin = BM_FastSin(fAngel);
    float fCos = BM_FastCos(fAngel);

    tAlphaBeta->fArg1 = tDq->fArg1 *fCos - tDq->fArg2 * fSin;
    tAlphaBeta->fArg2 = tDq->fArg1 * fSin + tDq->fArg2 * fCos;
}

float FOC_CtrlPIpBR(float fInErr, FOC_CtrlPIpBR_t* const ptParam) {
    float fKpPart, fKiPart, fOut, fAntiWindupErr;

    fKpPart = fInErr * ptParam->fKpGain;
    ptParam->fIntePartK_1 = ptParam->fIntePartK_1 + fInErr;
    fKiPart = ptParam->fIntePartK_1 * ptParam->fKiGain;
    fOut = fKiPart + fKpPart;

    // Anti-windup & limitout
    if (fOut > ptParam->fUpperLimit) {
        fAntiWindupErr = ptParam->fUpperLimit - fOut;
        ptParam->fIntePartK_1 += fAntiWindupErr * ptParam->fKaGain;
        fOut = ptParam->fUpperLimit;
    } else if (fOut < ptParam->fLowerLimit) {
        fAntiWindupErr = ptParam->fLowerLimit - fOut;
        ptParam->fIntePartK_1 += fAntiWindupErr * ptParam->fKaGain;
        fOut = ptParam->fLowerLimit;
    }

    return fOut;
}

void FOC_DcbusComp(float fDcbusActual, MATH_2SystF_t* tUAlphaBeta, MATH_2SystF_t* tUAlphaBetaComp) {
    tUAlphaBetaComp->fArg1 = (1.f / fDcbusActual) * tUAlphaBeta->fArg1;
    tUAlphaBetaComp->fArg2 = (1.f / fDcbusActual) * tUAlphaBeta->fArg2;
}

float FOC_Ramp(float fIn, FOC_Ramp_t* const ptRamp) {
    if (fIn > 0) {
        if (ptRamp->fState < fIn - ptRamp->fRampUp) ptRamp->fState += ptRamp->fRampUp;
        if (ptRamp->fState > fIn + ptRamp->fRampDown) ptRamp->fState -= ptRamp->fRampDown;
    }else if (fIn < 0) {
        if (ptRamp->fState > fIn + ptRamp->fRampUp) ptRamp->fState -= ptRamp->fRampUp;
        if (ptRamp->fState < fIn - ptRamp->fRampDown) ptRamp->fState += ptRamp->fRampDown;
    }

    return ptRamp->fState;
}

float FOC_Integrate(float fIn, FOC_Integral_t* const ptInte) {
    ptInte->fInte += ptInte->fTs * fIn;
    return ptInte->fInte;
}
