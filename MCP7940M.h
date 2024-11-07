/*
 * MCP7940M.h
 *
 * This file contains the declarations for the driver of the MCP7940M Real-Time Clock (RTC) using the STM32F429 microcontroller.
 * The driver provides functions to initialize the RTC, set and get the time, and read/write individual time registers.
 * The driver uses the HAL library for I2C communication.
 *
 * Created on: Nov 6, 2024
 *
 * MIT License
 *
 * Copyright (c) 2024 Stian-Lund
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INC_MCP7940M_H_
#define INC_MCP7940M_H_

#include "stm32f4xx_hal.h" /* Needed for I2C */
#include <stdint.h>        /* Needed for uint8_t etc. */

/*
 * DEFINES
 */
#define MCP7940M_I2C_ADDRESS (0x1101111 << 1) /* Datasheet p.8 */

/*
 * REGISTERS
 */
#define MCP7940M_REG_RTCSEC 0x00
#define MCP7940M_REG_RTCMIN 0x01
#define MCP7940M_REG_RTCHOUR 0x02
#define MCP7940M_REG_RTCWKDAY 0x03
#define MCP7940M_REG_RTCDATE 0x04
#define MCP7940M_REG_RTCMTH 0x05
#define MCP7940M_REG_RTCYEAR 0x06
#define MCP7940M_REG_CONTROL 0x07
#define MCP7940M_REG_OSCTRIM 0x08

#define MCP7940M_REG_ALM0SEC 0x0A
#define MCP7940M_REG_ALM0MIN 0x0B
#define MCP7940M_REG_ALM0HOUR 0x0C
#define MCP7940M_REG_ALM0WKDAY 0x0D
#define MCP7940M_REG_ALM0DATE 0x0E
#define MCP7940M_REG_ALM0MTH 0x0F

#define MCP7940M_REG_ALM1SEC 0x11
#define MCP7940M_REG_ALM1MIN 0x12
#define MCP7940M_REG_ALM1HOUR 0x13
#define MCP7940M_REG_ALM1WKDAY 0x14
#define MCP7940M_REG_ALM1DATE 0x15
#define MCP7940M_REG_ALM1MTH 0x16

/*
 * WEEKDAY ENUM
 */
typedef enum
{
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekday;

/*
 * MCP7940M STRUCT
 */
typedef struct
{
    /* I2C handle */
    I2C_HandleTypeDef *i2cHandle;

    /* Time */
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    Weekday weekday;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} MCP7940M;

/*
 * INITIALISATION
 */
void MCP7940M_Init(MCP7940M *p_mcp7940m, I2C_HandleTypeDef *p_i2cHandle);

/*
 * GET AND SET TIME
 */
HAL_StatusTypeDef MCP7940M_GetTime(MCP7940M *p_mcp7940m);
HAL_StatusTypeDef MCP7940M_SetTime(MCP7940M *p_mcp7940m);

/*
 * LOW LEVEL FUNCTIONS
 */
HAL_StatusTypeDef MCP7940M_ReadRegister(MCP7940M *mcp7940m, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef MCP7940M_WriteRegister(MCP7940M *mcp7940m, uint8_t reg, uint8_t data);

/*
 * TIME READ FUNCTIONS
 */
void MCP7940M_ReadSeconds(MCP7940M *p_mcp7940m);
void MCP7940M_ReadMinutes(MCP7940M *p_mcp7940m);
void MCP7940M_ReadHours(MCP7940M *p_mcp7940m);
void MCP7940M_ReadWeekday(MCP7940M *p_mcp7940m);
void MCP7940M_ReadDate(MCP7940M *p_mcp7940m);
void MCP7940M_ReadMonth(MCP7940M *p_mcp7940m);
void MCP7940M_ReadYear(MCP7940M *p_mcp7940m);

/*
 * TIME WRITE FUNCTIONS
 */
void MCP7940M_WriteSeconds(MCP7940M *p_mcp7940m);
void MCP7940M_WriteMinutes(MCP7940M *p_mcp7940m);
void MCP7940M_WriteHours(MCP7940M *p_mcp7940m);
void MCP7940M_WriteWeekday(MCP7940M *p_mcp7940m);
void MCP7940M_WriteDate(MCP7940M *p_mcp7940m);
void MCP7940M_WriteMonth(MCP7940M *p_mcp7940m);
void MCP7940M_WriteYear(MCP7940M *p_mcp7940m);

/*
 * UTILITY FUNCTIONS
 */
uint8_t BCDToBinary(uint8_t bcd);
uint8_t binaryToBCD(uint8_t binary);

#endif /* INC_MCP7940M_H_ */
