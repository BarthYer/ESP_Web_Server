


#ifndef __MCP9808__
#define __MCP9808__
#include<stdint.h>
#include<esp_err.h>
#include "sdkconfig.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOSConfig.h"
#include<freertos/FreeRTOS.h>
#include<freertos/FreeRTOSConfig.h>
#include<freertos/task.h>
#include"driver/gpio.h"

#define MCP_ADD 0x18
#define MCP_REG 0x05
#define TIMEOUT_MS 1000

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

esp_err_t  mcp9808_init(uint8_t addr, uint8_t add_reg, uint8_t sda_pin, uint8_t scl_pin) ;

esp_err_t mcp9808_read(float *temperature);

void mcp9808_test_main_c();

#endif
