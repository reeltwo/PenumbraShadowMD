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
#define RXD2_PIN                16
#define TXD2_PIN                17
#define RXD3_PIN                32
#define TXD3_PIN                4

#define OUTPUT_ENABLE_PIN       27
#define RS485_RTS_PIN           26

#define MD_SERIAL_RX            RXD1_PIN
#define MD_SERIAL_TX            TXD1_PIN

#define SABERTOOTH_SERIAL_RX    RXD2_PIN
#define SABERTOOTH_SERIAL_TX    TXD2_PIN

#define BODY_MD_SERIAL_RX       RXD3_PIN
#define BODY_MD_SERIAL_TX       TXD3_PIN

#define MD_SERIAL               Serial1
#define SABERTOOTH_SERIAL       Serial2
#define BODY_MD_SERIAL          bodyMDSerial

#define MD_SERIAL_INIT(baud)         MD_SERIAL.begin(baud, SERIAL_8N1, MD_SERIAL_RX, MD_SERIAL_TX)
#define SABERTOOTH_SERIAL_INIT(baud) SABERTOOTH_SERIAL.begin(baud, SERIAL_8N1, SABERTOOTH_SERIAL_RX, SABERTOOTH_SERIAL_TX)
#define BODY_MD_SERIAL_INIT(baud)    BODY_MD_SERIAL.begin(baud, SWSERIAL_8N1, BODY_MD_SERIAL_RX, BODY_MD_SERIAL_TX, false)

SoftwareSerial BODY_MD_SERIAL;
