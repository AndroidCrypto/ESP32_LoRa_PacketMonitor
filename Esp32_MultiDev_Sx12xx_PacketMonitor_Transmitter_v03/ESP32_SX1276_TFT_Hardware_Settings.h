/**
* This are the hardware definitions for an
* ESP32 Development board.
* The board has an external LoRa module 'SX1276', an external TFT display with 160 * 128 px 
* and no onboard LED.
* There are no battery voltage meterings and power control added. 
* External I2C sensors like a BME280 sensor are connected using the default I2C pins.
*/

// LoRa module
#define SX_NSS 5       // select pin on LoRa device
#define SX_SCK 18      // SCK
#define SX_MISO 19     // MISO 
#define SX_MOSI 23     // MOSI
#define SX_NRESET 17   // reset pin on LoRa device
#define SX_RFBUSY -1   // busy line, not available
#define SX_DIO0 4      // DIO0 pin on LoRa device, used for RX and TX done 
#define LORA_DEVICE DEVICE_SX1276 // we need to define the device we are using

// TFT Display 1.8' 160 * 128 px
/*
Wiring TFT-Display with ESP32
TFT  - ESP32
GND  - GND
VDD  - 3.3V
SCL  - 18
SDA  - 23
RST  - 4
DC   - 2
CS   - 15
BLK  - 3.3V Backlight 5V is brighter

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   16  // Data Command control pin
#define TFT_RST   2  // Reset pin (could connect to RST pin)
*/

#define IS_TFT // uncomment this if not included
#define TFT_MISO 19  // MISO not used
#define TFT_MOSI 23  // MOSI = SDA
#define TFT_CLK 18   // CLK = SLC
#define TFT_CS 15     // Chip select control pin
#define TFT_DC 16    // Data Command control pin
#define TFT_RST 2   // Reset pin (could connect to RST pin)

// OLED module
//#define IS_OLED // uncomment this if not included
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_SDA_PIN 21
#define OLED_I2C_SCL_PIN 22
#define OLED_I2C_RST_PIN -1 // set this to -1 if the OLED display has no RST terminal connected

// LED, if no LED is attached or unwanted set this to -1
// *** Warning *** do not use an onboard LED Pin (usually GPIO 2) as this is used for the TFT display/RST terminal.
#define LED_PIN -1     // on board LED, high for on

// BOOT or USR or PRG button pin or any other button attached to the device
#define BOOT_BUTTON_PIN 0

// Buzzer
#define BUZZER_PIN -1   // pin for buzzer, set to -1 if not used 

// Control external power
//#define IS_VEXT_CONTROL // uncomment this if not available
#define VEXT_POWER_CONTROL_PIN 21 // pin controls power to external devices

//#define IS_BATTERY_MANAGEMENT
#define BATTERY_VOLTAGE_ADC_MEASURE_CONTROL_PIN -1 // pin controls the measuring of the battery voltage
#define BATTERY_VOLTAGE_ADC_PIN -1 // adc pin to measure the battery voltage

// BME280 temperature, humidity and barometric air pressure sensor
//#define IS_BME280
// a note on the PIN definitions: usually you will use the same I2C pins as used for the display
#define BME280_I2C_SDA_PIN 21
#define BME280_I2C_SCL_PIN 22

// NEO-6M GPS module
//#define IS_NEO_6M
#define NEO6M_GPS_RX_PIN 12 // The GPS board has an RX terminal - connect it with GPIO 12
#define NEO6M_GPS_TX_PIN 13 // The GPS board has a  TX terminal - connect it with GPIO 13
