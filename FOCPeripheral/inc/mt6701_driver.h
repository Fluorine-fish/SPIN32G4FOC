#ifndef __MT6701_DRIVER_H__
#define __MT6701_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stm32g4xx.h"

typedef enum {
    MAG_NORMAL,
    MAG_OVER,
    MAG_WEAK
}MT_MagStatus;

typedef enum {
    SPD_NORMAL,
    SPD_OVER,
}MT_SpeedStatus;

typedef struct
{
    uint16_t  u16Angle;
    MT_MagStatus tMagStatus;
    MT_SpeedStatus tSpeedStatus;
} MT_Info_t;

uint16_t MT6701_GetAngle(void);

extern MT_Info_t tMTInfo;

#ifdef __cplusplus
}
#endif

#endif /* __MT6701_DRIVER_H__ */
