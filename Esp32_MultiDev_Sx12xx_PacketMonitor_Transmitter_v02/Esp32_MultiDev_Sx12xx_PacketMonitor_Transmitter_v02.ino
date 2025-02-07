
/*
Version Management
07.02.2025 V02 final version
06.02.2025 V01 First version 
*/

/**
* Please uncomment just one hardware definition file that reflects your hardware combination
* for Heltec WiFi LoRa 32 V2 boards use HELTEC_V2
* for Heltec WiFi LoRa 32 V3 boards use HELTEC_V3
* for ESP32 Development boards with attached LoRa module SX1276 module and OLED use ESP32_SX1276_OLED
* for ESP32 Development boards with attached LoRa module SX1276 module and TFT use ESP32_SX1276_TFT
* for all other boards and hardware combination you should consider to modify an existing one to your needs
*
* Don't forget to change the Board in Arduino:
* for Heltec V2: Heltec WiFi LoRa 32(V2)
* for Heltec V3: Heltec WiFi LoRa 32(V3) / Wireless shell (V3) / ...
* or ESP32 Development Boards: ESP32-WROOM-DA Module
*
* - or in Tools menue:
* for Heltec V2: Tools - Board - esp32 - Heltec WiFi LoRa 32(V2)
* for Heltec V3: Tools - Board - esp32 - Heltec WiFi LoRa 32(V3) / Wireless shell (V3) / ...
* for ESP32 Development Boards: Tools - Board - esp32 - ESP32-WROOM-DA Module
*
*/

#define HELTEC_V2
//#define HELTEC_V3
//#define ESP32_SX1276_OLED
//#define ESP32_SX1276_TFT

// ------------------------------------------------------------------
// include the hardware definition files depending on the uncommenting
#ifdef HELTEC_V2
#include "Heltec_V2_Hardware_Settings.h"
#endif

#ifdef HELTEC_V3
#include "Heltec_V3_Hardware_Settings.h"
#endif

#ifdef ESP32_SX1276_OLED
#include "ESP32_SX1276_OLED_Hardware_Settings.h"
#endif

#ifdef ESP32_SX1276_TFT
#include "ESP32_SX1276_TFT_Hardware_Settings.h"
#endif

// ------------------------------------------------------------------

// when using the (default) OLED display SSD1306 128 * 64 px the maximum length is 26 chars
const String PROGRAM_VERSION = "PacketMonitor Transm. V02";

// ------------------------------------------------------------------
// internal or external OLED SSD1306 128 * 64 px display

#ifdef IS_OLED
#include "FONT_MONOSPACE_9.h"
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>
#include "SSD1306.h"  // https://github.com/ThingPulse/esp8266-oled-ssd1306
SSD1306Wire display(OLED_I2C_ADDRESS, OLED_I2C_SDA_PIN, OLED_I2C_SCL_PIN);
#endif

#ifdef IS_TFT
// ------------------------------------------------------------------
// TFT display ST7735 1.8' 128 * 160 RGB
#include "FONT_MONOSPACE_9.h"
#include <SPI.h>
#include <Adafruit_GFX.h>                                        // Core graphics library, https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ST7735.h>                                     // Hardware-specific library for ST7735, https://github.com/adafruit/Adafruit-ST7735-Library
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);  // hardware SPI
#endif

// vars for displaying line 1 to 5 to display in a loop
String display1 = "";
String display2 = "";
String display3 = "";
String display4 = "";
String display5 = "";
// for TFT only
String display6, display7, display8, display9, display10, display11, display12, display13;

bool showDisplay = false;
bool isDisplayOn = false;  // this bool is needed for switching the display off after timer exceeds

// -----------------------------------------------------------------------
// https://github.com/StuartsProjects/SX12XX-LoRa

#include <SPI.h>

#ifdef HELTEC_V2
#include <SX127XLT.h>  //include the appropriate library
SX127XLT LT;           //create a library class instance called LT
#endif

#ifdef HELTEC_V3
#include <SX126XLT.h>  //include the appropriate library
SX126XLT LT;           //create a library class instance called LT
#endif

#ifdef ESP32_SX1276_OLED
#include <SX127XLT.h>  //include the appropriate library
SX127XLT LT;           //create a library class instance called LT
#endif

#ifdef ESP32_SX1276_TFT
#include <SX127XLT.h>  //include the appropriate library
SX127XLT LT;           //create a library class instance called LT
#endif

#include "LoRa_Settings.h"  // include the setttings file, LoRa frequencies, txpower etc
#include "Node_Settings.h"  // include the node/sketch specific settings

uint32_t TXPacketCounter;
uint8_t TXPacketL;

// vars for sending

// measure the transmission time in milliseconds
long startTransmissionMillis = 0;
long endTransmissionMillis = 0;
long durationTransmissionMillis = 0;

// -----------------------------------------------------------------------
// PRG/Boot button
// #define BOOT_BUTTON_PIN 0 // see settings or hardware settings
#include <HotButton.h> // https://github.com/ropg/HotButton
HotButton bootButton(BOOT_BUTTON_PIN);

boolean isSettingsMode = false;
uint8_t modeCounter = 0;  // just a counter
uint8_t currentPayloadSize = 4; // start/default
uint8_t nextPayloadSize = 16;

// -----------------------------------------------------------------------
// random data
#include "bootloader_random.h" // for true random RNG

void loop() {

  bootButton.update();
  if (bootButton.isSingleClick()) {
    Serial.println("The button was clicked.");
    if (!isSettingsMode) {
      singleDataTransmitting();
    } else {
      // in settings mode
      // this will increase the payload size with each single click
      // steps: 4 / 16 / 32 / 64 / 128 / 250 bytes, the 4 again..
      modeCounter ++;
      if (modeCounter == 6) modeCounter = 0; // roll over
      if (modeCounter == 0) {
        currentPayloadSize = 4;
        nextPayloadSize = 16;
      } else if (modeCounter == 1) {
        currentPayloadSize = 16;
        nextPayloadSize = 32;
      } else if (modeCounter == 2) {
        currentPayloadSize = 32;
        nextPayloadSize = 64;
      } else if (modeCounter == 3) {
        currentPayloadSize = 64;
        nextPayloadSize = 128;
      } else if (modeCounter == 4) {
        currentPayloadSize = 128;
        nextPayloadSize = 250;
      } else if (modeCounter == 5) {
        currentPayloadSize = 250;
        nextPayloadSize = 4;
      }
      display2 = "Single click: change size";
      display3 = "Current size: " + (String) currentPayloadSize + " bytes";
      display4 = "Next size:    " + (String) nextPayloadSize + " bytes";
      display5 = "Double: save setings";
      displayData();
    }  
  }
  if (bootButton.isDoubleClick()) {
    Serial.println("The button was double-clicked.");
    isSettingsMode = !isSettingsMode;
    if (!isSettingsMode) {
      // we left the settings with a double click
      // show short information
      clearDisplayData();
      display1 = PROGRAM_VERSION;
      display3 = "Current size: " + (String) currentPayloadSize + " bytes";
      display5 = "Single click:Transmitting";
      displayData();
    }
  }

  if (isSettingsMode) {
    //clearDisplayData();
    display2 = "Single click: change size";
    display3 = "Current size: " + (String) currentPayloadSize + " bytes";
    display4 = "Next size:    " + (String) nextPayloadSize + " bytes";
    display5 = "double: save setings";
    displayData();
  }
}

void singleDataTransmitting() {
  uint8_t len;
  uint16_t IRQStatus;

  TXPacketCounter++;
  Serial.print(TXPacketCounter);  //print the numbers of sends
  Serial.print(F(" Sending > "));

  // prepairing the buffer
  uint8_t buff[255];
  uint8_t bufferSize = currentPayloadSize;
  // fill the array with random data
  bootloader_fill_random(buff, 255);
  // the first two bytes are reserved for the TXPacketCounter value
  buff[0] = TXPacketCounter & 0xFF;
  buff[1] = (TXPacketCounter >> 8) & 0xFF;
  // the third byte is the payload size
  buff[2] = bufferSize;

  startTransmissionMillis = millis();
  TXPacketL = LT.transmit(buff, bufferSize, 10000, TX_POWER, WAIT_TX);
  endTransmissionMillis = millis();
  durationTransmissionMillis = endTransmissionMillis - startTransmissionMillis;
  Serial.print(F("TX duration: "));
  Serial.print(durationTransmissionMillis) + Serial.println(F(" ms"));  // print the numbers of seconds

  if (TXPacketL) {
    ledFlash(1, 125);
    Serial.println(" SUCCESS");
    clearDisplayData();
    display1 = PROGRAM_VERSION;
    display2 = "Frequency: " + (String) (FREQUENCY / 1000) + " Khz";
    display3 = "TXCounter: " + (String) TXPacketCounter + " | Size: " + (String) currentPayloadSize;
    display4 = "  * SUCCESS *";
    display5 = "TX duration: " + (String)durationTransmissionMillis + " ms";
    displayData();
  } else {
    ledFlash(3, 125);
    Serial.println(" FAILURE");
    display1 = PROGRAM_VERSION;
    display2 = "Frequency: " + (String) (FREQUENCY / 1000) + " Khz";
    display3 = "TXCounter: " + (String) TXPacketCounter + " | Size: " + (String) currentPayloadSize;
    display4 = "  * FAILURE *";
    display5 = "TX duration: " + (String)durationTransmissionMillis + " ms";
    displayData();
  }
}

void ledFlash(uint16_t flashes, uint16_t delaymS) {
  // run only if a LED is connected
  if (LED_PIN >= 0) {
    uint16_t index;
    for (index = 1; index <= flashes; index++) {
      digitalWrite(LED_PIN, HIGH);
      delay(delaymS);
      digitalWrite(LED_PIN, LOW);
      delay(delaymS);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  Serial.println(F("ESP32 MD LoRa Packet Monitor Transmitter V02"));

  // for random data
  bootloader_random_enable();

  // if we have a power control for devices put it on
#ifdef IS_VEXT_CONTROL
  setVextControl(true);
#endif

  if (LED_PIN >= 0) {
    pinMode(LED_PIN, OUTPUT);  // setup pin as output for indicator LED
    ledFlash(1, 125);         // two quick LED flashes to indicate program start
  }

  SPI.begin();

  // setup display
#ifdef IS_OLED
  if (OLED_I2C_RST_PIN >= 0) {
    pinMode(OLED_I2C_RST_PIN, OUTPUT);
    digitalWrite(OLED_I2C_RST_PIN, LOW);  // set GPIO16 low to reset OLED
    delay(50);
    digitalWrite(OLED_I2C_RST_PIN, HIGH);
    delay(50);
  }
  clearDisplayData();
  display.init();
#ifdef DISPLAY_ORIENTATION_FLIPPED  
  // do nothing
#else
  display.flipScreenVertically(); // Landscape 90 degrees right rotated
#endif  
  display.setFont(ArialMT_Plain_10);
  delay(50);
  display1 = PROGRAM_VERSION;
  //display.drawString(0, 0, PROGRAM_VERSION);
  //display.display();
  displayData();
  delay(500);
#endif

  // init TFT display
#ifdef IS_TFT
  tft.initR(INITR_BLACKTAB);     // den ST7735S Chip initialisieren, schwarz
  tft.fillScreen(ST77XX_BLACK);  // und den Schirm mit Schwarz füllen
  tft.setTextWrap(false);        // automatischen Zeilenumbruch ausschalten
#ifdef DISPLAY_ORIENTATION_FLIPPED  
  tft.setRotation(1);            // Landscape 270 degrees right rotated
#else
  tft.setRotation(3);            // Landscape 90 degrees right rotated
#endif  
  Serial.println(F("Display init done"));
  display1 = PROGRAM_VERSION;
  displayData();
  delay(500);
#endif

  display2 = "Display init done";
  displayData();
  delay(1000);

  // setup the LoRa device
  //SPI.begin();

#ifdef HELTEC_V2
  if (LT.begin(NSS, NRESET, DIO0, LORA_DEVICE)) {
    Serial.println(F("LoRa Device Heltec V2 found"));
    display3 = "LoRa Device HV2 found";
    displayData();
    ledFlash(2, 125);
  } else {
    Serial.println(F("Device error"));
    while (1) {
      Serial.println(F("No device responding"));
      display3 = "No LoRa Device found";
      displayData();
      ledFlash(50, 50);  // long fast speed flash indicates LoRa device error
    }
  }
#endif

#ifdef HELTEC_V3
  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, LORA_DEVICE)) {
    Serial.println(F("LoRa Device Heltec V3 found"));
    display3 = "LoRa Device HV3 found";
    displayData();
    ledFlash(1, 125);
  } else {
    Serial.println(F("Device error"));
    while (1) {
      Serial.println(F("No device responding"));
      display3 = "No LoRa Device found";
      displayData();
      ledFlash(50, 50);  // long fast speed flash indicates LoRa device error
    }
  }
  // The Heltec V3 board uses an unusual crystal voltage. Somme errors came up
  // when using Reliable communication so I'm setting the value here.
  LT.setDIO3AsTCXOCtrl(TCXO_CTRL_1_8V);
#endif

#ifdef ESP32_SX1276_OLED
  if (LT.begin(NSS, NRESET, DIO0, LORA_DEVICE)) {
    Serial.println(F("LoRa Device ESP32/SX1276 found"));
    display3 = "LoRa Dev.ESP32+SX1276";
    displayData();
    ledFlash(2, 125);
  } else {
    Serial.println(F("Device error"));
    while (1) {
      Serial.println(F("No device responding"));
      display3 = "No LoRa Device found";
      displayData();
      ledFlash(50, 50);  // long fast speed flash indicates LoRa device error
    }
  }
#endif

#ifdef ESP32_SX1276_TFT
  if (LT.begin(NSS, NRESET, DIO0, LORA_DEVICE)) {
    Serial.println(F("LoRa Device ESP32/SX1276 found"));
    display3 = "LoRa Dev.ESP32+SX1276";
    displayData();
    ledFlash(2, 125);
  } else {
    Serial.println(F("Device error"));
    while (1) {
      Serial.println(F("No device responding"));
      display3 = "No LoRa Device found";
      displayData();
      ledFlash(50, 50);  // long fast speed flash indicates LoRa device error
    }
  }
#endif

  // just to be for sure - use the default syncword
  LT.setSyncWord(LORA_MAC_PRIVATE_SYNCWORD); // this is the default value
  // set the high sensitive mode
  // Sets LoRa device for the highest sensitivity at expense of slightly higher LNA current. 
  // The alternative is setLowPowerReceive() for lower sensitivity with slightly reduced current.
  LT.setHighSensitivity();
  // start the device with default parameters
  LT.setupLoRa(FREQUENCY, OFFSET, SPREADING_FACTOR, BANDWIDTH, CODE_RATE, OPTIMISATION);
  Serial.println(F("LoRa setup is complete"));

  // debug information
  Serial.println();
  LT.printModemSettings();
  Serial.println();
  LT.printOperatingSettings();
  Serial.println();

  display1 = PROGRAM_VERSION;
  display2 = "Fqcy: " + (String) (FREQUENCY / 1000) + " kHz | SF: " + (String) SPREADING_FACTOR;
  display3 = "   Press BOOT button";
  display4 = "once:   Transmitting";
  display5 = "double: Change pkt.size";
  displayData();
  delay(1000);

}

void displayData() {
#ifdef IS_TFT
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setFont(NULL);  // Pass NULL to revert to 'classic' fixed-space bitmap font.
  tft.setCursor(0, 0);
  tft.print(display1);
  tft.setCursor(0, 10);
  tft.print(display2);
  tft.setCursor(0, 20);
  tft.print(display3);
  tft.setCursor(0, 30);
  tft.print(display4);
  tft.setCursor(0, 40);
  tft.print(display5);
  tft.setCursor(0, 50);
  tft.print(display6);
  tft.setCursor(0, 60);
  tft.print(display7);
  tft.setCursor(0, 70);
  tft.print(display8);
  tft.setCursor(0, 80);
  tft.print(display9);
  tft.setCursor(0, 90);
  tft.print(display10);
  tft.setCursor(0, 100);
  tft.print(display11);
  tft.setCursor(0, 110);
  tft.print(display12);
  tft.setCursor(0, 120);
  tft.print(display13);
#endif

#ifdef IS_OLED
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Monospaced_plain_9);
  display.drawString(0, 0, display1);
  display.drawString(0, 12, display2);
  display.drawString(0, 24, display3);
  display.drawString(0, 36, display4);
  display.drawString(0, 48, display5);
  display.display();
#endif
}

void clearDisplayData() {
  display1 = "";
  display2 = "";
  display3 = "";
  display4 = "";
  display5 = "";
  display6 = "";
  display7 = "";
  display8 = "";
  display9 = "";
  display10 = "";
  display11 = "";
  display12 = "";
  display13 = "";
}

void setVextControl(boolean trueIsOn) {
#ifdef IS_VEXT_CONTROL
  if (trueIsOn) {
    pinMode(VEXT_POWER_CONTROL_PIN, OUTPUT);
    digitalWrite(VEXT_POWER_CONTROL_PIN, LOW);
  } else {
    // pulled up, no need to drive it
    pinMode(VEXT_POWER_CONTROL_PIN, INPUT);
  }
#endif
}
