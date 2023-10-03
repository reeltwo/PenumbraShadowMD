////////////////////////////////////////////
// CONTROL BOARD PIN OUT
////////////////////////////////////////////
// Only change if you using a different PCB
////////////////////////////////////////////

#define DIN1_PIN                34
#define DIN2_PIN                35

#define DOUT1_PIN               14
#define DOUT2_PIN               13

#define RXD1_PIN                33
#define TXD1_PIN                25
// ERRATA: Pin 17 is not connected. Serial2 can either be RX or TX but not both.
#define RXD2_PIN                17
#define TXD2_PIN                16
#define RXD3_PIN                32
#define TXD3_PIN                4

#define OUTPUT_ENABLE_PIN       27
#define RS485_RTS_PIN           26

#define MD_SERIAL_RX            RXD1_PIN
#define MD_SERIAL_TX            TXD1_PIN

#define BODY_MD_SERIAL_RX       RXD2_PIN
#define BODY_MD_SERIAL_TX       TXD2_PIN

#define MOTOR_SERIAL_RX         RXD3_PIN
#define MOTOR_SERIAL_TX         TXD3_PIN

#define SOUND_SERIAL_TX         DOUT1_PIN
#define SOUND_SERIAL_RX         DIN2_PIN

#define MD_SERIAL               Serial1
#define MOTOR_SERIAL            motorSerial
#define BODY_MD_SERIAL          Serial2
#define SOUND_SERIAL            soundSerial

#define MD_SERIAL_INIT(baud)         MD_SERIAL.begin(baud, SERIAL_8N1, MD_SERIAL_RX, MD_SERIAL_TX)
#define BODY_MD_SERIAL_INIT(baud)    BODY_MD_SERIAL.begin(baud, SERIAL_8N1, BODY_MD_SERIAL_RX, BODY_MD_SERIAL_TX)
#define MOTOR_SERIAL_INIT(baud)      MOTOR_SERIAL.begin(baud, SWSERIAL_8N1, -1, MOTOR_SERIAL_TX, false)
#define SOUND_SERIAL_INIT(baud)      { SOUND_SERIAL.begin(baud, SWSERIAL_8N1, SOUND_SERIAL_RX, SOUND_SERIAL_TX, false); delay(1500); }

#define SOUND_SERIAL_BAUD       9600
#define SOUND_DEFAULT_VOLUME    50

#include "SoftwareSerial.h"
SoftwareSerial MOTOR_SERIAL;
SoftwareSerial SOUND_SERIAL;
