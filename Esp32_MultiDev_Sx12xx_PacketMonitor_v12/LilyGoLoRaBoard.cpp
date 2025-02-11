/**
 * @file      boards.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-04-24
 * @last-update 2024-08-07
 *
 */

#include "LilyGoLoRaBoard.h"

#if defined(ARDUINO_ARCH_ESP32)
#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,0,0))
#include "hal/gpio_hal.h"
#endif
#include "driver/gpio.h"
#endif //ARDUINO_ARCH_ESP32

//void setupBoards(bool disable_u8g2 )
void setupLilyGoBoard()
{
    Serial.begin(115200);

    // while (!Serial);

    Serial.println("setupBoards done");

    //Serial.println("init done . ");
}

