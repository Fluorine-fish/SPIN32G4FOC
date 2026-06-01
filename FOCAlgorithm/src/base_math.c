/**
*   @file base_math.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/

#include "base_math.h"

#define HOLLYST 0.017453292519943295769236907684886f

const float pfSinTable[] = {
    0.0,                                    //sin(0)
    0.17364817766693034885171662676931 ,    //sin(10)
    0.34202014332566873304409961468226 ,    //sin(20)
    0.5 ,                                   //sin(30)
    0.64278760968653932632264340990726 ,    //sin(40)
    0.76604444311897803520239265055542 ,    //sin(50)
    0.86602540378443864676372317075294 ,    //sin(60)
    0.93969262078590838405410927732473 ,    //sin(70)
    0.98480775301220805936674302458952 ,    //sin(80)
    1.0                                     //sin(90)
};

const float pfCosTable[] = {
    1.0 ,                                   //cos(0)
    0.99984769515639123915701155881391 ,    //cos(1)
    0.99939082701909573000624344004393 ,    //cos(2)
    0.99862953475457387378449205843944 ,    //cos(3)
    0.99756405025982424761316268064426 ,    //cos(4)
    0.99619469809174553229501040247389 ,    //cos(5)
    0.99452189536827333692269194498057 ,    //cos(6)
    0.99254615164132203498006158933058 ,    //cos(7)
    0.99026806874157031508377486734485 ,    //cos(8)
    0.98768834059513772619004024769344      //cos(9)
};

// input 0-1: 0-360 degree
float BM_FastSin(float fAngle)
{
    int sig = 0;
    float tmpAngle = 0;
    if (fAngle >= 0.5f) {
        sig = 1;
        fAngle = fAngle - 0.5f;
    }
    tmpAngle = (fAngle > 0.25f) ? (0.5f - fAngle) : fAngle;

    int a = tmpAngle * 36.0f;
    float b = tmpAngle * 360.0f - a*10.0f;
    float sin = pfSinTable[a] * pfCosTable[(int)b] +b * HOLLYST *pfSinTable[9-a];
    if (sin > 1.0f) {
        sin = 1.0f;
    }
    return (sig > 0) ? -sin: sin;
}

// input 0-1:0-360 degree
float BM_FastCos(float fAngle)
{
    int sig = 0;
    float tmpAngle = 0;

    if (fAngle <= 0.75f) {
        fAngle += 0.25f;
    } else {
        fAngle -= 0.75f;
    }

    if (fAngle >= 0.5f) {
        sig = 1;
        fAngle = fAngle - 0.5f;
    }
    tmpAngle = (fAngle > 0.25f) ? (0.5f - fAngle) : fAngle;

    int a = tmpAngle * 36.0f;
    float b = tmpAngle * 360.0f - a*10.0f;
    float sin = pfSinTable[a] * pfCosTable[(int)b] +b * HOLLYST *pfSinTable[9-a];
    if (sin > 1.0f) {
        sin = 1.0f;
    }
    return (sig > 0) ? -sin: sin;
}
