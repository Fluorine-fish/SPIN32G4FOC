#include "mt6701_driver.h"
#include "main.h"

#define MT6701_CS_Enable() HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET)
#define MT6701_CS_Disable() HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET)

MT_Info_t tMTInfo;
extern SPI_HandleTypeDef hspi3;
static uint8_t MT6701_Crc(uint32_t w_InputData);

/* CRC table for MT6701 crc-6*/
static uint8_t ptCrcTbl[64] = {
 0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09,
 0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
 0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
 0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
 0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
 0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
 0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A,
 0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02
};

/* receive MT6701 data*/
uint16_t MT6701_GetAngle(void)
{
    
    uint32_t u32RawData;
    uint8_t pu8RxData[3];
    uint8_t u8CalCRC;
    uint8_t u8RxCRC;
    uint16_t timeOut = 200;
    

    while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY)
    {
        if (timeOut-- == 0)
        {
            return tMTInfo.u16Angle;
        }
    }

    MT6701_CS_Enable();

    HAL_StatusTypeDef spiStatus = HAL_SPI_Receive(&hspi3, (uint8_t*)&pu8RxData, 3, 1);
    if (spiStatus != HAL_OK)
    {
        MT6701_CS_Disable();
        return tMTInfo.u16Angle; 
    }
    
    u32RawData = (pu8RxData[0] << 16) | (pu8RxData[1] << 8) | (pu8RxData[2]);
    MT6701_CS_Disable();
    u8CalCRC = MT6701_Crc(u32RawData>>6);
    u8RxCRC = u32RawData&0x3F;
    if (u8CalCRC == u8RxCRC) {
        tMTInfo.u16Angle = u32RawData >> 10; // only angle data
				tMTInfo.u16Angle = tMTInfo.u16Angle << 2; // transfer into 16 bit
        tMTInfo.tMagStatus = (uint8_t)((u32RawData &0xc0)>>6);
        tMTInfo.tSpeedStatus = (u32RawData &0x20)>>9;
    }

    return tMTInfo.u16Angle;
}


/*32-bit input data, right alignment, Calculation over 18 bits (mult. of 6) */
static uint8_t MT6701_Crc (uint32_t w_InputData)
{
 uint8_t u8Index = 0;
 uint8_t u8CRC = 0;

 u8Index = (uint8_t )(((uint32_t)w_InputData >> 12u) & 0x0000003Fu);

 u8CRC = (uint8_t )(((uint32_t)w_InputData >> 6u) & 0x0000003Fu);
 u8Index = u8CRC ^ ptCrcTbl[u8Index];

 u8CRC = (uint8_t )((uint32_t)w_InputData & 0x0000003Fu);
 u8Index = u8CRC ^ ptCrcTbl[u8Index];

 u8CRC = ptCrcTbl[u8Index];

 return u8CRC;
} 
