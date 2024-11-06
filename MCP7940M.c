/*
 * MCP7940M.c
 *
 * This file contains the implementation of the driver for the MCP7940M Real-Time Clock (RTC) using the STM32F429 microcontroller.
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

#include "MCP7940M.h"

/*
 * INITIALISATION
 */

/**
 * @brief  Initializes the MCP7940M
 * @param  p_mcp7940m Pointer to a MCP7940M structure that will contain
 *                  our MCP7940M data.
 * @param  p_i2cHandle Pointer to our I2C handle
 */
void MCP7940M_Init(MCP7940M *p_mcp7940m, I2C_HandleTypeDef *p_i2cHandle)
{
    p_mcp7940m->i2cHandle = p_i2cHandle;

    p_mcp7940m->seconds = 0;
    p_mcp7940m->minutes = 0;
    p_mcp7940m->hours = 0;
    p_mcp7940m->weekday = MONDAY;
    p_mcp7940m->date = 1;
    p_mcp7940m->month = 1;
    p_mcp7940m->year = 0;

    /* Enable Oscillator */
    MCP7940M_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCSEC, 1 << 7);
}

/**
 * @brief  Gets the time from the MCP7940M
 * @param  p_mcp7940m Pointer to a MCP7940M structure that will
 *                    store our MCP7940M data.
 * @retval HAL_OK if success, HAL_ERROR if failed.
 */
HAL_StatusTypeDef MCP7940M_GetTime(MCP7940M *p_mcp7940m)
{
    MCP7940M_ReadSeconds(p_mcp7940m);
    MCP7940M_ReadMinutes(p_mcp7940m);
    MCP7940M_ReadHours(p_mcp7940m);
    MCP7940M_ReadWeekday(p_mcp7940m);
    MCP7940M_ReadDate(p_mcp7940m);
    MCP7940M_ReadMonth(p_mcp7940m);
    MCP7940M_ReadYear(p_mcp7940m);
    return HAL_OK;
}

/**
 * @brief  Writes the time to the MCP7940M
 * @param  p_mcp7940m Pointer to a MCP7940M structure that contains
 *                  the data we ant to write to our MCP7940M.
 * @retval HAL_OK if success, HAL_ERROR if failed.
 */
HAL_StatusTypeDef MCP7940M_SetTime(MCP7940M *p_mcp7940m)
{
    MCP7940M_WriteSeconds(p_mcp7940m);
    MCP7940M_WriteMinutes(p_mcp7940m);
    MCP7940M_WriteHours(p_mcp7940m);
    MCP7940M_WriteWeekday(p_mcp7940m);
    MCP7940M_WriteDate(p_mcp7940m);
    MCP7940M_WriteMonth(p_mcp7940m);
    MCP7940M_WriteYear(p_mcp7940m);
    return HAL_OK;
}

/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef MCP7940M_ReadRegister(MCP7940M *p_mcp7940m, uint8_t reg, uint8_t *p_data)
{
    return HAL_I2C_Mem_Read(p_mcp7940m->i2cHandle, MCP7940M_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, p_data, 1, 1000);
}

HAL_StatusTypeDef MCP7940M_WriteRegister(MCP7940M *p_mcp7940m, uint8_t reg, uint8_t data)
{
    return HAL_I2C_Mem_Write(p_mcp7940m->i2cHandle, MCP7940M_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

void MCP7940M_ReadSeconds(MCP7940M *p_mcp7940m)
{
    uint8_t seconds;
    uint8_t bcdSeconds;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCSEC, &bcdSeconds);
    bcdSeconds &= 0b01111111; // Discard the ST bit.
    seconds = BCDToBinary(bcdSeconds);
    p_mcp7940m->seconds = seconds;
}

void MCP7940M_ReadMinutes(MCP7940M *p_mcp7940m)
{
    uint8_t minutes;
    uint8_t bcdMinutes;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCMIN, &bcdMinutes);
    minutes = BCDToBinary(bcdMinutes);
    p_mcp7940m->minutes = minutes;
}

void MCP7940M_ReadHours(MCP7940M *p_mcp7940m)
{
    uint8_t hours;
    uint8_t bcdHours;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCHOUR, &bcdHours);
    bcdHours &= 0b00111111; // Discard the 12/24 bit.
    hours = BCDToBinary(bcdHours);
    p_mcp7940m->hours = hours;
}

void MCP7940M_ReadWeekday(MCP7940M *p_mcp7940m)
{
    uint8_t weekday;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCWKDAY, &weekday);
    weekday &= 0b00000111; // Discard the OSCRUN bit.
    p_mcp7940m->weekday = weekday;
}

void MCP7940M_ReadDate(MCP7940M *p_mcp7940m)
{
    uint8_t date;
    uint8_t bcdDate;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCDATE, &bcdDate);
    date = BCDToBinary(bcdDate);
    p_mcp7940m->date = date;
}

void MCP7940M_ReadMonth(MCP7940M *p_mcp7940m)
{
    uint8_t month;
    uint8_t bcdMonth;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCMTH, &bcdMonth);
    bcdMonth &= 0b00011111; // Discard the LP bit.
    month = BCDToBinary(bcdMonth);
    p_mcp7940m->month = month;
}

void MCP7940M_ReadYear(MCP7940M *p_mcp7940m)
{
    uint8_t year;
    uint8_t bcdYear;
    HAL_ReadRegister(p_mcp7940m, MCP7940M_REG_RTCYEAR, &bcdYear);
    year = BCDToBinary(bcdYear);
    p_mcp7940m->year = year;
}

void MCP7940M_WriteSeconds(MCP7940M *p_mcp7940m)
{
    uint8_t bcdSeconds = binaryToBCD(p_mcp7940m->seconds);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCSEC, bcdSeconds | 1 << 7); // Set the ST bit.
}

void MCP7940M_WriteMinutes(MCP7940M *p_mcp7940m)
{
    uint8_t bcdMinutes = binaryToBCD(p_mcp7940m->minutes);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCMIN, bcdMinutes);
}

void MCP7940M_WriteHours(MCP7940M *p_mcp7940m)
{
    uint8_t bcdHours = binaryToBCD(p_mcp7940m->hours);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCHOUR, bcdHours & ~(1 << 6)); // Clear the 12/24 bit.
}

void MCP7940M_WriteWeekday(MCP7940M *p_mcp7940m)
{
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCWKDAY, p_mcp7940m->weekday);
}

void MCP7940M_WriteDate(MCP7940M *p_mcp7940m)
{
    uint8_t bcdDate = binaryToBCD(p_mcp7940m->date);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCDATE, bcdDate);
}

void MCP7940M_WriteMonth(MCP7940M *p_mcp7940m)
{
    uint8_t bcdMonth = binaryToBCD(p_mcp7940m->month);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCMTH, bcdMonth & ~(1 << 5)); // Clear the LP bit.
}

void MCP7940M_WriteYear(MCP7940M *p_mcp7940m)
{
    uint8_t bcdYear = binaryToBCD(p_mcp7940m->year);
    HAL_WriteRegister(p_mcp7940m, MCP7940M_REG_RTCYEAR, bcdYear);
}

uint8_t BCDToBinary(uint8_t bcd)
{
    uint8_t tens = bcd >> 4;
    uint8_t ones = bcd & 0b00001111;
    return tens * 10 + ones;
}

uint8_t binaryToBCD(uint8_t binary)
{
    uint8_t tens = binary / 10;
    uint8_t ones = binary % 10;
    return (tens << 4) | ones;
}