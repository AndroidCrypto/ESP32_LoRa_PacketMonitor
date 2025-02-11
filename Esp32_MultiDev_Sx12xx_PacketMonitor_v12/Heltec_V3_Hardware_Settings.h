/**
* This are the hardware definitions for a
* Heltec WiFi LoRa 32 V3 Development board
* The board has onboard a LoRa module 'SX1262', an OLED display with 128 * 64 px and an LED
* Additionally the board has a battery connector with included voltage metering and the
* possibility to shut down external devices by setting the 3.3 power to off.
* The microcontroller is an ESP-S3 module with 8 MB RAM.
*/

// LoRa module
#define SX_NSS 8      // select pin on LoRa device
#define SX_SCK 9       // SCK on SPI3
#define SX_MISO 11     // MISO on SPI3 
#define SX_MOSI 10     // MOSI on SPI3 
#define SX_NRESET 12   // reset pin on LoRa device
#define SX_RFBUSY 13   // busy line, not available
#define SX_DIO1 14     // DIO1 pin on LoRa device, used for RX and TX done 
#define LORA_DEVICE DEVICE_SX1262 // we need to define the device we are using

// OLED module
#define IS_OLED // uncomment this if not included
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_SDA_PIN 17
#define OLED_I2C_SCL_PIN 18
#define OLED_I2C_RST_PIN 21 // set this to -1 if the OLED display has no RST terminal connected

// LED, if no LED is attached or unwanted set this to -1
#define LED_PIN 35     // on board LED, high for on

// Buzzer
#define BUZZER_PIN -1   // pin for buzzer, set to -1 if not used 

// BOOT or USR or PRG button pin or any other button attached to the device
#define BOOT_BUTTON_PIN 0

// Control external power
#define IS_VEXT_CONTROL // uncomment this if not available
#define VEXT_POWER_CONTROL_PIN 36 // pin controls power to external devices
#define BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN 37 // pin controls the measuring of the battery voltage
#define BATTERY_VOLTAGE_ADC_PIN 1 // adc pin to measure the battery voltage

// BME280 temperature, humidity and barometric air pressure sensor
//#define IS_BME280
// a note on the PIN definitions: usually you will use the same I2C pins as used for the display
// but as the I2C pins for the regular bus are not available we use alternative pin ones
#define IS_SECOND_I2C_BUS
#define BME280_I2C_SDA_PIN 41
#define BME280_I2C_SCL_PIN 42
#define BME280_ADDRESS 0x76

// NEO-6M GPS module
//#define IS_NEO_6M
#define NEO6M_GPS_RX_PIN 46 // The GPS board has an RX terminal - connect it with GPIO 46
#define NEO6M_GPS_TX_PIN 45  // The GPS board has a TX terminal - connect it with GPIO 45
