# MCP7940M Driver for STM32F429

This repository contains the driver for the MCP7940M Real-Time Clock (RTC) using the STM32F429 microcontroller. The driver provides functions to initialize the RTC, set and get the time, and read/write individual time registers. It uses the HAL library for I2C communication.

## Installation
To use this driver, include `MCP7940M.h` and `MCP7940M.c` in your project and ensure that the HAL library for I2C communication is available.

## Usage
- Create a `MCP7940M` struct.
- Initialize the driver `MCP7940M_Init(&your_mcp_struct, &hi2c1);` (Pass the struct and i2c handle as arguments.)
- Set the time using `MCP7940M_SetTime(&your_mcp_struct);` (This uploads the struct to the MCP7940M.)
- Get the current time using `MCP7940M_GetTime(&your_mcp_struct);` (This updates the struct with the current time from the MCP7940M.)

### Example code

```
#include "MCP7940M.h"

int main(void)
{
  MCP7940M mcp7940m;    // Create a MCP7940M struct
  char string[9]        // Create a character array to store our string.

  // Initialize MCP7940M Driver.
  MCP7940M_Init(&mcp7940m, &hi2c1);

  while (1) 
  {
    // Get current time from the MCP7940M chip
    MCP7940M_GetTime(&mcp7940m);

    // Convert the timestamp to readable string.
    sprintf(string, "%02d:%02d:%02d", mcp7940m.hours, mcp7940m.minutes, mcp7940m.seconds);

    // Print the string to serial or screen ect...
    PrintExample(string);

    HAL_Delay(100);
  }
}
```
