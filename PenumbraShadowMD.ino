// =======================================================================================
//        SHADOW_MD:  Small Handheld Arduino Droid Operating Wand + MarcDuino
// =======================================================================================
//                          Last Revised Date: 08/23/2015
//                             Revised By: vint43
//                Inspired by the PADAWAN / KnightShade SHADOW effort
// =======================================================================================
//
//         This program is free software: you can redistribute it and/or modify it for
//         your personal use and the personal use of other astromech club members.  
//
//         This program is distributed in the hope that it will be useful 
//         as a courtesy to fellow astromech club members wanting to develop
//         their own droid control system.
//
//         IT IS OFFERED WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//         You are using this software at your own risk and, as a fellow club member, it is
//         expected you will have the proper experience / background to handle and manage that 
//         risk appropriately.  It is completely up to you to insure the safe operation of
//         your droid and to validate and test all aspects of your droid control system.
//
// =======================================================================================
//   Note: You will need a Arduino Mega ADK rev3 to run this sketch,
//   as a normal Arduino (Uno, Duemilanove etc.) doesn't have enough SRAM and FLASH
//
//   This is written to be a SPECIFIC Sketch - supporting only one type of controller
//      - PS3 Move Navigation + MarcDuino Dome Controller & Optional Body Panel Controller
//
//   PS3 Bluetooth library - developed by Kristian Lauszus (kristianl@tkjelectronics.com)
//   For more information visit my blog: http://blog.tkjelectronics.dk/ or
//
//   Sabertooth (Foot Drive):
//         Set Sabertooth 2x32 or 2x25 Dip Switches: 1 and 2 Down, All Others Up
//
//   SyRen 10 Dome Drive:
//         For SyRen packetized Serial Set Switches: 1, 2 and 4 Down, All Others Up
//         NOTE:  Support for SyRen Simple Serial has been removed, due to problems.
//         Please contact DimensionEngineering to get an RMA to flash your firmware
//         Some place a 10K ohm resistor between S1 & GND on the SyRen 10 itself
//
// =======================================================================================
//
// ---------------------------------------------------------------------------------------
//                        General User Settings
// ---------------------------------------------------------------------------------------

#define PANEL_COUNT 10                // Number of panels
#define USE_DEBUG                     // Define to enable debug diagnostic
#define USE_PREFERENCES

#define PS3_CONTROLLER_FOOT_MAC       "XX:XX:XX:XX:XX:XX"  //Set this to your FOOT PS3 controller MAC address
#define PS3_CONTROLLER_DOME_MAC       "XX:XX:XX:XX:XX:XX"  //Set to a secondary DOME PS3 controller MAC address (Optional)

String PS3ControllerFootMac = PS3_CONTROLLER_FOOT_MAC;
String PS3ControllerDomeMAC = PS3_CONTROLLER_DOME_MAC;

String PS3ControllerBackupFootMac = "XX";  //Set to the MAC Address of your BACKUP FOOT controller (Optional)
String PS3ControllerBackupDomeMAC = "XX";  //Set to the MAC Address of your BACKUP DOME controller (Optional)

byte drivespeed1 = 70;   //For Speed Setting (Normal): set this to whatever speeds works for you. 0-stop, 127-full speed.
byte drivespeed2 = 110;  //For Speed Setting (Over Throttle): set this for when needing extra power. 0-stop, 127-full speed.

byte turnspeed = 50;      // the higher this number the faster it will spin in place, lower - the easier to control.
                         // Recommend beginner: 40 to 50, experienced: 50+, I like 75

byte domespeed = 100;    // If using a speed controller for the dome, sets the top speed
                         // Use a number up to 127

byte ramping = 1;        // Ramping- the lower this number the longer R2 will take to speedup or slow down,
                         // change this by increments of 1

byte joystickFootDeadZoneRange = 15;  // For controllers that centering problems, use the lowest number with no drift
byte joystickDomeDeadZoneRange = 10;  // For controllers that centering problems, use the lowest number with no drift

byte driveDeadBandRange = 10;     // Used to set the Sabertooth DeadZone for foot motors

int invertTurnDirection = -1;   //This may need to be set to 1 for some configurations

byte domeAutoSpeed = 70;     // Speed used when dome automation is active - Valid Values: 50 - 100
int time360DomeTurn = 2500;  // milliseconds for dome to complete 360 turn at domeAutoSpeed - Valid Values: 2000 - 8000 (2000 = 2 seconds)

// #define SHADOW_DEBUG       //uncomment this for console DEBUG output
//#define SHADOW_VERBOSE     //uncomment this for console VERBOSE output

#define SHADOW_DEBUG(...) printf(__VA_ARGS__);
#define SHADOW_VERBOSE(...) printf(__VA_ARGS__);

// ---------------------------------------------------------------------------------------
//                          MarcDuino Button Settings
// ---------------------------------------------------------------------------------------
// Std MarcDuino Function Codes:
//     1 = Close All Panels
//     2 = Scream - all panels open
//     3 = Wave, One Panel at a time
//     4 = Fast (smirk) back and forth wave
//     5 = Wave 2, Open progressively all panels, then close one by one
//     6 = Beep cantina - w/ marching ants panel action
//     7 = Faint / Short Circuit
//     8 = Cantina Dance - orchestral, rhythmic panel dance
//     9 = Leia message
//    10 = Disco
//    11 = Quite mode reset (panel close, stop holos, stop sounds)
//    12 = Full Awake mode reset (panel close, rnd sound, holo move,holo lights off)
//    13 = Mid Awake mode reset (panel close, rnd sound, stop holos)
//    14 = Full Awake+ reset (panel close, rnd sound, holo move, holo lights on)
//    15 = Scream, with all panels open (NO SOUND)
//    16 = Wave, one panel at a time (NO SOUND)
//    17 = Fast (smirk) back and forth (NO SOUND)
//    18 = Wave 2 (Open progressively, then close one by one) (NO SOUND)
//    19 = Marching Ants (NO SOUND)
//    20 = Faint/Short Circuit (NO SOUND)
//    21 = Rhythmic cantina dance (NO SOUND)
//    22 = Random Holo Movement On (All) - No other actions
//    23 = Holo Lights On (All)
//    24 = Holo Lights Off (All)
//    25 = Holo reset (motion off, lights off)
//    26 = Volume Up
//    27 = Volume Down
//    28 = Volume Max
//    29 = Volume Mid
//    30 = Open All Dome Panels
//    31 = Open Top Dome Panels
//    32 = Open Bottom Dome Panels
//    33 = Close All Dome Panels
//    34 = Open Dome Panel #1
//    35 = Close Dome Panel #1
//    36 = Open Dome Panel #2
//    37 = Close Dome Panel #2
//    38 = Open Dome Panel #3
//    39 = Close Dome Panel #3
//    40 = Open Dome Panel #4
//    41 = Close Dome Panel #4
//    42 = Open Dome Panel #5
//    43 = Close Dome Panel #5
//    44 = Open Dome Panel #6
//    45 = Close Dome Panel #6
//    46 = Open Dome Panel #7
//    47 = Close Dome Panel #7
//    48 = Open Dome Panel #8
//    49 = Close Dome Panel #8
//    50 = Open Dome Panel #9
//    51 = Close Dome Panel #9
//    52 = Open Dome Panel #10
//    53 = Close Dome Panel #10
//   *** BODY PANEL OPTIONS ASSUME SECOND MARCDUINO MASTER BOARD ON MEGA ADK SERIAL #3 ***
//    54 = Open All Body Panels
//    55 = Close All Body Panels
//    56 = Open Body Panel #1
//    57 = Close Body Panel #1
//    58 = Open Body Panel #2
//    59 = Close Body Panel #2
//    60 = Open Body Panel #3
//    61 = Close Body Panel #3
//    62 = Open Body Panel #4
//    63 = Close Body Panel #4
//    64 = Open Body Panel #5
//    65 = Close Body Panel #5
//    66 = Open Body Panel #6
//    67 = Close Body Panel #6
//    68 = Open Body Panel #7
//    69 = Close Body Panel #7
//    70 = Open Body Panel #8
//    71 = Close Body Panel #8
//    72 = Open Body Panel #9
//    73 = Close Body Panel #9
//    74 = Open Body Panel #10
//    75 = Close Body Panel #10
//   *** MAGIC PANEL LIGHTING COMMANDS
//    76 = Magic Panel ON
//    77 = Magic Panel OFF
//    78 = Magic Panel Flicker (10 seconds) 
//
// Std MarcDuino Logic Display Functions (For custom functions)
//     1 = Display normal random sequence
//     2 = Short circuit (10 second display sequence)
//     3 = Scream (flashing light display sequence)
//     4 = Leia (34 second light sequence)
//     5 = Display “Star Wars”
//     6 = March light sequence
//     7 = Spectrum, bar graph display sequence
//     8 = Display custom text
//
// Std MarcDuino Panel Functions (For custom functions)
//     1 = Panels stay closed (normal position)
//     2 = Scream sequence, all panels open
//     3 = Wave panel sequence
//     4 = Fast (smirk) back and forth panel sequence
//     5 = Wave 2 panel sequence, open progressively all panels, then close one by one)
//     6 = Marching ants panel sequence
//     7 = Faint / short circuit panel sequence
//     8 = Rhythmic cantina panel sequence
//     9 = Custom Panel Sequence

// Marcduino Action Syntax:
// #<1-76> Standard Marcduino Functions
// MP3=<182->,LD=<1-8>,LDText="Hello World",Panel=M<1-8>,Panel<1-10>[delay=1,open=5]

#define MARCDUINO_ACTION(var,act) \
const char* var = act;
//----------------------------------------------------
// CONFIGURE: The FOOT Navigation Controller Buttons
//----------------------------------------------------

MARCDUINO_ACTION(btnUP_MD, "#12")
MARCDUINO_ACTION(btnLeft_MD, "#13")
MARCDUINO_ACTION(btnRight_MD, "#14")
MARCDUINO_ACTION(btnDown_MD, "#11")
MARCDUINO_ACTION(btnUP_CROSS_MD, "#26")
MARCDUINO_ACTION(btnLeft_CROSS_MD, "#23")
MARCDUINO_ACTION(btnRight_CROSS_MD, "#24")
MARCDUINO_ACTION(btnDown_CROSS_MD, "#27")
MARCDUINO_ACTION(btnUP_CIRCLE_MD, "#2")
MARCDUINO_ACTION(btnLeft_CIRCLE_MD, "#4")
MARCDUINO_ACTION(btnRight_CIRCLE_MD, "#7")
MARCDUINO_ACTION(btnDown_CIRCLE_MD, "#10")
MARCDUINO_ACTION(btnUP_PS_MD, "MP3=183,LD=5")
MARCDUINO_ACTION(btnLeft_PS_MD, "MP3=186,LD=1")
MARCDUINO_ACTION(btnRight_PS_MD, "MP3=185,LD=1")
MARCDUINO_ACTION(btnDown_PS_MD, "MP3=184,LD=1")
MARCDUINO_ACTION(btnUP_L1_MD, "#8")
MARCDUINO_ACTION(btnLeft_L1_MD, "#3")
MARCDUINO_ACTION(btnRight_L1_MD, "#5")
MARCDUINO_ACTION(btnDown_L1_MD, "#9")

//----------------------------------------------------
// CONFIGURE: The DOME Navigation Controller Buttons
//----------------------------------------------------
MARCDUINO_ACTION(FTbtnUP_MD, "#58")             // Arrow Up
MARCDUINO_ACTION(FTbtnLeft_MD, "#56")           // Arrow Left
MARCDUINO_ACTION(FTbtnRight_MD, "#57")          // Arrow Right
MARCDUINO_ACTION(FTbtnDown_MD, "#59")           // Arrow Down
MARCDUINO_ACTION(FTbtnUP_CROSS_MD, "#28")       // Arrow UP + CROSS
MARCDUINO_ACTION(FTbtnLeft_CROSS_MD, "#33")     // Arrow Left + CROSS
MARCDUINO_ACTION(FTbtnRight_CROSS_MD, "#30")    // Arrow Right + CROSS
MARCDUINO_ACTION(FTbtnDown_CROSS_MD, "#29")     // Arrow Down + CROSS
MARCDUINO_ACTION(FTbtnUP_CIRCLE_MD, "#22")      // Arrow Up + CIRCLE
MARCDUINO_ACTION(FTbtnLeft_CIRCLE_MD, "#23")    // Arrow Left + CIRCLE
MARCDUINO_ACTION(FTbtnRight_CIRCLE_MD, "#24")   // Arrow Right + CIRCLE
MARCDUINO_ACTION(FTbtnDown_CIRCLE_MD, "#25")    // Arrow Down + CIRCLE
MARCDUINO_ACTION(FTbtnUP_PS_MD, "#38")          // Arrow UP + PS
MARCDUINO_ACTION(FTbtnLeft_PS_MD, "#40")        // Arrow Left + PS
MARCDUINO_ACTION(FTbtnRight_PS_MD, "#41")       // Arrow Right + PS
MARCDUINO_ACTION(FTbtnDown_PS_MD, "#39")        // Arrow Down + PS
MARCDUINO_ACTION(FTbtnUP_L1_MD, "#34")          // Arrow UP + L1
MARCDUINO_ACTION(FTbtnLeft_L1_MD, "#36")        // Arrow Left + L1
MARCDUINO_ACTION(FTbtnRight_L1_MD, "#37")       // Arrow Right + L1
MARCDUINO_ACTION(FTbtnDown_L1_MD, "#35")        // Arrow Down + L1

// ---------------------------------------------------------------------------------------
//               SYSTEM VARIABLES - USER CONFIG SECTION COMPLETED
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
//                          Drive Controller Settings
// ---------------------------------------------------------------------------------------

int motorControllerBaudRate = 9600; // Set the baud rate for the Syren motor controller
                                    // for packetized options are: 2400, 9600, 19200 and 38400

int marcDuinoBaudRate = 9600; // Set the baud rate for the Syren motor controller
                                    
#define SYREN_ADDR         129      // Serial Address for Dome Syren
#define SABERTOOTH_ADDR    128      // Serial Address for Foot Sabertooth

#define ENABLE_UHS_DEBUGGING 1

// ---------------------------------------------------------------------------------------
//                          Libraries
// ---------------------------------------------------------------------------------------
#include <ReelTwo.h>
#include <core/SetupEvent.h>
#include <core/StringUtils.h>

#include <PS3BT.h>
#include <usbhub.h>
#ifdef USE_PREFERENCES
#include <Preferences.h>
#define PREFERENCE_PS3_FOOT_MAC             "ps3footmac"
#define PREFERENCE_PS3_DOME_MAC             "ps3domemac"
Preferences preferences;
#endif

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <motor/SabertoothDriver.h>

#include "SoftwareSerial.h"

#include "pin-map.h"

#define CONSOLE_BUFFER_SIZE     300
static unsigned sPos;
static char sBuffer[CONSOLE_BUFFER_SIZE];

// ---------------------------------------------------------------------------------------
//                    Panel Management Variables
// ---------------------------------------------------------------------------------------
static bool sRunningCustRoutine = false;

struct PanelStatus
{
    uint8_t fStatus = 0;
    uint32_t fStartTime = 0;
    uint8_t fStartDelay = 1;
    uint8_t fDuration = 5;
};

PanelStatus sPanelStatus[PANEL_COUNT];

// ---------------------------------------------------------------------------------------
//                          Variables
// ---------------------------------------------------------------------------------------

long previousDomeMillis = millis();
long previousFootMillis = millis();
long previousMarcDuinoMillis = millis();
long previousDomeToggleMillis = millis();
long previousSpeedToggleMillis = millis();
long currentMillis = millis();

int serialLatency = 25;   //This is a delay factor in ms to prevent queueing of the Serial data.
                          //25ms seems approprate for HardwareSerial, values of 50ms or larger are needed for Softare Emulation
                          
int marcDuinoButtonCounter = 0;
int speedToggleButtonCounter = 0;
int domeToggleButtonCounter = 0;

SabertoothDriver STImpl(SABERTOOTH_ADDR, SABERTOOTH_SERIAL);
SabertoothDriver SyRImpl(SYREN_ADDR, SABERTOOTH_SERIAL);

SabertoothDriver *ST=&STImpl;
SabertoothDriver *SyR=&SyRImpl;

///////Setup for USB and Bluetooth Devices////////////////////////////
USB Usb;
BTD Btd(&Usb);
PS3BT PS3NavFootImpl(&Btd);
PS3BT PS3NavDomeImpl(&Btd);
PS3BT* PS3NavFoot=&PS3NavFootImpl;
PS3BT* PS3NavDome=&PS3NavDomeImpl;

//Used for PS3 Fault Detection
uint32_t msgLagTime = 0;
uint32_t lastMsgTime = 0;
uint32_t currentTime = 0;
uint32_t lastLoopTime = 0;
int badPS3Data = 0;
int badPS3DataDome = 0;

bool firstMessage = true;

bool isFootMotorStopped = true;
bool isDomeMotorStopped = true;

bool overSpeedSelected = false;

bool isPS3NavigatonInitialized = false;
bool isSecondaryPS3NavigatonInitialized = false;

bool isStickEnabled = true;

bool WaitingforReconnect = false;
bool WaitingforReconnectDome = false;

bool mainControllerConnected = false;
bool domeControllerConnected = false;

// Dome Automation Variables
bool domeAutomation = false;
int domeTurnDirection = 1;  // 1 = positive turn, -1 negative turn
float domeTargetPosition = 0; // (0 - 359) - degrees in a circle, 0 = home
unsigned long domeStopTurnTime = 0;    // millis() when next turn should stop
unsigned long domeStartTurnTime = 0;  // millis() when next turn should start
int domeStatus = 0;  // 0 = stopped, 1 = prepare to turn, 2 = turning

byte action = 0;
unsigned long DriveMillis = 0;

int footDriveSpeed = 0;

// =======================================================================================

static const char* sMarcCommands[] = {
#include "MarcduinoCommands.h"
};

static const char* sMarcMP3[] = {
#include "MarcduinoMP3.h"
};

bool handleMarcduinoAction(const char* action)
{
    String LD_text = "";
    bool panelTypeSelected = false;
    bool customRoutine = false;
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", action);
    char* cmd = buffer;
    if (*cmd == '#')
    {
        // Std Marcduino Function Call Configured
        uint32_t seq = strtolu(cmd+1, &cmd);
        if (*cmd == '\0')
        {
            if (seq >= 1 && seq <= SizeOfArray(sMarcCommands))
            {
                // If the commands starts with "BM" we direct it to the body marc controller
                const char* marcCommand = sMarcCommands[seq-1];
                if (marcCommand[0] == 'B' && marcCommand[1] == 'M')
                {
                    sendBodyMarcCommand(&marcCommand[2]);
                }
                else
                {
                    // Otherwise we send it to the dome Marcduino
                    sendMarcCommand(marcCommand);
                }
                return true;
            }
            else
            {
                SHADOW_DEBUG("Marcduino sequence range is 1-%d in action command \"%s\"\n", SizeOfArray(sMarcCommands), action)
                return false;
            }
        }
        SHADOW_DEBUG("Excepting number after # in action command \"%s\"\n", action)
        return false;
    }
    for (;;)
    {
        if (startswith(cmd, "MP3="))
        {
            constexpr int lowerRange = 182;
            constexpr int upperRange = lowerRange + SizeOfArray(sMarcMP3);
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= lowerRange && num <= upperRange)
            {
                sendMarcCommand(sMarcMP3[num - lowerRange]);
            }
            else
            {
                SHADOW_DEBUG("MP3 number range is 181 - %d in action command \"%s\"\n", upperRange, action)
                return false;
            }
        }
        else if (startswith(cmd, "Panel=M"))
        {
            static const char* sCommands[] = {
                ":CL00",
                ":SE51",
                ":SE52",
                ":SE53",
                ":SE54",
                ":SE55",
                ":SE56",
                ":SE57"
            };
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num <= SizeOfArray(sCommands))
            {
                if (num > 1)
                {
                    sendMarcCommand(":CL00");  // close all the panels prior to next custom routine
                    delay(50); // give panel close command time to process before starting next panel command 
                }
                sendMarcCommand(sCommands[num-1]);
                panelTypeSelected = true;
            }
            else
            {
                SHADOW_DEBUG("Marc Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sCommands), action)
                return false;
            }
        }
        else if (startswith(cmd, "Panel"))
        {
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num <= SizeOfArray(sPanelStatus))
            {
                PanelStatus panel;
                panel.fStatus = 1;
                if (*cmd == '[')
                {
                    cmd++;
                    do
                    {
                        if (startswith(cmd, "delay="))
                        {
                            uint32_t delayNum = strtolu(cmd, &cmd);
                            if (delayNum < 31)
                            {
                                panel.fStartDelay = delayNum;
                            }
                            else
                            {
                                panel.fStatus = 0;
                            }
                        }
                        else if (startswith(cmd, "dur="))
                        {
                            uint32_t duration = strtolu(cmd, &cmd);
                            if (duration < 31)
                            {
                                panel.fDuration = duration;
                            }
                            else
                            {
                                panel.fStatus = 0;
                            }
                        }
                        else if (*cmd == ',')
                        {
                            cmd++;
                        }
                    }
                    while (*cmd != '\0' && *cmd != ']');
                }
                if (panel.fStatus)
                {
                    panelTypeSelected = true;
                    panel.fStartTime = millis();
                }
            }
            else
            {
                SHADOW_DEBUG("Panel range is 1 - %d in action command \"%s\"\n", SizeOfArray(sPanelStatus), action)
                return false;
            }
        }
        else if (startswith(cmd, "LDText=\""))
        {
            char* startString = ++cmd;
            while (*cmd != '\0' && *cmd != '"')
                cmd++;
            if (*cmd == '"')
                *cmd = '\0';
            LD_text = startString;
        }
        else if (startswith(cmd, "LD="))
        {
            uint32_t num = strtolu(cmd, &cmd);
            if (num >= 1 && num < 8)
            {
                // If a custom panel movement was selected - need to briefly pause before changing light sequence to avoid conflict)
                if (panelTypeSelected)
                {
                    delay(30);
                }
                switch (num)
                {
                    case 1:
                        sendMarcCommand("@0T1");
                        break;              
                    case 2:
                        sendMarcCommand("@0T4");
                        break;              
                    case 3:
                        sendMarcCommand("@0T5");
                        break;
                    case 4:
                        sendMarcCommand("@0T6");
                        break;
                    case 5:
                        sendMarcCommand("@0T10");
                        break;
                    case 6:
                        sendMarcCommand("@0T11");
                        break;
                    case 7:
                        sendMarcCommand("@0T92");
                        break;
                    case 8:
                        sendMarcCommand("@0T100");
                        delay(50);
                        String custString = "@0M";
                        custString += LD_text;
                        sendMarcCommand(custString.c_str());
                        break;
                }
            }
            else
            {
                SHADOW_DEBUG("LD range is 1 - 8 in action command \"%s\"\n", action)
                return false;
            }
        }
        if (*cmd != ',')
            break;
        cmd++;
    }
    if (*cmd != '\0')
    {
        SHADOW_DEBUG("Ignoring unknown trailing \"%s\" in action \"%s\"\n", cmd, action);
    }
    if (customRoutine)
    {
        sRunningCustRoutine = true;
    }
    return true;
}

// =======================================================================================
//                          Main Program
// =======================================================================================


// =======================================================================================
//                          Initialize - Setup Function
// =======================================================================================
void setup()
{
    REELTWO_READY();

#ifdef USE_PREFERENCES
    if (!preferences.begin("penumbrashadow", false))
    {
        DEBUG_PRINTLN("Failed to init prefs");
    }
    else
    {
        PS3ControllerFootMac = preferences.getString(PREFERENCE_PS3_FOOT_MAC, PS3_CONTROLLER_FOOT_MAC);
        PS3ControllerDomeMAC = preferences.getString(PREFERENCE_PS3_DOME_MAC, PS3_CONTROLLER_DOME_MAC);
    }
#endif
    PrintReelTwoInfo(Serial, "Penumbra Shadow MD");

    Serial.print(F("\r\nBluetooth Library Started"));

    //Setup for PS3
    PS3NavFoot->attachOnInit(onInitPS3NavFoot); // onInitPS3NavFoot is called upon a new connection
    PS3NavDome->attachOnInit(onInitPS3NavDome); 

    //Setup for SABERTOOTH_SERIAL Motor Controllers - Sabertooth (Feet) 
    SABERTOOTH_SERIAL_INIT(motorControllerBaudRate);
    ST->autobaud();          // Send the autobaud command to the Sabertooth controller(s).
    ST->setTimeout(10);      //DMB:  How low can we go for safety reasons?  multiples of 100ms
    ST->setDeadband(driveDeadBandRange);
    ST->stop(); 
    SyR->autobaud();
    SyR->setTimeout(20);      //DMB:  How low can we go for safety reasons?  multiples of 100ms
    SyR->stop(); 

    // //Setup for MD_SERIAL MarcDuino Dome Control Board
    MD_SERIAL_INIT(marcDuinoBaudRate);

    //Setup for BODY_MD_SERIAL Optional MarcDuino Control Board for Body Panels
    BODY_MD_SERIAL_INIT(marcDuinoBaudRate);

    // randomSeed(analogRead(0));  // random number seed for dome automation   

    SetupEvent::ready();

    if (Usb.Init() == -1)
    {
        Serial.print(F("\r\nOSC did not start"));
        while (1); //halt
    }
}

void sendMarcCommand(const char* cmd)
{
    SHADOW_VERBOSE("Sending MARC: \"%s\"\n", cmd)
    MD_SERIAL.print(cmd); MD_SERIAL.print("\n");
}

void sendBodyMarcCommand(const char* cmd)
{
    SHADOW_VERBOSE("Sending BODYMARC: \"%s\"\n", cmd)
    BODY_MD_SERIAL.print(cmd); BODY_MD_SERIAL.print("\n");
}

////////////////////////////////
// This function is called when settings have been changed and needs a reboot
void reboot()
{
    DEBUG_PRINTLN("Restarting...");
    preferences.end();
    delay(1000);
    ESP.restart();
}

// =======================================================================================
//           Main Program Loop - This is the recurring check loop for entire sketch
// =======================================================================================

void loop()
{   
    //LOOP through functions from highest to lowest priority.
    if (!readUSB())
        return;
    
    footMotorDrive();
    domeDrive();
    marcDuinoDome();
    marcDuinoFoot();
    toggleSettings();
    custMarcDuinoPanel();     
    
    // If dome automation is enabled - Call function
    if (domeAutomation && time360DomeTurn > 1999 && time360DomeTurn < 8001 && domeAutoSpeed > 49 && domeAutoSpeed < 101)  
    {
       autoDome(); 
    }   

    if (Serial.available())
    {
        int ch = Serial.read();
        if (ch == 0x0A || ch == 0x0D)
        {
            const char* cmd = sBuffer;
            if (startswith(cmd, "#SMZERO"))
            {
                preferences.clear();
                DEBUG_PRINT("Clearing preferences. ");
                reboot();
            }
            else if (startswith(cmd, "#SMRESTART"))
            {
                reboot();
            }
            else
            {
                printf("Unknown\n");
            }
            sPos = 0;
        }
        else if (sPos < SizeOfArray(sBuffer)-1)
        {
            sBuffer[sPos++] = ch;
            sBuffer[sPos] = '\0';
        }
    }
}

// =======================================================================================
//           footDrive Motor Control Section
// =======================================================================================

bool ps3FootMotorDrive(PS3BT* myPS3 = PS3NavFoot)
{
    int stickSpeed = 0;
    int turnnum = 0;
  
    if (isPS3NavigatonInitialized)
    {    
         // Additional fault control.  Do NOT send additional commands to Sabertooth if no controllers have initialized.
        if (!isStickEnabled)
        {
        #ifdef SHADOW_VERBOSE
            if (abs(myPS3->getAnalogHat(LeftHatY)-128) > joystickFootDeadZoneRange)
            {
                SHADOW_VERBOSE("Drive Stick is disabled\n")
            }
        #endif

            if (!isFootMotorStopped)
            {
                ST->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;
              
                SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
            }
            return false;
        }
        else if (!myPS3->PS3NavigationConnected)
        {
            if (!isFootMotorStopped)
            {
                ST->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;

                SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
            }
          
            return false;
        }
        else if (myPS3->getButtonPress(L2) || myPS3->getButtonPress(L1))
        {
            if (!isFootMotorStopped)
            {
                ST->stop();
                isFootMotorStopped = true;
                footDriveSpeed = 0;

                SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")              
            }
            return false;
        }
        else
        {
            int joystickPosition = myPS3->getAnalogHat(LeftHatY);
          
            if (overSpeedSelected) //Over throttle is selected
            {
                stickSpeed = (map(joystickPosition, 0, 255, -drivespeed2, drivespeed2));   
            }
            else 
            {
                stickSpeed = (map(joystickPosition, 0, 255, -drivespeed1, drivespeed1));
            }

            if ( abs(joystickPosition-128) < joystickFootDeadZoneRange)
            {
                // This is RAMP DOWN code when stick is now at ZERO but prior FootSpeed > 20
                if (abs(footDriveSpeed) > 50)
                {   
                    if (footDriveSpeed > 0)
                    {
                        footDriveSpeed -= 3;
                    }
                    else
                    {
                        footDriveSpeed += 3;
                    }
                    SHADOW_VERBOSE("ZERO FAST RAMP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                }
                else if (abs(footDriveSpeed) > 20)
                {   
                    if (footDriveSpeed > 0)
                    {
                        footDriveSpeed -= 2;
                    }
                    else
                    {
                        footDriveSpeed += 2;
                    }  
                    SHADOW_VERBOSE("ZERO MID RAMP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                }
                else
                {        
                    footDriveSpeed = 0;
                }
            }
            else 
            {
                isFootMotorStopped = false;
                if (footDriveSpeed < stickSpeed)
                {
                    if ((stickSpeed-footDriveSpeed)>(ramping+1))
                    {
                        footDriveSpeed+=ramping;
                        SHADOW_VERBOSE("RAMPING UP: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                    }
                    else
                    {
                        footDriveSpeed = stickSpeed;
                    }
                }
                else if (footDriveSpeed > stickSpeed)
                {
                    if ((footDriveSpeed-stickSpeed)>(ramping+1))
                    {
                        footDriveSpeed-=ramping;
                        SHADOW_VERBOSE("RAMPING DOWN: footSpeed: %d\nStick Speed: %d\n", footDriveSpeed, stickSpeed)
                    }
                    else
                    {
                        footDriveSpeed = stickSpeed;  
                    }
                }
                else
                {
                    footDriveSpeed = stickSpeed;  
                }
            }
            turnnum = (myPS3->getAnalogHat(LeftHatX));

            //TODO:  Is there a better algorithm here?  
            if ( abs(footDriveSpeed) > 50)
                turnnum = (map(myPS3->getAnalogHat(LeftHatX), 54, 200, -(turnspeed/4), (turnspeed/4)));
            else if (turnnum <= 200 && turnnum >= 54)
                turnnum = (map(myPS3->getAnalogHat(LeftHatX), 54, 200, -(turnspeed/3), (turnspeed/3)));
            else if (turnnum > 200)
                turnnum = (map(myPS3->getAnalogHat(LeftHatX), 201, 255, turnspeed/3, turnspeed));
            else if (turnnum < 54)
                turnnum = (map(myPS3->getAnalogHat(LeftHatX), 0, 53, -turnspeed, -(turnspeed/3)));
              
            if (abs(turnnum) > 5)
            {
                isFootMotorStopped = false;   
            }

            currentMillis = millis();
          
            if ((currentMillis - previousFootMillis) > serialLatency)
            {
                if (footDriveSpeed != 0 || abs(turnnum) > 5)
                {
                    SHADOW_VERBOSE("Motor: FootSpeed: %d\nTurnnum: %d\nTime of command: %lu\n", footDriveSpeed, turnnum, millis())              
                    ST->turn(turnnum * invertTurnDirection);
                    ST->drive(footDriveSpeed);
                }
                else
                {    
                    if (!isFootMotorStopped)
                    {
                        ST->stop();
                        isFootMotorStopped = true;
                        footDriveSpeed = 0;
                      
                        SHADOW_VERBOSE("\n***Foot Motor STOPPED***\n")
                    }
                }
              
                // The Sabertooth won't act on mixed mode packet serial commands until
                // it has received power levels for BOTH throttle and turning, since it
                // mixes the two together to get diff-drive power levels for both motors.
              
                previousFootMillis = currentMillis;
                return true; //we sent a foot command   
            }
        }
    }
    return false;
}

void footMotorDrive()
{
    //Flood control prevention
    if ((millis() - previousFootMillis) < serialLatency)
        return;
  
    if (PS3NavFoot->PS3NavigationConnected)
        ps3FootMotorDrive(PS3NavFoot);
}  


// =======================================================================================
//           domeDrive Motor Control Section
// =======================================================================================

int ps3DomeDrive(PS3BT* myPS3 = PS3NavDome)
{
    int domeRotationSpeed = 0;
    int joystickPosition = myPS3->getAnalogHat(LeftHatX);
        
    domeRotationSpeed = (map(joystickPosition, 0, 255, -domespeed, domespeed));
    if ( abs(joystickPosition-128) < joystickDomeDeadZoneRange ) 
       domeRotationSpeed = 0;
          
    if (domeRotationSpeed != 0 && domeAutomation == true)  // Turn off dome automation if manually moved
    {   
        domeAutomation = false; 
        domeStatus = 0;
        domeTargetPosition = 0; 
        
        SHADOW_VERBOSE("Dome Automation OFF\n")
    }    
    return domeRotationSpeed;
}

void rotateDome(int domeRotationSpeed, String mesg)
{
    //Constantly sending commands to the SyRen (Dome) is causing foot motor delay.
    //Lets reduce that chatter by trying 3 things:
    // 1.) Eliminate a constant stream of "don't spin" messages (isDomeMotorStopped flag)
    // 2.) Add a delay between commands sent to the SyRen (previousDomeMillis timer)
    // 3.) Switch to real UART on the MEGA (Likely the *CORE* issue and solution)
    // 4.) Reduce the timout of the SyRen - just better for safety!
    
    currentMillis = millis();
    if ((!isDomeMotorStopped || domeRotationSpeed != 0) &&
        ((currentMillis - previousDomeMillis) > (2*serialLatency)))
    {
        if (domeRotationSpeed != 0)
        {
            isDomeMotorStopped = false;
            SHADOW_VERBOSE("Dome rotation speed: %d\n", domeRotationSpeed)        
            SyR->motor(domeRotationSpeed);
        }
        else
        {
            isDomeMotorStopped = true; 
            SHADOW_VERBOSE("\n***Dome motor is STOPPED***\n")
            
            SyR->stop();
        }
        previousDomeMillis = currentMillis;      
    }
}

void domeDrive()
{
    //Flood control prevention
    //This is intentionally set to double the rate of the Dome Motor Latency
    if ((millis() - previousDomeMillis) < (2*serialLatency))
        return;
  
    int domeRotationSpeed = 0;
    int ps3NavControlSpeed = 0;
    if (PS3NavDome->PS3NavigationConnected) 
    {
        ps3NavControlSpeed = ps3DomeDrive(PS3NavDome);
        domeRotationSpeed = ps3NavControlSpeed; 

        rotateDome(domeRotationSpeed,"Controller Move");
    }
    else if (PS3NavFoot->PS3NavigationConnected && PS3NavFoot->getButtonPress(L2))
    {
        ps3NavControlSpeed = ps3DomeDrive(PS3NavFoot);
        domeRotationSpeed = ps3NavControlSpeed;

        rotateDome(domeRotationSpeed,"Controller Move");    
    }
    else if (!isDomeMotorStopped)
    {
        SyR->stop();
        isDomeMotorStopped = true;
    }  
}  

// =======================================================================================
//                               Toggle Control Section
// =======================================================================================

void ps3ToggleSettings(PS3BT* myPS3 = PS3NavFoot)
{
    // enable / disable drive stick
    if (myPS3->getButtonPress(PS) && myPS3->getButtonClick(CROSS))
    {
        SHADOW_DEBUG("Disabling the DriveStick\n")
        SHADOW_DEBUG("Stopping Motors\n")

        ST->stop();
        isFootMotorStopped = true;
        isStickEnabled = false;
        footDriveSpeed = 0;
    }
    
    if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(CIRCLE))
    {
        SHADOW_DEBUG("Enabling the DriveStick\n");
        isStickEnabled = true;
    }
    
    // Enable and Disable Overspeed
    if (myPS3->getButtonPress(L3) && myPS3->getButtonPress(L1) && isStickEnabled)
    {
        if ((millis() - previousSpeedToggleMillis) > 1000)
        {
            speedToggleButtonCounter = 0;
            previousSpeedToggleMillis = millis();
        } 

        speedToggleButtonCounter += 1;       
        if (speedToggleButtonCounter == 1)
        {
            if (!overSpeedSelected)
            {
                overSpeedSelected = true;
                SHADOW_VERBOSE("Over Speed is now: ON\n");
            }
            else
            {      
                overSpeedSelected = false;
                SHADOW_VERBOSE("Over Speed is now: OFF\n")
            }
        }
    }
   
    // Enable Disable Dome Automation
    if(myPS3->getButtonPress(L2) && myPS3->getButtonClick(CROSS))
    {
        domeAutomation = false;
        domeStatus = 0;
        domeTargetPosition = 0;
        SyR->stop();
        isDomeMotorStopped = true;
        
        SHADOW_DEBUG("Dome Automation OFF\n")
    } 

    if(myPS3->getButtonPress(L2) && myPS3->getButtonClick(CIRCLE))
    {
        domeAutomation = true;

        SHADOW_DEBUG("Dome Automation On\n")
    } 
}

void toggleSettings()
{
    if (PS3NavFoot->PS3NavigationConnected)
        ps3ToggleSettings(PS3NavFoot);
}  

// ====================================================================================================================
// This function determines if MarcDuino buttons were selected and calls main processing function for FOOT controller
// ====================================================================================================================
void marcDuinoFoot()
{
    if (PS3NavFoot->PS3NavigationConnected && (PS3NavFoot->getButtonPress(UP) || PS3NavFoot->getButtonPress(DOWN) || PS3NavFoot->getButtonPress(LEFT) || PS3NavFoot->getButtonPress(RIGHT)))
    {
        if ((millis() - previousMarcDuinoMillis) > 1000)
        {
            marcDuinoButtonCounter = 0;
            previousMarcDuinoMillis = millis();
        }
       marcDuinoButtonCounter += 1;
    }
    else
    {
        return;    
    }
   
    // Clear inbound buffer of any data sent from the MarcDuino board
    while (MD_SERIAL.available())
        MD_SERIAL.read();

    //------------------------------------ 
    // Send triggers for the base buttons 
    //------------------------------------
    if (PS3NavFoot->getButtonPress(UP) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(L1) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        if (PS3NavDome->PS3NavigationConnected && (PS3NavDome->getButtonPress(CROSS) || PS3NavDome->getButtonPress(CIRCLE) || PS3NavDome->getButtonPress(PS)))
        {
            // Skip this section
        }
        else
        {
            handleMarcduinoAction(btnUP_MD);
            SHADOW_VERBOSE("FOOT: btnUP\n")
            return;
        }
    }
    
    if (PS3NavFoot->getButtonPress(DOWN) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(L1) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        if (PS3NavDome->PS3NavigationConnected && (PS3NavDome->getButtonPress(CROSS) || PS3NavDome->getButtonPress(CIRCLE) || PS3NavDome->getButtonPress(PS)))
        {
            // Skip this section
        }
        else
        {     
            handleMarcduinoAction(btnDown_MD);
            SHADOW_VERBOSE("FOOT: btnDown\n");
            return;
        }
    }
    
    if (PS3NavFoot->getButtonPress(LEFT) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(L1) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        if (PS3NavDome->PS3NavigationConnected && (PS3NavDome->getButtonPress(CROSS) || PS3NavDome->getButtonPress(CIRCLE) || PS3NavDome->getButtonPress(PS)))
        {
            // Skip this section
        }
        else
        {           
            handleMarcduinoAction(btnLeft_MD);
            SHADOW_VERBOSE("FOOT: btnLeft\n")
            return;
        }
    }

    if (PS3NavFoot->getButtonPress(RIGHT) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(L1) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        if (PS3NavDome->PS3NavigationConnected && (PS3NavDome->getButtonPress(CROSS) || PS3NavDome->getButtonPress(CIRCLE) || PS3NavDome->getButtonPress(PS)))
        {
            // Skip this section
        }
        else
        {     
            handleMarcduinoAction(btnRight_MD);
            SHADOW_VERBOSE("FOOT: btnRight\n")
            return;
        }
    }
    
    //------------------------------------ 
    // Send triggers for the CROSS + base buttons 
    //------------------------------------
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavFoot->getButtonPress(CROSS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavDome->getButtonPress(CROSS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnUP_CROSS_MD);
        SHADOW_VERBOSE("FOOT: btnUP_CROSS\n")
        return;        
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(CROSS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavDome->getButtonPress(CROSS))) && marcDuinoButtonCounter == 1)
    {      
        handleMarcduinoAction(btnDown_CROSS_MD);
        SHADOW_VERBOSE("FOOT: btnDown_CROSS\n")
        return;
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(CROSS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavDome->getButtonPress(CROSS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnLeft_CROSS_MD);
        SHADOW_VERBOSE("FOOT: btnLeft_CROSS\n")
        return;
    }

    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(CROSS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavDome->getButtonPress(CROSS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnRight_CROSS_MD);
        SHADOW_VERBOSE("FOOT: btnRight_CROSS\n")
        return;
    }

    //------------------------------------ 
    // Send triggers for the CIRCLE + base buttons 
    //------------------------------------
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavFoot->getButtonPress(CIRCLE)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavDome->getButtonPress(CIRCLE))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnUP_CIRCLE_MD);
        SHADOW_VERBOSE("FOOT: btnUP_CIRCLE\n")
        return;
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(CIRCLE)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavDome->getButtonPress(CIRCLE))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnDown_CIRCLE_MD);
        SHADOW_VERBOSE("FOOT: btnDown_CIRCLE\n")
        return;
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(CIRCLE)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavDome->getButtonPress(CIRCLE))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnLeft_CIRCLE_MD);
        SHADOW_VERBOSE("FOOT: btnLeft_CIRCLE\n")
        return;
    }

    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(CIRCLE)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavDome->getButtonPress(CIRCLE))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnRight_CIRCLE_MD);
        SHADOW_VERBOSE("FOOT: btnRight_CIRCLE\n")
        return;
    }
    
    //------------------------------------ 
    // Send triggers for the L1 + base buttons 
    //------------------------------------
    if (PS3NavFoot->getButtonPress(UP) && PS3NavFoot->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnUP_L1_MD);
        SHADOW_VERBOSE("FOOT: btnUP_L1\n")
        return;
    }
    
    if (PS3NavFoot->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnDown_L1_MD);
        SHADOW_VERBOSE("FOOT: btnDown_L1\n")
        return;
    }
    
    if (PS3NavFoot->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnLeft_L1_MD);
        SHADOW_VERBOSE("FOOT: btnLeft_L1\n")
        return;
    }

    if (PS3NavFoot->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnRight_L1_MD);
        SHADOW_VERBOSE("FOOT: btnRight_L1\n")
        return;
    }
    
    //------------------------------------ 
    // Send triggers for the PS + base buttons 
    //------------------------------------
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavFoot->getButtonPress(PS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(UP) && PS3NavDome->getButtonPress(PS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnUP_PS_MD);
        SHADOW_VERBOSE("FOOT: btnUP_PS\n")
        return;
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(PS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(DOWN) && PS3NavDome->getButtonPress(PS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnDown_PS_MD);
        SHADOW_VERBOSE("FOOT: btnDown_PS\n")
        return;
    }
    
    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(PS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(LEFT) && PS3NavDome->getButtonPress(PS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnLeft_PS_MD);
        SHADOW_VERBOSE("FOOT: btnLeft_PS\n")
        return;
    }

    if (((!PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(PS)) || (PS3NavDome->PS3NavigationConnected && PS3NavFoot->getButtonPress(RIGHT) && PS3NavDome->getButtonPress(PS))) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(btnRight_PS_MD);
        SHADOW_VERBOSE("FOOT: btnRight_PS\n")
        return;
    }
}

// ===================================================================================================================
// This function determines if MarcDuino buttons were selected and calls main processing function for DOME Controller
// ===================================================================================================================
void marcDuinoDome()
{
    if (PS3NavDome->PS3NavigationConnected && (PS3NavDome->getButtonPress(UP) || PS3NavDome->getButtonPress(DOWN) || PS3NavDome->getButtonPress(LEFT) || PS3NavDome->getButtonPress(RIGHT)))
    {
        if ((millis() - previousMarcDuinoMillis) > 1000)
        {
            marcDuinoButtonCounter = 0;
            previousMarcDuinoMillis = millis();
        }
        marcDuinoButtonCounter += 1;
    }
    else
    {
        return;    
    }
   
    // Clear inbound buffer of any data sent form the MarcDuino board
    while (MD_SERIAL.available())
        MD_SERIAL.read();

    //------------------------------------ 
    // Send triggers for the base buttons 
    //------------------------------------
    if (PS3NavDome->getButtonPress(UP) && !PS3NavDome->getButtonPress(CROSS) && !PS3NavDome->getButtonPress(CIRCLE) && !PS3NavDome->getButtonPress(L1) && !PS3NavDome->getButtonPress(PS) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnUP_MD);
        SHADOW_VERBOSE("DOME: btnUP\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(DOWN) && !PS3NavDome->getButtonPress(CROSS) && !PS3NavDome->getButtonPress(CIRCLE) && !PS3NavDome->getButtonPress(L1) && !PS3NavDome->getButtonPress(PS) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnDown_MD);
        SHADOW_VERBOSE("DOME: btnDown\n")
        return;      
    }
    
    if (PS3NavDome->getButtonPress(LEFT) && !PS3NavDome->getButtonPress(CROSS) && !PS3NavDome->getButtonPress(CIRCLE) && !PS3NavDome->getButtonPress(L1) && !PS3NavDome->getButtonPress(PS) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnLeft_MD);
        SHADOW_VERBOSE("DOME: btnLeft\n")
        return;
    }

    if (PS3NavDome->getButtonPress(RIGHT) && !PS3NavDome->getButtonPress(CROSS) && !PS3NavDome->getButtonPress(CIRCLE) && !PS3NavDome->getButtonPress(L1) && !PS3NavDome->getButtonPress(PS) && !PS3NavFoot->getButtonPress(CROSS) && !PS3NavFoot->getButtonPress(CIRCLE) && !PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnRight_MD);
        SHADOW_VERBOSE("DOME: btnRight\n")
        return;
    }
    
    //------------------------------------ 
    // Send triggers for the CROSS + base buttons 
    //------------------------------------
    if (PS3NavDome->getButtonPress(UP) && PS3NavFoot->getButtonPress(CROSS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnUP_CROSS_MD);
        SHADOW_VERBOSE("DOME: btnUP_CROSS\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(CROSS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnDown_CROSS_MD);
        SHADOW_VERBOSE("DOME: btnDown_CROSS\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(CROSS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnLeft_CROSS_MD);
        SHADOW_VERBOSE("DOME: btnLeft_CROSS\n")
        return;
    }

    if (PS3NavDome->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(CROSS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnRight_CROSS_MD);
        SHADOW_VERBOSE("DOME: btnRight_CROSS\n")
        return;
    }

    //------------------------------------ 
    // Send triggers for the CIRCLE + base buttons 
    //------------------------------------
    if (PS3NavDome->getButtonPress(UP) && PS3NavFoot->getButtonPress(CIRCLE) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnUP_CIRCLE_MD);
        SHADOW_VERBOSE("DOME: btnUP_CIRCLE\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(CIRCLE) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnDown_CIRCLE_MD);
        SHADOW_VERBOSE("DOME: btnDown_CIRCLE\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(CIRCLE) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnLeft_CIRCLE_MD);
        SHADOW_VERBOSE("DOME: btnLeft_CIRCLE\n")
        return;
    }

    if (PS3NavDome->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(CIRCLE) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnRight_CIRCLE_MD);
        SHADOW_VERBOSE("DOME: btnRight_CIRCLE\n")
        return;
    }
    
    //------------------------------------ 
    // Send triggers for the L1 + base buttons 
    //------------------------------------
    if (PS3NavDome->getButtonPress(UP) && PS3NavDome->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnUP_L1_MD);
        SHADOW_VERBOSE("DOME: btnUP_L1\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(DOWN) && PS3NavDome->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnDown_L1_MD);
        SHADOW_VERBOSE("DOME: btnDown_L1\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(LEFT) && PS3NavDome->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnLeft_L1_MD);
        SHADOW_VERBOSE("DOME: btnLeft_L1\n")
        return;
    }

    if (PS3NavDome->getButtonPress(RIGHT) && PS3NavDome->getButtonPress(L1) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnRight_L1_MD);
        SHADOW_VERBOSE("DOME: btnRight_L1\n")
        return;
    }
    
    //------------------------------------ 
    // Send triggers for the PS + base buttons 
    //------------------------------------
    if (PS3NavDome->getButtonPress(UP) && PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnUP_PS_MD);
        SHADOW_VERBOSE("DOME: btnUP_PS\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(DOWN) && PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnDown_PS_MD);
        SHADOW_VERBOSE("DOME: btnDown_PS\n")
        return;
    }
    
    if (PS3NavDome->getButtonPress(LEFT) && PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnLeft_PS_MD);
        SHADOW_VERBOSE("DOME: btnLeft_PS\n")
        return;
    }

    if (PS3NavDome->getButtonPress(RIGHT) && PS3NavFoot->getButtonPress(PS) && marcDuinoButtonCounter == 1)
    {
        handleMarcduinoAction(FTbtnRight_PS_MD);
        SHADOW_VERBOSE("DOME: btnRight_PS\n")
        return;
    }
}


// =======================================================================================
// This function handles the processing of custom MarcDuino panel routines
// =======================================================================================
void custMarcDuinoPanel()
{
    if (!sRunningCustRoutine)
        return;
    sRunningCustRoutine = false;
    for (int i = 0; i < SizeOfArray(sPanelStatus); i++)
    {
        PanelStatus &panel = sPanelStatus[i];
        if (panel.fStatus == 1)
        {
            if (panel.fStartTime + panel.fStartDelay * 1000 < millis())
            {
                char cmd[10];
                snprintf(cmd, sizeof(cmd), ":OP%02d", i+1);
                sendMarcCommand(cmd);
                panel.fStatus = 2;
            }
        }
        else if (panel.fStatus == 2)
        {
            if (panel.fStartTime + (panel.fStartDelay + panel.fDuration) * 1000 < millis())
            {
                char cmd[10];
                snprintf(cmd, sizeof(cmd), ":CL%02d", i+1);
                sendMarcCommand(cmd);
                panel.fStatus = 0;
            }
        }
        if (panel.fStatus != 0)
            sRunningCustRoutine = true;
    }
}

// =======================================================================================
//                             Dome Automation Function
//
//    Features toggles 'on' via L2 + CIRCLE.  'off' via L2 + CROSS.  Default is 'off'.
//
//    This routines randomly turns the dome motor in both directions.  It assumes the 
//    dome is in the 'home' position when the auto dome feature is toggled on.  From
//    there it turns the dome in a random direction.  Stops for a random length of 
//    of time.  Then returns the dome to the home position.  This randomly repeats.
//
//    It is driven off the user variable - time360DomeTurn.  This records how long
//    it takes the dome to do a 360 degree turn at the given auto dome speed.  Tweaking
//    this parameter to be close provides the best results.
//
//    Activating the dome controller manually immediately cancels the auto dome feature
//    or you can toggle the feature off by pressing L2 + CROSS.
// =======================================================================================
void autoDome()
{
    long rndNum;
    int domeSpeed;
    
    if (domeStatus == 0)  // Dome is currently stopped - prepare for a future turn
    { 
        if (domeTargetPosition == 0)  // Dome is currently in the home position - prepare to turn away
        {
            domeStartTurnTime = millis() + (random(3, 10) * 1000);

            rndNum = random(5,354);

            domeTargetPosition = rndNum;  // set the target position to a random degree of a 360 circle - shaving off the first and last 5 degrees

            if (domeTargetPosition < 180)  // Turn the dome in the positive direction
            {
                domeTurnDirection = 1;
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);              
            }
            else  // Turn the dome in the negative direction
            {
                domeTurnDirection = -1;
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);              
            }
        }
        else  // Dome is not in the home position - send it back to home
        {
            domeStartTurnTime = millis() + (random(3, 10) * 1000);   
            if (domeTargetPosition < 180)
            {
                domeTurnDirection = -1;
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);
            }
            else
            {
                domeTurnDirection = 1;
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);
            }
            domeTargetPosition = 0;
        }
        domeStatus = 1;  // Set dome status to preparing for a future turn

        SHADOW_DEBUG("Dome Automation: Initial Turn Set\nCurrent Time: %lu\nNext Start Time: %lu\nNext Stop Time: %lu\nDome Target Position: %f\n",
            millis(), domeStartTurnTime, domeStopTurnTime, domeTargetPosition);
    }    

    if (domeStatus == 1)  // Dome is prepared for a future move - start the turn when ready
    {
        if (domeStartTurnTime < millis())
        {
            domeStatus = 2; 
            SHADOW_DEBUG("Dome Automation: Ready To Start Turn\n")
        }
    }
    
    if (domeStatus == 2) // Dome is now actively turning until it reaches its stop time
    {      
        if (domeStopTurnTime > millis())
        {
            domeSpeed = domeAutoSpeed * domeTurnDirection;
            SyR->motor(domeSpeed);

            SHADOW_DEBUG("Turning Now!!\n")
        }
        else  // turn completed - stop the motor
        {
            domeStatus = 0;
            SyR->stop();

            SHADOW_DEBUG("STOP TURN!!\n")
        }      
    }
}

// =======================================================================================
//           PS3 Controller Device Mgt Functions
// =======================================================================================

void onInitPS3NavFoot()
{
    String btAddress = getLastConnectedBtMAC();
    PS3NavFoot->setLedOn(LED1);
    isPS3NavigatonInitialized = true;
    badPS3Data = 0;

    SHADOW_DEBUG("\nBT Address of Last connected Device when FOOT PS3 Connected: %s\n", btAddress.c_str());
    
    if (btAddress == PS3ControllerFootMac || btAddress == PS3ControllerBackupFootMac)
    {
        SHADOW_DEBUG("\nWe have our FOOT controller connected.\n")
          
        mainControllerConnected = true;
        WaitingforReconnect = true;
    }
#ifdef USE_PREFERENCES
    else if (PS3ControllerFootMac[0] == 'X')
    {
        SHADOW_DEBUG("\nAssigning %s as FOOT controller.\n", btAddress.c_str());
          
        preferences.putString(PREFERENCE_PS3_FOOT_MAC, btAddress);
        PS3ControllerFootMac = btAddress;
        mainControllerConnected = true;
        WaitingforReconnect = true;
    }
#endif
    else
    {
        // Prevent connection from anything but the MAIN controllers          
        SHADOW_DEBUG("\nWe have an invalid controller trying to connect as tha FOOT controller, it will be dropped.\n")

        ST->stop();
        SyR->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        PS3NavFoot->setLedOff(LED1);
        PS3NavFoot->disconnect();
    
        isPS3NavigatonInitialized = false;
        mainControllerConnected = false;        
    } 
}

void onInitPS3NavDome()
{
    String btAddress = getLastConnectedBtMAC();
    PS3NavDome->setLedOn(LED1);
    isSecondaryPS3NavigatonInitialized = true;
    badPS3Data = 0;
    
    if (btAddress == PS3ControllerDomeMAC || btAddress == PS3ControllerBackupDomeMAC)
    {
        SHADOW_DEBUG("\nWe have our DOME controller connected.\n")
          
        domeControllerConnected = true;
        WaitingforReconnectDome = true;
    }
#ifdef USE_PREFERENCES
    else if (PS3ControllerDomeMAC[0] == 'X')
    {
        SHADOW_DEBUG("\nAssigning %s as DOME controller.\n", btAddress.c_str());
          
        preferences.putString(PREFERENCE_PS3_DOME_MAC, btAddress);
        PS3ControllerDomeMAC = btAddress;

        domeControllerConnected = true;
        WaitingforReconnectDome = true;
    }
#endif
    else
    {
        // Prevent connection from anything but the DOME controllers          
        SHADOW_DEBUG("\nWe have an invalid controller trying to connect as the DOME controller, it will be dropped.\n")

        ST->stop();
        SyR->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        PS3NavDome->setLedOff(LED1);
        PS3NavDome->disconnect();
    
        isSecondaryPS3NavigatonInitialized = false;
        domeControllerConnected = false;        
    } 
}

String getLastConnectedBtMAC()
{
    char buffer[20];
    uint8_t* addr = Btd.disc_bdaddr;
    snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    return buffer;
}

bool criticalFaultDetect()
{
    if (PS3NavFoot->PS3NavigationConnected || PS3NavFoot->PS3Connected)
    {
        currentTime = millis();
        lastMsgTime = PS3NavFoot->getLastMessageTime();
        msgLagTime = currentTime - lastMsgTime;            
        
        if (WaitingforReconnect)
        {
            if (msgLagTime < 200)
            {             
                WaitingforReconnect = false; 
            }
            lastMsgTime = currentTime;            
        } 

        if (currentTime >= lastMsgTime)
        {
            msgLagTime = currentTime - lastMsgTime;
        }
        else
        {
            msgLagTime = 0;
        }
        
        if (msgLagTime > 300 && !isFootMotorStopped)
        {
            SHADOW_DEBUG("It has been 300ms since we heard from the PS3 Foot Controller\n")
            SHADOW_DEBUG("Shutting down motors, and watching for a new PS3 Foot message\n")
            ST->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
        }
        
        if ( msgLagTime > 10000 )
        {
            SHADOW_DEBUG("It has been 10s since we heard from the PS3 Foot Controller\nmsgLagTime:%u  lastMsgTime:%u  millis: %lu\n",
                          msgLagTime, lastMsgTime, millis())
            SHADOW_DEBUG("Disconnecting the Foot controller\n")
            ST->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
            PS3NavFoot->disconnect();
            WaitingforReconnect = true;
            return true;
        }

        //Check PS3 Signal Data
        if(!PS3NavFoot->getStatus(Plugged) && !PS3NavFoot->getStatus(Unplugged))
        {
            //We don't have good data from the controller.
            //Wait 15ms if no second controller - 100ms if some controller connected, Update USB, and try again
            if (PS3NavDome->PS3NavigationConnected)
            {
                  delay(100);     
            } else
            {
                  delay(15);
            }
            
            // Usb.Task();   
            lastMsgTime = PS3NavFoot->getLastMessageTime();
            
            if(!PS3NavFoot->getStatus(Plugged) && !PS3NavFoot->getStatus(Unplugged))
            {
                badPS3Data++;
                SHADOW_DEBUG("\n**Invalid data from PS3 FOOT Controller. - Resetting Data**\n")
                return true;
            }
        }
        else if (badPS3Data > 0)
        {

            badPS3Data = 0;
        }
        
        if ( badPS3Data > 10 )
        {
            SHADOW_DEBUG("Too much bad data coming from the PS3 FOOT Controller\n")
            SHADOW_DEBUG("Disconnecting the controller and stop motors.\n")

            ST->stop();
            isFootMotorStopped = true;
            footDriveSpeed = 0;
            PS3NavFoot->disconnect();
            WaitingforReconnect = true;
            return true;
        }
    }
    else if (!isFootMotorStopped)
    {
        SHADOW_DEBUG("No foot controller was found\n")
        SHADOW_DEBUG("Shuting down motors and watching for a new PS3 foot message\n")

        ST->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        WaitingforReconnect = true;
        return true;
    }
    
    return false;
}

bool criticalFaultDetectDome()
{
    if (PS3NavDome->PS3NavigationConnected || PS3NavDome->PS3Connected)
    {
        currentTime = millis();
        lastMsgTime = PS3NavDome->getLastMessageTime();
        msgLagTime = currentTime - lastMsgTime;            
        
        if (WaitingforReconnectDome)
        {
            if (msgLagTime < 200)
            {
                WaitingforReconnectDome = false; 
            }            
            lastMsgTime = currentTime;
        }
        
        if (currentTime >= lastMsgTime)
        {
            msgLagTime = currentTime - lastMsgTime;
              
        }
        else
        {
            msgLagTime = 0;
        }
        
        if ( msgLagTime > 10000 )
        {
            SHADOW_DEBUG("It has been 10s since we heard from the PS3 Dome Controller\nmsgLagTime:%u  lastMsgTime:%u  millis: %lu\n",
                          msgLagTime, lastMsgTime, millis())
            SHADOW_DEBUG("Disconnecting the Foot controller\n")
            
            SyR->stop();
            PS3NavDome->disconnect();
            WaitingforReconnectDome = true;
            return true;
        }

        //Check PS3 Signal Data
        if (!PS3NavDome->getStatus(Plugged) && !PS3NavDome->getStatus(Unplugged))
        {
            // We don't have good data from the controller.
            //Wait 100ms, Update USB, and try again
            delay(100);
            
            // Usb.Task();
            lastMsgTime = PS3NavDome->getLastMessageTime();
            
            if(!PS3NavDome->getStatus(Plugged) && !PS3NavDome->getStatus(Unplugged))
            {
                badPS3DataDome++;
                SHADOW_DEBUG("\n**Invalid data from PS3 Dome Controller. - Resetting Data**\n")
                return true;
            }
        }
        else if (badPS3DataDome > 0)
        {
            badPS3DataDome = 0;
        }
        
        if (badPS3DataDome > 10)
        {
            SHADOW_DEBUG("Too much bad data coming from the PS3 DOME Controller\n")
            SHADOW_DEBUG("Disconnecting the controller and stop motors.\n")

            SyR->stop();
            PS3NavDome->disconnect();
            WaitingforReconnectDome = true;
            return true;
        }
    }
    return false;
}

// =======================================================================================
//           USB Read Function - Supports Main Program Loop
// =======================================================================================

bool readUSB()
{     
    Usb.Task();

    //The more devices we have connected to the USB or BlueTooth, the more often Usb.Task need to be called to eliminate latency.
    if (PS3NavFoot->PS3NavigationConnected) 
    {
        if (criticalFaultDetect())
        {
            //We have a fault condition that we want to ensure that we do NOT process any controller data!!!
            return false;
        }   
    }
    else if (!isFootMotorStopped)
    {
        SHADOW_DEBUG("No foot controller was found\n")
        SHADOW_DEBUG("Shuting down motors, and watching for a new PS3 foot message\n")

        ST->stop();
        isFootMotorStopped = true;
        footDriveSpeed = 0;
        WaitingforReconnect = true;
    }
    
    if (PS3NavDome->PS3NavigationConnected) 
    {
        if (criticalFaultDetectDome())
        {
           //We have a fault condition that we want to ensure that we do NOT process any controller data!!!
           return false;
        }
    }    
    return true;
}
