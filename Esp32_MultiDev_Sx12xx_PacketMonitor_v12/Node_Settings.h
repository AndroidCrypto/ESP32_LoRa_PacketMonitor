/**
* This file defines the node specific data that should be changed for each node (sensor).
* 
*/

// ------------------------------------------------------------------
// sketch specific settings

// ------------------------------------------------------------------
// node settings
#define NODE_NUMBER 0x67     // the address of the sender node
#define RECEIVER_NUMBER 0x99 // the address of the receiver node

// select flipped orientation if uncommented
//#define DISPLAY_ORIENTATION_FLIPPED   // display 180 degrees right rotated 

// ------------------------------------------------------------------
// RC request type
// defines what command and data is following in the request
#define RC_NODE_NUMBER 0xF0      // the address of the sender = remote controler node
#define RC_RECEIVER_NUMBER 0x67  // the address of the receiver node
#define SEND_SENSOR_DATA 0x01
#define FREQUENCY_CHANGE 0x02
#define SPREADING_FACTOR_CHANGE 0x03
#define TXPOWER_CHANGE 0x04
#define MEASURE_BATTERY_VOLTAGE 0x05
#define SET_FACTORY_SETTINGS 0x06
#define DISPLAY_SETTINGS 0x07

// ------------------------------------------------------------------
// *******  AES Keys ***************

// AES-256 Key => replace with your own
#define AES_KEY_67     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, \
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, \
                    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F

// IV => should be a real random value
#define AES_IV_67      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F