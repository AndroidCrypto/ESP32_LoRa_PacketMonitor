//*******  Setup LoRa Parameters Here ! ***************

//LoRa Modem Parameters
const uint32_t FREQUENCY = 863100000;           // This is band K // frequency of transmissions in hertz
const uint32_t OFFSET = 0;                      // offset frequency for calibration purposes
const uint8_t BANDWIDTH = LORA_BW_125;          // LoRa bandwidth
const uint8_t SPREADING_FACTOR = LORA_SF7;       // LoRa spreading factor
//const uint8_t SPREADING_FACTOR = LORA_SF12;       // LoRa spreading factor
const uint8_t CODE_RATE = LORA_CR_4_5;           // LoRa coding rate
const uint8_t OPTIMISATION = LDRO_AUTO;         // low data rate optimisation setting, normally set to auto

const int8_t TX_POWER = 10;                      // LoRa transmit power in dBm, maximum in Europe is 14, not used in this sketch
