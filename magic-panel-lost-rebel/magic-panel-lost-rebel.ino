// Magic Panel FX by IA-PARTS.com 
//
//// Release History
//      - Add support for Roddbotics panel, bug fixes and cleanup (LostRebel 02-28-2022)
// v011 - Cleanup of code, additional documentation added (TheJugg1er 02-18-2020)
// v010.6 - Added Serial interface support and additional sequences (TheJugg1er 01-05-2020)
// v010.5 - Re-added Working I2C and additional display sequences  (FlthyMcNsty 05-21-2014)
// v010 - Remove I2C code and clean up
// v009 - Combine Big Happy Dude functions to v008 + allow for 3 pin binary input
// v008 - Final Code Release May 24 2013 - D Dobyns
// v007 - New Functions for Program Pin Control - D Dobyns
// v006 - Jumper Pin Program - D Dobyns
// v005 - Cleaned code - Production
// v004 - Added JEDI support for JEDI Serial Address 10 decimal 
// v003 - Decode events... next add selectable jumper support.
// v002 - Added default operation & I2C support
// v001 - Initial Demo Sketch
//

/*
 * The Full description of all command sequences are listed below.  These are called using either i2c or serial and supplying
 * the mode number as listed.
 * 
 * The Code has two modes of operating.  It is possible to set this during runtime of the panel, however the default is controlled
 * in the code below.  To set the Default mode, change the variable "alwaysOn" to be either true or false.  The default here is false.
 * 
 * If alwaysOn is set to False, the patterns will display for a given time, and then turn off.
 * If alwaysOn is set to True, the pattern will display until the T00 (off) command is received. (or loop until T00 is received).
 * 
 * To change the Default, Command P0 will set to "false", Command P1 will set to "true", overriding the default for this session.
 * 
 * The Magic Panel uses JawaLite commands to trigger the patterns.  Currently only A, D, P, and T commands from the JawaLite 
 * spec are supported.
 * 
 * Command A will turn the panel on
 * 
 * Command D will turn the panel off
 * 
 * Command P will set the default panel mode as described above.
 * 
 * Command T will trigger a numbered pattern.  Txx where xx is the pattern number below. 
 * 
 * When sending i2c command the Panel Address is defined below to be 20.  The command type and value are needed.  To trigger a
 * pattern, send character 'T' and the value corresponding to the pattern list below to trigger the corresponding sequence.  
 * Sequences must be terminated with a carriage return (\r)
 * 
 * On the MarcDuino i2c can be connected from either the Master or Slave
 * 
 * e.g. &20,x54,2\r    -  This will turn on the MP for ~2 seconds.
 * 
 * When sending commands via serial, no address is required as Serial is a point to point communication protocol. 
 * Sequences must be terminated with a carriage return (\r)
 * 
 * To use the MarcDuino Serial triggering, connect Pin 3 of the Magic Panel (Rx) to the "To Slave" Signal pin on the 
 * MarcDuino Slave.
 * 
 * e.g. %T2      - This will turn on the MP for ~2 seconds.
 * 
 * Mode 0 - Turn Panel off
 * Mode 1  - Turn Panel on Indefinately
 * Mode 2  - Turn Panel on for 2s
 * Mode 3  - Turn Panel on for 5s
 * Mode 4  - Turn Panel on for 10s
 * Mode 5  - Begins Toggle Sequence: Top and Bottom Half of Panel Alternate
 * Mode 6  - Begins Alert Sequence (4s): Panel Rapidly Flashes On & Off
 * Mode 7  - Begins Alert Sequence (10s): Panel Rapidly Flashes On & Off
 * Mode 8  - Begins Trace Up Sequence (Filled): Each Row of the MP lights up from bottom to top filling entire panel
 * Mode 9  - Begins Trace Up Sequence (No Fill):  Each row of the MP lights up from bottom to top individually
 * Mode 10 - Begins Trace Down Sequence (Filled):  Each row of the MP lights up from top to bottom filling entire panel
 * Mode 11 - Begins Trace Down Sequence (No Fill):  Each row of the MP lights up from top to bottom individually
 * Mode 12 - Begins Trace Right Sequence (Filled):  Each column of the MP lights up from left to right filling entire panel
 * Mode 13 - Begins Trace Right Sequence (No Fill):  Each column of the MP lights up from left to right individually
 * Mode 14 - Begins Trace Left Sequence (Filled):  Each column of the MP lights up from right to left filling entire panel
 * Mode 15 - Begins Trace Left Sequence (No Fill):  Each column of the MP lights up from right to left individually
 * Mode 16 - Begins Expand Sequence (Filled): Panel expands from center filling entire panel
 * Mode 17 - Begins Expand Sequence (No Fill): Ring of pixels expands from center of panel
 * Mode 18 - Begins Compress Sequence (Filled): Panel compresses from outer edge filling entire panel
 * Mode 19 - Begins Compress Sequence (No Fill): Ring of pixels compresses from outer edge of panel
 * Mode 20 - Begins Cross Sequence: Panel is lit to display an X for 3s
 * Mode 21 - Begins Cylon Column Sequence: Each column illuminated one at a time from left to right back to left. (like the Cylons from Battlestar Galactica)
 * Mode 22 - Begins Cylon Row Sequence: Each row illuminated one at a time from top to bottom back to top. (like the Cylons from Battlestar Galactica)
 * Mode 23 - Begins Eye Scan Sequence:  Each row is illuminated from top to bottom followed by each column left to right. (like the eye ball scanners in the Mission Impossible movies)
 * Mode 24 - Begins Fade Out/In Sequence:  MP gradually and randomly fades out and then fades back in the same manner. 
 * Mode 25 - Begins Fade Out Sequence:  MP gradually and randomly fades out (Similar to the Short Circuit Sequence on Teeces).
 * Mode 26 - Begins Flash Sequence:  MP flashes rapidly for 5 seconds (Similar to Alarm Sequence)
 * Mode 27 - Begins Flash V Sequence: Left and Right Half of Panel Alternate
 * Mode 28 - Begins Flash Q Sequence:  Alternating quadrants of MP flash rapidly
 * Mode 29 - Begins Two Loop Sequence: Dual pixels are lit opposite each other completing a loop around the 2nd ring from panel edge.
 * Mode 30 - Begins One Loop Sequence: A single pixel is lit individually completing a loop around the 2nd ring from panel edge.
 * Mode 31 - Begins Test Sequence (Type 1):  Each pixel of the MP is lit sequentially from row 0, column 7 to row 7, column 0 until panel is filled, then unlit in the same order.
 * Mode 32 - Begins Test Sequence (Type 2):  Each pixel of the MP is lit indivually from row 0, column 7 to row 7, column 0. 
 * Mode 33 - Begins AI Logo Sequence:  Displays the AI Aurebesh characters for 3s (...that we see all over our awesome packages from Rotopod and McWhlr)
 * Mode 34 - Begins 2GWD Logo Sequence: Displays the Characters 2-G-W-D sequentially every 1s (...shameless, I know.) 
 * Mode 35 - Begins Quadrant Sequence (Type 1):  Each Panel Quadrant lights up individually (TL, TR, BL, BR)
 * Mode 36 - Begins Quadrant Sequence (Type 2):  Each Panel Quadrant lights up individually (TR, TL, BR, BL) 
 * Mode 37 - Begins Quadrant Sequence (Type 3):  Each Panel Quadrant lights up individually (TR, BR, BL, TL)
 * Mode 38 - Begins Quadrant Sequence (Type 4):  Each Panel Quadrant lights up individually (TL, BL, BR, TR)
 * Mode 39 - Begins Random Pixel Sequence:  Random pixels flashe individually for 6s
 * Mode 40 - Begins Countdown from 9 to 0 (takes 10 seconds)
 * Mode 41 - Begins Countdown from 3 to 0 (takes 4 seconds)
 * Mode 42 - Begins Alert Sequence (4s):  Panel Randomly Flashes On & Off, mimics the original MarcDuino code
 * Mode 43 - Begins Alert Sequence (8s):  Panel Randomly Flashes On & Off, mimics the original MarcDuino code
 * Mode 44 - Begins Smiley Face
 * Mode 45 - Begins Sad Face
 * Mode 46 - Begins Heart
 * Mode 47 - Begins Flash Checkerboard sequence.  Flashes small squares in a checkerboard pattern
 * Mode 48 - Begins Compress In Type 1 - Fills the rows from TL/BR in half row increments 5 times
 * Mode 49 - Begins Compress In Type 2 - Fills the rows from TL/BR in half row increments then clears in the same order 5 times
 * Mode 50 - Begins Explode Out Type 1 - Fills the rows from Centre Out in half row increments 5 times
 * Mode 51 - Begins Explode Out Type 2 - Fills the rows from Centre Out in half row increments then clears in the same order 5 times
 * Mode 52 - Begins VU Meter - Randomly ramp up/down on the columns to simulate a VU Meter. (Type 1 Peak up)
 * Mode 53 - Begins VU Meter - Randomly ramp up/down on the rows to simulate a VU Meter. (Type 2 Peak left)
 * Mode 54 - Begins VU Meter - Randomly ramp up/down on the columns from the top to simulate a VU Meter. (Type 3 Peak down)
 * Mode 55 - Begins VU Meter - Randomly ramp up/down on the rows from the right to simulate a VU Meter. (Type 4 Peak right)
 * Mode 56 - Begins Rebound Sequence (Filled) - Expand and Compress pattern back to back
 * Mode 57 - Begins Rebound Sequence (No Fill) - Expand and Compress ring pattern back to back
 */

/*
 * If/when more patterns are added, change this define and change the switch/case in RunPattern accordingly
 */
#define PATTERN_MAX 59


//We always have to include the library
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
   #include "WProgram.h"
#endif
#include "LedControl.h"
#include "Wire.h"

// Uncomment this if you want debug messages
//#define DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Select your MagicPanel hardware here  ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MP_IAPARTS_V1
//#define MP_IAPARTS_V2
//#define MP_IAPARTS_V3
#define MP_RODDBOTICS_V2


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Assign IC2 Address Below   //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte I2CAdress = 20;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////  Patterns always on define  //////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool alwaysOn = true;     // false is default behaviour.  Pattern will turn off after running (or given time) 
                           // Set to true to have patterns remain on until Mode 0 is selected.
                           // Note this can cause some slight delays in response from the panel.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Current and last time state
unsigned long time_ms       	= 0;
unsigned long last_time_ms  	= 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The minimum number of msecs that must elapse before we let the main loop do any work
#define MIN_TIME_DELTA_MS	1 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Random Pattern Mode
// These are the duration and interval defaults. Values are in msecs. Make sure that MIN < MAX.
//
// Interval is how much time should elapse before the next random pattern is played.
//
// Duration is how long a pattern will play. The Random driver will check after each invocation of the pattern
// function if the duration has elapsed and if not, invoke the pattern again. Patterns will thus complete their
// standard executions and are not preemptable if the duration elapses prior to completion.
//
// So rather than running long duration versions of a pattern (eg. #4 All ON for 10 sec or #7 Alert for 10s),
// the Random driver will run the shorter versions of those patterns (#2 and #6 respectively) and repeat it for
// the desired duration, this lets's it be preempted at the shorter pattern duration which is better than
// blocking for longer pattern duration.
//
// Patterns which don't make much sense to repeat (eg. Countdown) are never repeated regardless of the requested
// duration.
// 

#define RANDOM_MIN_DURATION_MS 		 20000 //  5 secs
#define RANDOM_MAX_DURATION_MS 		 21000 //  8 secs
#define RANDOM_MIN_FAST_INTERVAL_MS 	 8000 //  8 secs
#define RANDOM_MAX_FAST_INTERVAL_MS     14000 // 14 secs
#define RANDOM_MIN_SLOW_INTERVAL_MS     40000 // 40 secs
#define RANDOM_MAX_SLOW_INTERVAL_MS     60000 // 60 secs
#define RANDOM_MIN_JMPR_INTERVAL_MS     60000 //  1 min
#define RANDOM_MAX_JMPR_INTERVAL_MS    120000 //  2 min


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time state for random activity
// We need to know:
// The last time any pattern ran
// The time the next random patter should run
unsigned long LastPatternTimeMS = 0;
unsigned long RandomDurationMS = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED Intensity: Value from 0 to 15 with 15 being the brightest
//
// When installed in the dome behind a lens and sticker we want maximum brightness 15.
// Set it to 0 (zero) when developing hardware to avoid going blind!

#define MP_INTENSITY 15

////////////////////////////////////////////////////////////////////////////////////////////////////
// To load a sketch onto Magic Panel as Arduino Duemilanove w/ ATmega328
//
// Now we need a LedControl to work with.
// ***** These pin numbers will probably not work with your hardware *****
//
// MAX7221
// Pin 1 - Data IN
// Pin 12 - Load
// Pin 13 - CLK
// Pin 24 - Data Out
//
// Top 7221    = 0
// Bottom 7221 = 1
//
// Assign the pins from the ATMega328p to LedControl
// 
// pin D8 is connected to the DataIn 
// pin D7 is connected to the CLK 
// pin D6 is connected to LOAD 
// We have two MAX7221 on the Magic Panel, so we use 2 for device count.
//
// --------------------------------------------------------------
// RoddBotics Magic Panel v2.1 using 1 Max7221 and Arduino Nano
// --------------------------------------------------------------
// pin D12 is connected to the DataIn 
// pin D11 is connected to the CLK 
// pin D10 is connected to LOAD
// 
//

#define MP_ROWS 8
#define MP_COLS 8

#if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
# define TWO_MAX_CHIPS
# define HAVE_JUMPERS
# define HAVE_DIGITAL_CMD_INPUT

LedControl lc = LedControl(8,7,6,2);

#elif defined(MP_RODDBOTICS_V2)

LedControl lc = LedControl(12,11,10,1);

#else

hardware not defined

#endif


#if defined(HAVE_DIGITAL_CMD_INPUT) || defined(HAVE_JUMPERS)
bool first_time       = true;	// used for 4-bit (0 2 3 5) input reading - reset all variables
  			        // when 4-bit address value changes (except the first time on power up)
byte DigInState       = 0;
byte lastDigInState;
#endif


////////////////////////////////////////////////////////////////////////////////
// Board Layout and MAX chip mapping
//
// The physical and logical layout of the pixels need not match. This sketch
// uses a virtual pixel array (VMagicPanel) for the code to draw in and then maps
// that to byte array bitmap (MagicPanel) used when calling the LedControl
// routines which handle the MAX chip interface.
//
// IA-Parts
// --------
// The IA-Parts boards have two MAX chips, each driving 4 rows
// Each half row of pixels is driven by one row in the MAX chip
//
// So the physical layout is
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R0  R0  R0  R0  R0  R0  R0  R0
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R1  R1  R1  R1  R1  R1  R1  R1
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R2  R2  R2  R2  R2  R2  R2  R2
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R3  R3  R3  R3  R3  R3  R3  R3
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R4  R4  R4  R4  R4  R4  R4  R4
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R5  R5  R5  R5  R5  R5  R5  R5
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R6  R6  R6  R6  R6  R6  R6  R6
//
//     C7  C6  C5  C4  C3  C2  C1  C0
//     R7  R7  R7  R7  R7  R7  R7  R7
//
// The chip grid layout is:
//
// Chip      Chip 0 Column               Chip      Chip 1 Column
// Row  7   6   5   4   3   2   1   0    Row  7   6   5   4   3   2   1   0
//  0  C7  C6  C5  C4                     0  C7  C6  C5  C4
//     R0  R0  R0  R0                        R4  R4  R4  R4
//  1                  C3  C2  C1  C0     1                  C3  C2  C1  C0
//                     R0  R0  R0  R0                        R4  R4  R4  R4
//  2  C7  C6  C5  C4                     2  C7  C6  C5  C4
//     R1  R1  R1  R1                        R5  R5  R5  R5
//  3                  C3  C2  C1  C0     3                  C3  C2  C1  C0
//                     R1  R1  R1  R1                        R5  R5  R5  R5
//  4  C7  C6  C5  C4                     4  C7  C6  C5  C4
//     R2  R2  R2  R2                        R6  R6  R6  R6
//  5                  C3  C2  C1  C0     5                  C3  C2  C1  C0
//                     R2  R2  R2  R2                        R6  R6  R6  R6
//  6  C7  C6  C5  C4                     6  C7  C6  C5  C4
//     R3  R3  R3  R3                        R7  R7  R7  R7
//  7                  C3  C2  C1  C0     7                  C3  C2  C1  C0
//                     R3  R3  R3  R3                        R7  R7  R7  R7
//
// Note: there is no harm in trying to address chip pixels that have no
//       corresponding physical led. So writing B11111111 to Chip 0, Row 0
//       has the same effect as writing B11110000, turning on Row 0 pixels
//       C4, C5, C6, and C7.
//
// RoddBotics
// ----------
//
// The Roddbotics boards have one MAX chip so conceptually easier to deal with except the
// the row/column layout is rotated. MapBoolGrid takes care of rotating the matrix just before
// output so you don't need to care. 
//
// The chip grid layout is:
//
// Chip      Chip 0 Column               
// Row  7   6   5   4   3   2   1   0    
//  0  C0  C0  C0  C0  C0  C0  C0  C0                        
//     R0  R1  R2  R3  R4  R5  R6  R7                        
//  1  C1  C1  C1  C1  C1  C1  C1  C1                        
//     R0  R1  R2  R3  R4  R5  R1  R7 
//  2  C2  C2  C2  C2  C2  C2  C2  C2
//     R0  R1  R2  R3  R4  R5  R6  R7
//  3  C3  C3  C3  C3  C3  C3  C3  C3
//     R0  R1  R2  R3  R4  R5  R6  R7
//  4  C4  C4  C4  C4  C4  C4  C4  C4
//     R0  R1  R2  R3  R4  R5  R6  R7
//  5  C5  C5  C5  C5  C5  C5  C5  C5
//     R0  R1  R2  R3  R4  R5  R6  R7
//  6  C6  C6  C6  C6  C6  C6  C6  C6
//     R0  R1  R2  R3  R4  R5  R6  R7
//  7  C7  C7  C7  C7  C7  C7  C7  C7
//     R0  R1  R2  R3  R4  R5  R6  R7
//
// 

#ifdef TWO_MAX_CHIPS
byte MagicPanel[2 * MP_ROWS]; // The 1D array representing the state of each half row
#else
byte MagicPanel[MP_ROWS];  // The 1D array representing the state of each whole row
#endif 
boolean VMagicPanel[MP_ROWS][MP_COLS];    // The 2D array of logical pixels



////////////////////////////////////////////////////////////////////////////////////////////////////
// To avoid "blink" when invoking the same pattern multiple times like alwaysOn mode or when the
// Random driver is repeating a pattern for a duration, the RunPattern function will skip blanking
// the display if the same pattern is run in succession.
//
// Rule of thumb: If your pattern ends with the same pixels on as when it starts (eg. Heart, Smile)
// you probably want to prefix invocation of your pattern with
//
//    if (firstTimeCode)
//      allOFF();
//    yourPattern();
//
bool firstTimeCode           = true;

// String processing from Serial
String inputString           = "";    // a string to hold incoming data
boolean stringComplete       = false; // whether the string is complete
boolean stringEvaluated      = false; // whether the serial "packet" has been processed

//Serial Stuff
int lastMPeventCode          = 0;

//Timing stuff
unsigned long patternEndTimeMS = 0;
boolean patternActive          = false;


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Pattern Function Modes
//
// If your pattern function has display modes, enumerate them here to make the code easier to read
// Note: The specified constant values are important as they are used to index into arrays by some patterns
//   and we depend on the fact that values are assigned sequentially. Although the standard defines that
//   values will start from zero, we explicitly start from zero to make it clear that we care since we
//   we often use the value to index into arrays.

enum PatternCylonMode { CylonModeLeftRight = 0, CylonModeUpDown };
enum PatternFlashMode { FlashModeAll = 0, FlashModeUpDown, FlashModeLeftRight, FlashModeQuadrant };
enum PatternTraceMode { TraceModeUp = 0, TraceModeDown, TraceModeLeft, TraceModeRight };
enum PatternExpandCompressMode { ExpandCompressModeExpand = 0, ExpandCompressModeCompress, ExpandCompressModeRebound };
enum PatternLoopMode { LoopModeOne = 0, LoopModeTwo };
enum PatternFadeOutInMode { FadeOutInModeOut = 0, FadeOutInModeOutIn };
enum PatternQuadrantMode { QuadrantMode1 = 0, QuadrantMode2, QuadrantMode3, QuadrantMode4, QuadrantMode5 };
enum PatternVUMode { VUModeUp = 0, VUModeLeft, VUModeDown, VUModeRight };
enum PatternCompressExplodeMode { CompressExplodeModeCompress = 0, CompressExplodeModeExplode };

// Fill and NoFill are used by multiple patterns so we call it an option not a mode.
// Note: The specified values are important as they are used to index into arrays by some patterns
enum PatternCommonFillOpt { OptFill = 0, OptNoFill };


////////////////////////////////////////////////////////////////////////////////////////////////////
// setup
//
// Actions that must be performed before the arduino starts invoking the main loop
//
// Here we:
//    - Register our I2C address and handler
//    - setup our serial interface
//    - initialize our MAX chip(s)
//    - seed the random generator
//    - setup the jumper pins (for platforms that support it)
//    - setup the digital command input pins (for platforms that support it)

void setup() 
{ 
  Wire.begin(I2CAdress);                   // Start I2C Bus as Master I2C Address
  Wire.onReceive(receiveEvent);            // register event so when we receive something we jump to receiveEvent();

  // Setup the Serial Interface
  Serial.begin(9600);
  inputString.reserve(200);

  /*
   * The MAX72XX is in power-saving mode on startup,
   * We have to:
   *  1. do a wakeup call,
   *  2. set the LED brightness
   *  3. and clear the display 
   */
  lc.shutdown(0, false);
#ifdef TWO_MAX_CHIPS  
  lc.shutdown(1, false);
#endif  

  lc.setIntensity(0, MP_INTENSITY);
#ifdef TWO_MAX_CHIPS  
  lc.setIntensity(1, MP_INTENSITY);
#endif

  lc.clearDisplay(0);
#ifdef TWO_MAX_CHIPS  
  lc.clearDisplay(1);
#endif

  randomSeed(analogRead(A3));           // Randomizer

  // Test Pixels
  TheTest(30);

  // SETUP 6 DIGITAL PINS FOR MANUAL CONTROL
#ifdef HAVE_JUMPERS  
# if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
  // Jumper Pins used by MP_IAPARTS_*
  pinMode(11, INPUT);             // set pin 11 to input - input 3  
  pinMode(12, OUTPUT);            // set pin PB4 to output - pin 4 - used to allow a jumper from pin 4 to adjacent pin to pull down the adjacent pin
  pinMode(13, INPUT);             // set pin 13 to input - input 5

  digitalWrite(11, HIGH);         // turn on pullup resistors
  digitalWrite(13, HIGH);         // turn on pullup resistors
  digitalWrite(12, LOW);          // set pin PC1 to output - pin 1
# endif /* MP_IAPARTS_* */
#endif /* HAVE_JUMPERS */
  
#ifdef HAVE_DIGITAL_CMD_INPUT
# if defined(MP_IAPARTS_V1) || defined(MP_IAPARTS_V2) || defined(MP_IAPARTS_V3)
  digitalWrite(A0, HIGH);         // turn on pullup resistors
  digitalWrite(A1, HIGH);         // turn on pullup resistors
  digitalWrite(A2, HIGH);         // turn on pullup resistors
# endif  
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stuff that should move into a header like MagicPanel_PatternData.h

/////////////////////////////////////////////////////////////////////////////////
// Pattern data
//    a) stored in arrays so that it can be displayed programatically and
//    b) stored in PROGMEM to save sram space
// 

////////////////////////////////////////////////////////////////////////////////////////////////////
// Used by pattern 33 (IA logo), 20 (Cross), 44 (Smile), 45 (Sad), 46 (Heart).

#define SYMBOLS_AND_FACES_PANEL_COUNT 5
enum SymFacePanel { SymFaceIA = 0, SymFaceCross, SymFaceSmile, SymFaceSad, SymFaceHeart };

const static byte symFaceTable[] PROGMEM  = {
 B00000100,                                    /* IA Aurebesh */
 B00000010,
 B11111111,
 B00000000,
 B11100111,
 B00100100,
 B00100100,
 B01000010,

 B00000000,                                    /* Cross */
 B01000010,
 B00100100,
 B00011000,
 B00011000,
 B00100100,
 B01000010,
 B00000000,

 B00111100,                                    /* Smile */
 B01000010,
 B10100101,
 B10000001,
 B10100101,
 B10011001,
 B01000010,
 B00111100,

 B00111100,                                    /* Sad */
 B01000010,
 B10100101,
 B10000001,
 B10011001,
 B10100101,
 B01000010,
 B00111100,

 B00000000,                                    /* Heart */
 B01100110,
 B11111111,
 B11111111,
 B01111110,
 B00111100,
 B00011000,
 B00000000
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Used by patterns 16, 17, (Expand) 18, 19 (Compress), 56, 57 (Rebound)

/* The expand/compress function uses 4 panel scenes to animate */
#define EXPAND_COMPRESS_TABLE_PANEL_COUNT 8
#define EXPAND_COMPRESS_TABLE_FRAMES      4
/*
 * This is an array of bytes but easier to think of it as a 3D array (4x8x2)
 *
 * The 4 panels for the expand filled sequence followed by the
 *     4 panels for the expand unfilled sequence.
 *
 * Compress just uses the same panels but played in reverse sequence.
 */
const static byte expandTable[] PROGMEM  = {
 B00000000,                                    /* Fill */
 B00000000,
 B00000000,
 B00011000,
 B00011000,
 B00000000,
 B00000000,
 B00000000,  

 B00000000,
 B00000000,
 B00111100,
 B00111100,
 B00111100,
 B00111100,
 B00000000,
 B00000000,   

 B00000000,
 B01111110,
 B01111110,
 B01111110,
 B01111110,
 B01111110,
 B01111110,
 B00000000,      

 B11111111,
 B11111111,
 B11111111,
 B11111111,
 B11111111,
 B11111111,
 B11111111,
 B11111111,      

 B00000000,                                    /* No Fill */
 B00000000,
 B00000000,
 B00011000,
 B00011000,
 B00000000,
 B00000000,
 B00000000,      

 B00000000,
 B00000000,
 B00111100,
 B00100100,
 B00100100,
 B00111100,
 B00000000,
 B00000000,      

 B00000000,
 B01111110,
 B01000010,
 B01000010,
 B01000010,
 B01000010,
 B01111110,
 B00000000,      

 B11111111,
 B10000001,
 B10000001,
 B10000001,
 B10000001,
 B10000001,
 B10000001,
 B11111111
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Used by pattern 34
#define FLTHY_PANEL_COUNT 4

const static byte flthyTable[] PROGMEM  = {
 B00000000,                                    /* 2 */
 B01111110,
 B00000010,
 B01111110,
 B01000000,
 B01000000,
 B01111110,
 B00000000,
 
 B00000000,                                    /* G */
 B01111110,
 B01000010,
 B01000000,
 B01001110,
 B01000010,
 B01111110,
 B00000000,
 
 B00000000,                                    /* W */
 B01000010,
 B01000010,
 B01000010,
 B01011010,
 B01100110,
 B01000010,
 B00000000,
 
 B00000000,                                    /* D */
 B01111100,
 B01000010,
 B01000010,
 B01000010,
 B01000010,
 B01111100,
 B00000000
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Used by patterns 40, 41 and by the JawaCommand parser to communicate confirmation of P commands
#define NUM_TABLE_PANEL_COUNT 10

const static byte numTable[] PROGMEM  = {
 B00111000,                                    /* Zero */
 B01000100,
 B01000100,
 B01000100,
 B01000100,
 B01000100,
 B01000100,
 B00111000,

 B00010000,                                    /* One */
 B00110000,
 B00010000,
 B00010000,
 B00010000,
 B00010000,
 B00010000,
 B00111000,

 B00111000,                                    /* Two */
 B01000100,
 B00000100,
 B00000100,
 B00001000,
 B00010000,
 B00100000,
 B01111100,

 B00111000,                                    /* Three */
 B01000100,
 B00000100,
 B00011000,
 B00000100,
 B00000100,
 B01000100,
 B00111000,

 B00000100,                                    /* Four */
 B00001100,
 B00010100,
 B00100100,
 B01000100,
 B01111100,
 B00000100,
 B00000100,

 B01111100,                                    /* Five */
 B01000000,
 B01000000,
 B01111000,
 B00000100,
 B00000100,
 B01000100,
 B00111000,

 B00111000,                                    /* Six */
 B01000100,
 B01000000,
 B01111000,
 B01000100,
 B01000100,
 B01000100,
 B00111000,

 B01111100,                                    /* Seven */
 B00000100,
 B00000100,
 B00001000,
 B00010000,
 B00100000,
 B00100000,
 B00100000,

 B00111000,                                    /* Eight */
 B01000100,
 B01000100,
 B00111000,
 B01000100,
 B01000100,
 B01000100,
 B00111000,

 B00111000,                                    /* Nine */
 B01000100,
 B01000100,
 B01000100,
 B00111100,
 B00000100,
 B01000100,
 B00111000
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Used by Quadrant patterns 35, 36, 37, 38

// Map "names" to the panels in quadrantTable
enum QuadrantTablePanel { QuadrantTL   = 0,
                          QuadrantTR   = 1,
                          QuadrantBR   = 2,
                          QuadrantBL   = 3,
                          QuadrantTLBR = 4,
                          QuadrantTRBL = 5};

#define QUADRANT_TABLE_PANEL_COUNT 6         /* n panels in quadrantTable */
#define QUADRANT_SEQUENCE_LENGTH 4           /* Sequences of n frames */
#define QUADRANT_SEQUENCE_COUNT  5           /* n sequences defined */

const static byte quadrantSequenceTable[] PROGMEM = {
  QuadrantTL, QuadrantTR, QuadrantBL, QuadrantBR,
  QuadrantTR, QuadrantTL, QuadrantBR, QuadrantBL,
  QuadrantTR, QuadrantBR, QuadrantBL, QuadrantTL,
  QuadrantTL, QuadrantBL, QuadrantBR, QuadrantTR,
  QuadrantTLBR, QuadrantTRBL, QuadrantTLBR, QuadrantTRBL
};

const static byte quadrantTable[] PROGMEM  = {
 B11110000,                                    /* Quandrant TL */
 B11110000,
 B11110000,
 B11110000,
 B00000000,
 B00000000,
 B00000000,
 B00000000,

 B00001111,                                    /* Quadrant TR */
 B00001111,
 B00001111,
 B00001111, 
 B00000000,
 B00000000,
 B00000000,
 B00000000,

 B00000000,                                    /* Quadrant BR */
 B00000000,
 B00000000,
 B00000000,
 B00001111,
 B00001111,
 B00001111,
 B00001111, 
 
 B00000000,                                    /* Quadrant BL */
 B00000000,
 B00000000,
 B00000000,
 B11110000,
 B11110000,
 B11110000,
 B11110000,

 B11110000,                                    /* Quadrant TL/BR */
 B11110000,
 B11110000,
 B11110000,
 B00001111,
 B00001111,
 B00001111,
 B00001111, 

 B00001111,                                    /* Quadrant TR/BL */
 B00001111,
 B00001111,
 B00001111, 
 B11110000,
 B11110000,
 B11110000,
 B11110000
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Used in the VU Meter (pattern 52, 53, 54, 55 to display "levels"
const static byte bargraph[] PROGMEM = {
 B00000000,
 B10000000,
 B11000000,
 B11100000,
 B11110000,
 B11111000,
 B11111100,
 B11111110,
 B11111111,
 B01111111,
 B00111111,
 B00011111,
 B00001111,
 B00000111,
 B00000011,
 B00000001,
 B00000000
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// main loop

void loop()
{ 
  time_ms= millis();                          // get an updated time stamp

  if (time_ms- last_time_ms > MIN_TIME_DELTA_MS) // If enough time has passed...
  {
    last_time_ms = time_ms;			    // reset the timer

#if defined(HAVE_DIGITAL_CMD_INPUT) || defined(HAVE_JUMPERS)
    DigInState = 0;	
# ifdef HAVE_DIGITAL_CMD_INPUT        
    //***********************************************
    // These are the Binary Coded Input Pins
    //
    // read in the 4-bit address line
    if (digitalRead(A0) == LOW)        	// 0
      DigInState = DigInState + 4; 
    if (digitalRead(A1) == LOW)        	// 1
      DigInState = DigInState + 2; 
    if (digitalRead(A2) == LOW)       	// 2
      DigInState = DigInState + 1; 

# endif /* HAVE_DIGITAL_CMD_INPUT */
# ifdef HAVE_JUMPERS    
    //***********************************************
    // Read the Jumper Pins
    // They override anything that came in on the address line
    //
    // Note: The original IA-Parts panel documented the jumpers as follows:
    // Jump pins 0-1 DEMO randomly picks a pattern every 8-12 minutes
    // Jump pins 1-2 SOLID RED
    // Jump pins 3-4 FLASH
    // Jump pins 4-5 FLIP FLOP
    //
    // The code only appears to implement reading the first two jumpers.
    if (digitalRead(11) == LOW)        	// 3
      DigInState = 8;                   // if Jumper-1 is Placed - Solid Red Mode
    if (digitalRead(13) == LOW)        	// 5
      DigInState = 9;                   // if Jumper-2 is Placed - Random Pattern Mode
# endif /* HAVE_JUMPERS */    

    if(first_time)
    {
      lastDigInState = DigInState;	// dont allow a change in DigInState values if this is the first time through
      first_time = false;
    }

    if (DigInState != lastDigInState)  	//if DigInState has changed...
      blankPANEL();                    // Clear LED's

    lastDigInState = DigInState;	// Store the current Input Status  

    switch (DigInState)			// Call the appropriate code routine - based on the jumpers or cmd number read from control lines A B C
    {
      case 0: 			        // DO NOTHING - ALLOW SERIAL OR I2C FUNCTIONS TO OVER-RIDE 6-PIN INPUT
      {
        break;
      }
      case 1: 			        // FADE IN AND OUT: 
      {
        allOFF();
        FadeOutIn(FadeOutInModeOutIn);
        allOFF();
        break;
      }
      case 2: 			        // FLASH ALL FAST: 
      {  
        allOFF();
        Flash(8, 200, FlashModeAll);
        allOFF();
        break;
      }
      case 3: 			        // 2 LOOP: 
      {  
        allOFF();
        Looper(2,LoopModeTwo);      
        allOFF();
        break;
      }
      case 4: 			        // TRACE DOWN: 
      {  
        allOFF();
        Trace(5, TraceModeDown, OptFill);
        allOFF();
        break;
      }
      case 5: 			        // ONE TEST: 
      {  
        allOFF();
        OneTest(30);
        allOFF();
        break;
      }   
      case 6: 			        // RANDOM FAST: 
      {  
        RandomPattern(random(RANDOM_MIN_FAST_INTERVAL_MS,RANDOM_MAX_FAST_INTERVAL_MS));
        break;
      }
      case 7: 			        // RANDOM SLOW: 
      {
        RandomPattern(random(RANDOM_MIN_SLOW_INTERVAL_MS,RANDOM_MAX_SLOW_INTERVAL_MS));
        break;
      }
      case 8:                           // JUMPER 1: 
      {  
        allONTimed(0);   
        break;
      }
      case 9: 			        // JUMPER 2: 
      {  
        RandomPattern(random(RANDOM_MIN_JMPR_INTERVAL_MS,RANDOM_MAX_JMPR_INTERVAL_MS));  
        break;
      }
      default:
      {                                 // OFF: 
        allOFF();                       // Clear LED's
        break;
      }
    }//end switch,
#endif /* HAVE_DIGITAL_CMD_INPUT || HAVE_JUMPERS */

    // End the current Pattern if there is one being "played"
    // This replaces the use of delay() which is a blocking call.
    if (patternActive)
    {
      if (time_ms >= patternEndTimeMS)
      {
        allOFF();
        patternActive = false;
      }
    }

    // If we want the pattern to always be on, then re-call the process function
    // with the same code as the last time.
    // This only applies to patterns that are not "all on" patterns, to avoid a 
    // LED blink due to re-calling the code.
    if (alwaysOn && (lastMPeventCode > 4))
      RunPattern(lastMPeventCode);

  }// end of if(time...)
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// setPatternEndTime
//
// Set the pattern expiration time based on current time plus the specified patternTimeMS and
// set the patternActive flag.
//
// The main loop will check if the expiration time has been reached and turn off the panel and
// clear the patternActive flag.
//

void setPatternEndTime(unsigned long patternTimeMS)
{
  time_ms= millis();
  patternEndTimeMS = time_ms + patternTimeMS;
  patternActive = true;
}


////////////////////////////////////////////////////////////////////////////////
// RandomPattern driver
//
// The Random driver has three states:
//   1. Prepare - selection of pattern and duration, handling any special cases
//   2. Execute - run the pattern then check if we should:
//                  a) stay in state 2 because the duration hasn't elapsed yet or
//                  b) transition to state 3
//   3. Idle    - do nothing until the interval since last pattern has elapsed
//                which transitions back to state 0
//

void RandomPattern(int IntervalMS)
{
  static byte RandomState = 0;
  static byte RandomPatternNum;                    // Pattern to run
  static unsigned long RandomPatternStartTimeMS;
  static unsigned long RandomIntervalMS;
  
  switch (RandomState) 
  {
    // Random prepare state
    case 0:
    {
      RandomIntervalMS = IntervalMS;
      RandomPatternNum = random(0,PATTERN_MAX); // randomly select pattern
      RandomDurationMS = random(RANDOM_MIN_DURATION_MS, RANDOM_MAX_DURATION_MS);
      RandomState++;         // proceed to next state
      RandomPatternStartTimeMS = time_ms;

      /*
       * Special cases.
       * Remap:
       * - off to On
       * - longer sequences to shorter ones to minimize blocking
       * Countdown sequences should only run once, not much sense making them repeat
       */
      switch(RandomPatternNum)
      {
        case 0:  // Off
        case 1:  // On
        case 3:  // On 5s
        case 4:  // On 10s
          RandomPatternNum = 2;
          break;

        case 7:
          RandomPatternNum = 6;
          break;

        case 40:
        case 41:
          RandomDurationMS = 1;
          break;

        case 43:
          RandomPatternNum = 42;
          break;
      }
      
      break;
    }

    // Pattern execute state
    case 1:
    {
      // Run the randomly selected pattern
      if (RandomDurationMS > 0)
        RunPattern(RandomPatternNum);

      if(time_ms >= RandomPatternStartTimeMS + RandomDurationMS) 
        RandomState++;     // proceed to next state

      break;
    }
    
    // Interval Idle State 
    case 2:
    {
       allOFF();         // turn off All Leds
       if (time_ms > LastPatternTimeMS + RandomIntervalMS)
       {
         RandomPatternNum = lastMPeventCode;
         RandomState = 0;     // proceed to next state
       }
       
       break;     
    }     
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// MapBoolGrid
//
// Map the Virtual 2D pixel grid to the 1D byte array that is used to write to the MAX chip(s).
// This routine does any transformations needed like mapping to multiple MAX chips (IA-Parts hardware)
// or screen rotation (RoddBotics hardware)
//

void MapBoolGrid()
{
  for(int Row = 0; Row < MP_ROWS; Row++)
  {
#ifdef TWO_MAX_CHIPS
    MagicPanel[2*Row]=128*VMagicPanel[Row][7]+64*VMagicPanel[Row][6]+32*VMagicPanel[Row][5]+16*VMagicPanel[Row][4];       // 0, 2, 4, 6, 8, 10, 12, 14
    MagicPanel[2*Row+1]=8*VMagicPanel[Row][3]+4*VMagicPanel[Row][2]+2*VMagicPanel[Row][1]+VMagicPanel[Row][0];            // 1, 3, 5, 7, 9, 11, 13, 15
#else
# ifdef MP_RODDBOTICS_V2
    /*
     * V2 PCB has X/Y rotated 90 clockwise so transpose the matrix 90 anti-clockwise
     */
    MagicPanel[Row]=VMagicPanel[7][Row]+2*VMagicPanel[6][Row]+4*VMagicPanel[5][Row]+8*VMagicPanel[4][Row]+
      16*VMagicPanel[3][Row]+32*VMagicPanel[2][Row]+64*VMagicPanel[1][Row]+128*VMagicPanel[0][Row];
# endif
#endif    
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintGrid
//
// PrintGrid first invokes the MapBoolGrid to perform any transformations then invokes the
// LedControl routines to push the state to the MAX chips.

void PrintGrid()
{
  MapBoolGrid();
  
#ifdef TWO_MAX_CHIPS  
  for(int i = 0; i < MP_ROWS * 2; i++)
  {
    if(i < MP_ROWS)
      lc.setRow(0, i, MagicPanel[i]);
    else
      lc.setRow(1, i-8, MagicPanel[i]);
  }
#else
  for(int i = 0; i < MP_ROWS; i++)
    lc.setRow(0, i, MagicPanel[i]);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// SetRow and SetCol
//
// Write the state of a complete row or col to the designated row or column

void SetRow(int LEDRow, unsigned char RowState)
{
  for(int Col = 0; Col < MP_COLS; Col++)
    VMagicPanel[LEDRow][Col] = ((RowState >> Col) & 1);
}

void SetCol(int LEDCol, unsigned char ColState)
{
  for(int Row = 0; Row < MP_ROWS; Row++)
    VMagicPanel[Row][LEDCol] = ((ColState >> Row) & 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// SetPixel and ClearPixel
//
// Set on or off the state an individual pixel in the panel
void SetPixel(int LEDRow, int LEDCol)
{
  VMagicPanel[LEDRow][LEDCol]=true;
}

void ClearPixel(int LEDRow, int LEDCol)
{
  VMagicPanel[LEDRow][LEDCol]=false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadPmemPanel
//
// Load the state of an entire panel from PROGMEM data. It is presumed that the pointer passed
// contains MP_ROWS worth of row data in PROGMEM.
void LoadPmemPanel(byte *panel)
{
  for(int j = 0; j < MP_ROWS; j++)
    SetRow(j, pgm_read_byte(panel+j));

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Test patterns
////////////////////////////////////////////////////////////////////////////////////////////////////

// Turn on all pixels one by one row by row
void TheTest(int FlashDelayMS)
{
  for(int row = 0; row < MP_ROWS; row++)
  {
    for(int col = 0; col < MP_COLS; col++)
    {
      SetPixel(row, col);
      PrintGrid();
      delay(FlashDelayMS);
    }
  }
  for(int row = 0; row < MP_ROWS; row++)
  {
    for(int col = 0; col < MP_COLS; col++)
    {
      ClearPixel(row, col);
      PrintGrid();
      delay(FlashDelayMS);
    }
  }
}

// Turn on and off pixels one by one row by row
void OneTest(int FlashDelayMS)
{
  for(int row = 0; row < MP_ROWS; row++)
  {
    for(int col = 0; col < MP_COLS; col++)
    {
      SetPixel(row, col);
      PrintGrid();
      delay(FlashDelayMS);
      ClearPixel(row, col);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// blankPANEL
//
// blank the display

void blankPANEL()
{
  lc.clearDisplay(0);
#ifdef TWO_MAX_CHIPS  
  lc.clearDisplay(1);
#endif  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// turnOFF
//
// Turn off all pixels unless we are in alwaysOn mode

void turnOff()
{
  if (!alwaysOn)
  {
    allOFF();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// allON and allOFF
//
// Turn on or off the entire panel

void allON()
{
  for(int row = 0; row < MP_ROWS; row++)
  {
    for(int col = 0; col < MP_COLS; col++)
    {
      VMagicPanel[row][col] = true;
    }
  }
  PrintGrid();
}

void allOFF()
{
  for(int row = 0; row < MP_ROWS; row++)
  {
    for(int col = 0; col < MP_COLS; col++)
    {
      VMagicPanel[row][col] = false;
    }
  }
  PrintGrid();
  patternActive = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// allONTimed (FlthyMcNsty)
//
// Turn on the panel but instead of blocking in a delay, set an expiration time which is
// which is inspected in the main loop to terminate the pattern.

void allONTimed(int timer) 
{ 
  allOFF();
  for(int row = 0;row < MP_ROWS; row++)      
    SetRow(row, B11111111);

  PrintGrid();
    
  if (timer < 1)            // Passing a value of 0 or below turns panel on indefinately (well for 1000s anyway)
    setPatternEndTime(1000000);  
  else
    setPatternEndTime(timer);  // Otherwise it stays on for the number of ms passed.
}



////////////////////////////////////////////////////////////////////////////////////////
// EysScan (BHD)
//
//

void EyeScan(int Repeats, int FlashDelayMS)
{
  for(int reps = 0; reps < Repeats; reps++)
  {
    for(int j = 0; j < MP_ROWS; j++)
    {
      SetRow(j, B11111111);
      PrintGrid();
      delay(FlashDelayMS);
      SetRow(j, B00000000);
    }
    allOFF();
    delay(FlashDelayMS);
    for(int j = 0; j < MP_COLS; j++)
    {
      SetCol(MP_COLS - 1 - j, B11111111);
      PrintGrid();
      delay(FlashDelayMS);
      SetCol(MP_COLS - 1 - j, B00000000);
    }
    allOFF();
    delay(FlashDelayMS);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Cylon common function (BHD)
//
// Paint a single line back and forth across the screen left/right or up/down depending type arg:
// 1 = Left/Right, 2 = Up/Down
//
// Paints col (or row) 0 thru 7 ascending then paints col (or row) 6 thru 1 descending.
// This avoids double painting the first and last cols (or rows) providing a smooth cylon effect.
//
// Algorithm uses a function pointer to call SetCol or SetRow based on which type of Cylon we are
// doing, CylonModeLeftRight or CylonModeUpDown
//
// Note: Code presumes a square matrix ROWS_MAX == COLS_MAX
//

void Cylon(int Repeats, int FlashDelayMS, enum PatternCylonMode Mode)
{
  void (*rowcolFn)(int, unsigned char); // Fn ptr for the set row or col procedure
  
  if(Mode == CylonModeLeftRight)                  /* Left-Right */
    rowcolFn = SetCol;
  else                                         /* Up-Down */
    rowcolFn = SetRow;
  
  for(int reps = 0; reps < Repeats; reps++)
  {
    for(int j = 0; j < MP_ROWS; j++)
    {
      rowcolFn(j, B11111111);
      PrintGrid();
      delay(FlashDelayMS);
      rowcolFn(j, B00000000);
    }
    for(int j = 0; j < MP_ROWS - 2; j++)
    {
      rowcolFn(MP_ROWS - 2 - j, B11111111);
      PrintGrid();
      delay(FlashDelayMS);
      rowcolFn(MP_ROWS - 2 - j, B00000000);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Flash common (BHD)
//
// Mode:
//   FlashModeAll       - Solid display (all pixels)
//   FlashModeUpDown    - Top half/Bottom half
//   FlashModeLeftRight - Left half/Right half
//   FlashModeQuadrant  - Quadrant
//

void Flash(int Repeats, int FlashDelayMS, enum PatternFlashMode Mode)
{
  void (*rowcolFn)(int, unsigned char); // Fn ptr for the set row or col procedure
  byte patternA, patternB;

  /* Default is FlashModeAll */
  rowcolFn = SetRow;
  patternA = patternB = B11111111;

   
  if (Mode == FlashModeUpDown)                      /* Horizontal */
  {
    patternB = B00000000;
  }
  else if (Mode == FlashModeLeftRight)             /* Vertical */
  {
    patternB = B00000000;
    rowcolFn = SetCol;
  }
  else if (Mode == FlashModeQuadrant)              /* Quadrant */
  {
    patternA = B00001111;
    patternB = B11110000;
  }

  for(int reps = 0; reps < Repeats; reps++)
  {
    for(int j = 0; j < MP_ROWS/2; j++)
    {
      rowcolFn(j, patternA);
      rowcolFn(j+(MP_ROWS/2), patternB);
    }
    PrintGrid();
    delay(FlashDelayMS);
    for(int j = 0; j < MP_ROWS/2; j++)                /* Now invert the pattern */
    {
      rowcolFn(j, ~patternA);
      rowcolFn(j+(MP_ROWS/2), ~patternB);
    }
    PrintGrid();
    delay(FlashDelayMS);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Looper common function (BHD)
//
// One or Two pixels run around the secondmost outer lane as controlled by Opt.
// The second pixel will be diametrically opposite the first.
//
// Algorithm iterates (Cols - 2) pixels across then
//                    (Rows - 4) pixels down then
//                    (Cols - 2) pixels back then
//                    (Rows - 4) pixels up
// where Rows and Cols are the dimensions of the panel (8 by 8 is standard).
// A picture helps visualize the path:
//
//   . . . . . . . .
//   . x x x x x x .    <- (Cols - 2)
//   . y . . . . y .
//   . y . . . . y .    <- (Rows - 4)
//   . y . . . . y .
//   . y . . . . y .
//   . x x x x x x .    <- (Cols - 2)
//   . . . . . . . .

void Looper(int Repeats, enum PatternLoopMode Mode)
{
  int i;
  boolean two = (Mode == LoopModeTwo) ? true : false;
  int x_steps = MP_COLS - 2;
  int y_steps = MP_ROWS - 4;
  int x_max = MP_COLS - 2;
  int y_max = MP_ROWS - 2;
  
#define LOOPER_DELAY_X 100
#define LOOPER_DELAY_Y 150
  
  for(int reps = 0; reps < Repeats; reps++)
  {
    for(i = 0; i < x_steps; i++)
    {
      SetPixel(1, y_max - i);
      if(two)
        SetPixel(x_max, (1 + i));
      PrintGrid();
      delay(LOOPER_DELAY_X);
      ClearPixel(1, y_max - i);      
      if(two)
        ClearPixel(x_max, (1 + i));
    }
    for(i = 0; i < y_steps; i++) 
    {
      SetPixel(2 + i, 1);
      if(two)
        SetPixel(x_max - 1 - i, y_max);
      PrintGrid();
      delay(LOOPER_DELAY_Y);
      ClearPixel(2 + i, 1);
      if(two)
        ClearPixel(x_max - 1 - i, y_max);
    }
    for(i = 0; i < x_steps; i++)
    {
      SetPixel(x_max, (1 + i));
      if(two)
        SetPixel(1, y_max - i);
      PrintGrid();
      delay(LOOPER_DELAY_X);
      ClearPixel(x_max, (1 + i));
      if(two)
        ClearPixel(1, y_max - i);
    }
    for(i = 0; i < y_steps; i++) 
    {
      SetPixel(x_max - 1 - i, y_max);
      if(two)
        SetPixel(2 + i, 1);
      PrintGrid();
      delay(LOOPER_DELAY_Y);
      ClearPixel(x_max - 1 - i, y_max);
      if(two)
        ClearPixel(2 + i, 1);
    }
  }
#undef LOOPER_DELAY_X
#undef LOOPER_DELAY_Y  
}



////////////////////////////////////////////////////////////////////////////////
// FadeOutIn (BHD)
//
// Light random pixels in decreasing density.
//
// Default is to fade out only.
// If type == FadeOutInModeOutIn then fade in with increasing density after fade out
//
// Note: Increasing FADE_OUTIN_FADESTEPS beyond 5 does not make for a more
//       gradual fade as the pixel density is done by progressively masking
//       random bits (bitwise AND) and after 5 masks you have precious few
//       left already. To improve this you would need to scale
//       FADE_OUTIN_RANDMAX appropriately.
//
// Note: FlthyMcNsty added fade back in option.
//

void FadeOutIn(enum PatternFadeOutInMode Mode)
{
#define FADE_OUTIN_DELAY_MS  150
#define FADE_OUTIN_NUMLOOPS  2   // Run each step this many loops
#define FADE_OUTIN_FADESTEPS 5   // pixel density changes in this many steps
#define FADE_OUTIN_RANDMAX   256  
  unsigned char rowstate;
  int loopi, rowi;
  int fade_step;
  
  /*
   * This loop starts the pattern with high density random pixels
   */
  for(int loopi = 0; loopi < 2 * FADE_OUTIN_NUMLOOPS; loopi++)
  {
    for(int rowi = 0; rowi < MP_ROWS; rowi++)
      SetRow(rowi, (random(FADE_OUTIN_RANDMAX)|random(FADE_OUTIN_RANDMAX)));
    PrintGrid();
    delay(FADE_OUTIN_DELAY_MS);
  }

  /*
   * Fade Out:
   * This loop sets pixels in decreasing density
   * by and'ing random numbers 
   */
  for(fade_step = 0; fade_step <= FADE_OUTIN_FADESTEPS; fade_step++)
  {
    for(loopi = 0; loopi < FADE_OUTIN_NUMLOOPS; loopi++)
    {
      for(rowi = 0; rowi < MP_ROWS; rowi++)
      {
        rowstate = random(FADE_OUTIN_RANDMAX);
        for(int i = 0; i < fade_step; i++)
          rowstate &= random(FADE_OUTIN_RANDMAX);
        
        SetRow(rowi, rowstate);
      }
      PrintGrid();
      delay(FADE_OUTIN_DELAY_MS);
    }
  }
  
  /*
   * Fade In:
   */
  if (Mode == FadeOutInModeOutIn)
  {
    /*
     * This loop sets pixels in increasing density,
     * and'ing random numbers 
     */
    for(fade_step = FADE_OUTIN_FADESTEPS; fade_step >= 0; fade_step--)
    {
      for(loopi = 0; loopi < FADE_OUTIN_NUMLOOPS; loopi++)
      {
        for(rowi = 0; rowi < MP_ROWS; rowi++)
        {
          rowstate = random(FADE_OUTIN_RANDMAX);
          for(int i = fade_step; i > 0; i--)
            rowstate &= random(FADE_OUTIN_RANDMAX);
          
          SetRow(rowi, rowstate);
        }
        PrintGrid();
        delay(FADE_OUTIN_DELAY_MS);
      }
    }

    /*
     * This loop ends the pattern with high density random pixels
     */
    for(loopi = 0; loopi < 2 * FADE_OUTIN_NUMLOOPS; loopi++)
    {
      for(rowi = 0; rowi < MP_ROWS; rowi++)
        SetRow(rowi, (random(FADE_OUTIN_RANDMAX)|random(FADE_OUTIN_RANDMAX)));
      PrintGrid();
      delay(FADE_OUTIN_DELAY_MS);
    }
  }
#undef FADE_OUTIN_DELAY_MS
#undef FADE_OUTIN_NUMLOOPS
#undef FADE_OUTIN_FADESTEPS
#undef FADE_OUTIN_RANDMAX  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// delay used by Symbol and Cross
#define SYMBOL_DELAY_MS 3000

////////////////////////////////////////////////////////////////////////////////////////////////////
// Industrial Automaton "IA" Aurebesh symbol (BHD)

void Symbol()
{

  LoadPmemPanel(&symFaceTable[MP_ROWS * SymFaceIA]);
  PrintGrid();
  delay(SYMBOL_DELAY_MS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// X symbol (BHD)

void Cross()
{
  LoadPmemPanel(&symFaceTable[MP_ROWS * SymFaceCross]);
  PrintGrid();
  delay(SYMBOL_DELAY_MS);
}
#undef SYMBOL_DELAY_MS


////////////////////////////////////////////////////////////////////////////////////////////////////
// delay used by faces and heart
#define FACE_DELAY_MS 1000

////////////////////////////////////////////////////////////////////////////////////////////////////
// Smiley Face (The Jugg1er)

void Smile()
{
  LoadPmemPanel(&symFaceTable[MP_ROWS * SymFaceSmile]);
  PrintGrid();
  delay(FACE_DELAY_MS);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Sad Face (The Jugg1er)

void Sad()
{
  LoadPmemPanel(&symFaceTable[MP_ROWS * SymFaceSad]);
  PrintGrid();
  delay(FACE_DELAY_MS);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Heart (The Juggler)

void Heart()
{
  LoadPmemPanel(&symFaceTable[MP_ROWS * SymFaceHeart]);
  PrintGrid();
  delay(FACE_DELAY_MS);  
}
#undef FACE_DELAY_MS



////////////////////////////////////////////////////////////////////////////////////////
// Trace common function (FlthyMcNsty)
//
// Fill or Sweep up/down/left/right
// Mode specifies direction of fill/sweep
// Fill specifies whether it will be a fill or no-filled sweep

void Trace(int Repeats, enum PatternTraceMode Mode, enum PatternCommonFillOpt Fill)
{
  void (*rowcolFn)(int, unsigned char); // Fn ptr for the set row or col procedure
  byte patternA, patternB;
  int line, startline, lineInc;

#define TRACE_DELAY 200  
  
  rowcolFn = SetRow;
  patternA = patternB = B11111111;
  patternB = B00000000;

  /* Default iteration is increment from 0 */
  startline = 0;
  lineInc = 1;

  /* Up and Left decrement from 7 */
  if(Mode == TraceModeUp || Mode == TraceModeLeft)
  {
    startline = 7;
    lineInc = -1;
  }
  
  /* Left and Right paint the columns not rows */
  if(Mode == TraceModeLeft || Mode == TraceModeRight)
    rowcolFn = SetCol;

  
  for(int reps = 0; reps < Repeats; reps++)
  {
    line = startline;
    
    for(int i = 0; i < MP_ROWS; i++) 
    {
      rowcolFn(line, patternA);
      PrintGrid();
      delay(TRACE_DELAY);
      if (Fill == OptNoFill)
      {
        rowcolFn(line, patternB);
        PrintGrid();
      }
      line += lineInc;
    }
    allOFF(); 
  }

#undef TRACE_DELAY  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// RandomPixel (FlthyMcNsty)
//
// Light single random pixels

void RandomPixel(int Repeats)
{
  int randRow;
  int randCol;

#define RANDOM_PIXEL_DELAY_MS 150
  
  for(int reps = 0; reps < Repeats; reps++)
  {
    randRow = random(0,7);
    randCol = random(0,7);
    SetPixel(randRow, randCol);
    PrintGrid();
    delay(RANDOM_PIXEL_DELAY_MS); 
    allOFF();
  }

#undef RANDOM_PIXEL_DELAY_MS  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadrant (FlthyMcNsty)
//
// Light one quadrant at a time in one of four sequences according to option type.
//
// QuadrantMode1: TL, TR, BL, BR
// QuadrantMode2: TR, TL, BR, BL
// QuadrantMode3: TR, BR, BL, TL
// QuadrantMode4: TL, BL, BR, TR
//
// Implemented but redundant:
// QuadrantMode5: TLBR, TRBL, TLBR, TRBL   (This duplicates Pattern 28 Flash (Quadrant))
//
// The sequences are defined in the quadrantSequenceTable

void Quadrant(int Repeats, enum PatternQuadrantMode Mode)
{
#define QUADRANT_DELAY_MS 200  
  int panel;

  if(Mode >= QUADRANT_SEQUENCE_COUNT) /* Sanity check: Sequence num valid? */
    return;

  for( int i = 0; i < 4; i++)
  {
    panel = pgm_read_byte(&(quadrantSequenceTable[(QUADRANT_SEQUENCE_LENGTH * Mode) + i]));

    if(panel >= QUADRANT_TABLE_PANEL_COUNT) /* Sanity check: Panel num valid? */
      return;
  }
  
  for( int reps = 0; reps < Repeats; reps++)
  {
    for( int i = 0; i < 4; i++)
    {
      panel = pgm_read_byte(&(quadrantSequenceTable[(QUADRANT_SEQUENCE_LENGTH * Mode) + i]));
      LoadPmemPanel(&quadrantTable[MP_ROWS * panel]);
      PrintGrid();
      delay(QUADRANT_DELAY_MS); 
    }
  }
  allOFF();
#undef QUADRANT_DELAY_MS  
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ExpandCompress (FlthyMcNsty)
//
// Expand a box out or Compress a box in with Fill or NoFill
// opt: Expand or Compress
// Mode: Fill or No Fill

void ExpandCompress(int Repeats, enum PatternExpandCompressMode Mode, enum PatternCommonFillOpt FillOpt )
{
#define EXPAND_DELAY 200
  int panel, panelInc, startPanel;

  startPanel = 0;
  panelInc = 1;
  
  if(Mode == ExpandCompressModeCompress)
  {
    startPanel = EXPAND_COMPRESS_TABLE_FRAMES - 1;
    panelInc = -1;
  }
  
  for(int reps = 0; reps < Repeats; reps++)
  {
    panel = startPanel;
    for(int i = 0; i < EXPAND_COMPRESS_TABLE_FRAMES; i++)
    {
      LoadPmemPanel(&expandTable[MP_ROWS * EXPAND_COMPRESS_TABLE_FRAMES * FillOpt + MP_ROWS * panel]);
      PrintGrid();
      delay(EXPAND_DELAY);
      panel += panelInc;
    }
    if(Mode == ExpandCompressModeRebound)
    {
      panel = 3;
      
      for(panel = EXPAND_COMPRESS_TABLE_FRAMES - 1; panel >= 0; panel--)
      {
        LoadPmemPanel(&expandTable[MP_ROWS * EXPAND_COMPRESS_TABLE_FRAMES * FillOpt + MP_ROWS * panel]);
        PrintGrid();
        delay(EXPAND_DELAY);
      }
    }
    
    allOFF();
    delay(EXPAND_DELAY); 
  }
#undef EXPAND_DELAY
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// MySymbol (FlthyMcNsty)
//
// Scroll the 2GWD logo
void MySymbol()
{
#define MYSYMBOL_DELAY_MS 1000

  for(int panel = 0; panel < FLTHY_PANEL_COUNT; panel++)
  {
    LoadPmemPanel(&flthyTable[MP_ROWS * panel]);
    PrintGrid();
    delay(MYSYMBOL_DELAY_MS);
  }
#undef MYSYMBOL_DELAY_MS  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Countdown and ShortCountdown (The Juggler)
//
// Count down from 9 or 3 respectively

#define COUNTDOWN_DELAY_MS 1000  

void Countdown()
{
  for(int i = 9; i >= 0; i--)
  {
    LoadPmemPanel(&numTable[MP_ROWS * i]);
    PrintGrid();
    delay(1000);
  }
}

void ShortCountdown()
{
  for(int i = 3; i >= 0; i--)
  {
    LoadPmemPanel(&numTable[MP_ROWS * i]);
    PrintGrid();
    delay(1000);
  }
}
#undef COUNTDOWN_DELAY_MS


// Attempt to remove delay on the pattern functions
// Currently failed so commented out.
/*
void waitTime(unsigned long waitTime){
  unsigned long startTime = millis();            // get an updated time stamp
  do {
    // just wait around in a non-blocking fashion
  } while (millis() <= startTime + waitTime);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckerBoard (The Juggler)
//
// Display 2x2 pixel checkerboard pattern
  
void CheckerBoard(int Repeats, int FlashDelayMS)
{
  int i, j;
  
  for(i = 0; i < Repeats; i++)
  {
    for(j = 0; j < 2; j++)
    {
      SetRow(j,   B11001100);
      SetRow(j+2, B00110011);
      SetRow(j+4, B11001100);
      SetRow(j+6, B00110011);
    }
    PrintGrid();
    delay(FlashDelayMS);
    for(j = 0; j < 2; j++)
    {
      SetRow(j,   B00110011);
      SetRow(j+2, B11001100);
      SetRow(j+4, B00110011);
      SetRow(j+6, B11001100);
    }
    PrintGrid();
    delay(FlashDelayMS);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// RandomAlert (The Juggler)
//
// This more closely matches the flicker from the MarcDuino

void RandomAlert(int Repeats)
{
  for (int reps = 0; reps < Repeats; reps++)
  {
    allON();
    delay(random(5, 40));
    allOFF();
    delay(random(3, 25)); 
  }  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// CompressExplode (The Juggler)
//
// Compress, Explode and LineScan patterns are handled by this common routine.
//
// Compress/Explode demonstrates a quirk of the IA-Parts hw architecture where each chip row represents
// only half a physical row. 
//
//  Frame 1           Frame 2           Frame 3           Frame 4          etc... 
//  O O O O . . . .   O O O O O O O O   O O O O O O O O   O O O O O O O O
//  . . . . . . . .   . . . . . . . .   O O O O . . . .   O O O O O O O O
//  . . . . . . . .   . . . . . . . .   . . . . . . . .   . . . . . . . .
//  . . . . . . . .   . . . . . . . .   . . . . . . . .   . . . . . . . .
//  . . . . . . . .   . . . . . . . .   . . . . . . . .   . . . . . . . . 
//  . . . . . . . .   . . . . . . . .   . . . . . . . .   . . . . . . . .
//  . . . . . . . .   . . . . . . . .   . . . . O O O O   O O O O O O O O
//  . . . . O O O O   O O O O O O O O   O O O O O O O O   O O O O O O O O
//
// The original routines achieved this effect by addressing row by row to the chip instead of the VMagicPanel array.
// This consolidated routine emulates that pattern style when RetroMode flag is set to true.
// When RetroMode is false, the half steps are skipped, painting only complete rows, and the delay time is doubled
// so that the pattern takes the same amount of time in either Mode.
//
// Mode: CompressExplodeModeCompress - Draw from edges towards centre (default)
//       CompressExplodeModeExplode - Draw from centre out to edges
//
// FillOpt: OptFill - Do not follow the pattern erasing rows. (default)
//          OptNoFill - Erase rows in the same order as they were painted after all the rows have been drawn.
//
// RetroMode: true  - Paint rows half a row at a time (default)
//            false - Paint whole rows at a time
//
// Rotate: false - Paint rows (default)
//         true  - Paint cols instead of rows, patterns will be rotated 90 degrees.
//         

void CompressExplode(int Repeats, enum PatternCompressExplodeMode Mode, enum PatternCommonFillOpt FillOpt, boolean RetroMode, boolean Rotate)
{
#define COMPRESS_IN_DELAY_MS 100
  int i;
  int trow, brow, trow_start, brow_start;
  int trow_inc, brow_inc;
  byte row_half_t = B11110000;
  byte row_half_b = B00001111;
  byte row_full   = B11111111;
  void (*rowcolFn)(int, unsigned char);     // Fn ptr for the set row or col procedure
  int frame_delay_ms = COMPRESS_IN_DELAY_MS;
  
  if (!RetroMode) // New style with whole rows? Keep cumulative per row time equal.
    frame_delay_ms = COMPRESS_IN_DELAY_MS * 2;
  
  // Default is Compress
  trow_start = 0;
  brow_start = MP_ROWS - 1;
  trow_inc = 1;
  brow_inc = -1;

  // Default is Rotate == false (paint by row)
  rowcolFn = SetRow;
  if (Rotate)
    rowcolFn = SetCol;
  
  if (Mode == CompressExplodeModeExplode) // Explode
  {
    trow_start = (MP_ROWS/2) - 1;
    brow_start = MP_ROWS/2;
    trow_inc = -1;
    brow_inc = 1;
  }
  
  for (int reps = 0; reps < Repeats; reps++)
  {
    trow = trow_start;
    brow = brow_start;
    
    for (i = 0; i < MP_ROWS/2; i++)
    {
      if(RetroMode) // Retro style with half rows?
      {
        rowcolFn(trow, row_half_t);
        rowcolFn(brow, row_half_b);
        PrintGrid();
        delay(frame_delay_ms);
      }
      rowcolFn(trow, row_full);
      rowcolFn(brow, row_full);
      PrintGrid();
      delay(frame_delay_ms);
      trow += trow_inc;
      brow += brow_inc;
    }
    
    if (FillOpt == OptNoFill)
    {
      trow = trow_start;
      brow = brow_start;
      
      for (i = 0; i < MP_ROWS/2; i++)
      {
        if(RetroMode) // Retro style with half rows?
        {
          rowcolFn(trow, ~row_half_t);
          rowcolFn(brow, ~row_half_b);
          PrintGrid();
          delay(frame_delay_ms);
        }
        rowcolFn(trow, ~row_full);
        rowcolFn(brow, ~row_full);
        PrintGrid();
        delay(frame_delay_ms);

        trow += trow_inc;
        brow += brow_inc;
      }
    }
    allOFF();
  }
#undef COMPRESS_IN_DELAY_MS  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// VUMeter (The Juggler)
//
// Simulated VU meter
// Randomly selects an up or down shift in the bars of either 1 or 2 pixels
// on each update of the display.  4 Modes supported based on where the bars emanate from.

void VUMeter(int Repeats, enum PatternVUMode Mode)
{
  int i;
  int level[8] = {0,0,0,0,0,0,0,0};  

#define VUMETER_DELAY_MS 250
  
  // Set a default start level for each column.
  for (i = 0; i < 8; i++)
    level[i] = random(0,9);

  for (int reps = 0; reps < Repeats; reps++)
  {
    if (Mode == VUModeUp)
    {
      for(i = 0; i < MP_COLS; i++)
        SetCol(7-i, pgm_read_byte(&bargraph[level[i]]));
    }
    else if (Mode == VUModeLeft)
    {
      for(i = 0; i < MP_ROWS; i++)
        SetRow(7-i, pgm_read_byte(&bargraph[level[i]]));
    }
    else if (Mode == VUModeDown)
    {
      for(i = 0; i < MP_COLS; i++)
        SetCol(7-i, pgm_read_byte(&bargraph[level[i]+8]));
    }
    else if (Mode == VUModeRight)
    {
      for(i = 0; i < MP_ROWS; i++)
        SetRow(7-i, pgm_read_byte(&bargraph[level[i]+8]));
    }
    PrintGrid();

    delay(VUMETER_DELAY_MS);

    // calc the next position of the bars
    for (i = 0; i < 8; i++)
    {
      byte upDown = random(0,2);
      byte changeSize = random(1,3);

      if (upDown == 1)                         // go up
        ((level[i] + changeSize) <= 8) ? level[i] += changeSize : level[i] = 8;
      else                                     // go down
        ((level[i] - changeSize) >= 0) ? level[i] -= changeSize : level[i] = 0;
    }
  }
  
#undef VUMETER_DELAY_MS
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// RunPattern
//
// The following takes the Pattern code, and executes the relevant function
// This allows i2c and serial inputs to use the same function to start patterns
// so we avoid the need to duplicate this code.
void RunPattern(int pattern)
{
  if (lastMPeventCode != pattern)
  {
    lastMPeventCode = pattern;
    firstTimeCode = true;
  }
  else
  {
    firstTimeCode = false;
  }

  switch (pattern)
  {
    case 0:              //  0 = Turns Panel Off
    {    
      allOFF();
      break;
    }
    case 1:              //  1 = Turns Panel On Indefinately
    {
      allONTimed(0);
      break;
    }
    case 2:              //  2 = Turns Panel on for 2s
    {
      allONTimed(2000);
    }
    case 3:              //  3 = Turns Panel on for 5s
    {
      allONTimed(5000);
      break;
    }
    case 4:              //  4 = Turns Panel on for 10s
    {
      allONTimed(10000);
      break;
    }
    case 5:              //  5 = Begins Toggle Sequence: Top and Bottom Half of Panel Alternate
    {
      allOFF();
      alwaysOn ? Flash(1, 500, FlashModeUpDown): Flash(8, 500, FlashModeUpDown);
      turnOff();
      break;
    }
    case 6:              // 6 = Begins Alert Sequence (4s):  Panel Rapidly Flashes On & Off
    {
      allOFF();
      alwaysOn ? Flash(1, 250, FlashModeAll): Flash(8, 250, FlashModeAll);
      turnOff();
      break;
    } 
    case 7:              // 7 = Begins Alert Sequence (10s):  Panel Rapidly Flashes On & Off
    {
      allOFF();
      alwaysOn ? Flash(1, 250, FlashModeAll): Flash(20, 250, FlashModeAll);
      turnOff();
      break;
    }         
    case 8:              //  8 = Begins Trace Up Sequence (Filled):  Each row of the MP lights up from bottom to top filling entire panel
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeUp, OptFill): Trace(5, TraceModeUp, OptFill);
      turnOff();
      break;
    } 
    case 9:              //  9 = Begins Trace Up Sequence (No Fill):  Each row of the MP lights up from bottom to top individually
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeUp, OptNoFill): Trace(5, TraceModeUp, OptNoFill);
      turnOff();
      break;
    }        
    case 10:             //  10 = Begins Trace Down Sequence (Filled):  Each row of the MP lights up from top to bottom filling entire panel
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeDown, OptFill): Trace(5, TraceModeDown, OptFill);
      turnOff();
      break;
    }
    case 11:              //  11 = Begins Trace Down Sequence (No Fill):  Each row of the MP lights up from top to bottom individually
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeDown, OptNoFill): Trace(5, TraceModeDown, OptNoFill);
      turnOff();
      break;
    }
    case 12:             //  12 = Begins Trace Right Sequence (Filled):  Each column of the MP lights up from left to right filling entire panel
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeRight, OptFill): Trace(5, TraceModeRight, OptFill);
      turnOff();
      break;
    }
    case 13:              //  13 = Begins Trace Right Sequence (No Fill):  Each column of the MP lights up from left to right individually
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeRight, OptNoFill): Trace(5, TraceModeRight, OptNoFill);
      turnOff();
      break;
    }
    case 14:             //  14 = Begins Trace Left Sequence (Filled):  Each column of the MP lights up from right to left filling entire panel
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeLeft, OptFill): Trace(5, TraceModeLeft, OptFill);
      turnOff();
      break;
    }
    case 15:              //  15 = Begins Trace Left Sequence (No Fill):  Each column of the MP lights up from right to left individually
    {
      allOFF();
      alwaysOn ? Trace(1, TraceModeLeft, OptNoFill): Trace(5, TraceModeLeft, OptNoFill);
      turnOff();
      break;
    }
    case 16:              //  16 = Begins Expand Sequence (Filled): Panel expands from center filling entire panel 
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeExpand, OptFill): ExpandCompress(5, ExpandCompressModeExpand, OptFill);
      turnOff();  
      break;
    }
    case 17:               //  17 = Begins Expand Sequence (No Fill): Ring of pixels expands from center of panel
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeExpand, OptNoFill): ExpandCompress(5, ExpandCompressModeExpand, OptNoFill);
      turnOff();  
      break;
    }
    case 18:              //  18 = Begins Compress Sequence (Filled): Panel compresses from outer edge filling entire panel 
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeCompress, OptFill): ExpandCompress(5, ExpandCompressModeCompress, OptFill);
      turnOff();  
      break;
    }
    case 19:               //  19 = Begins Compress Sequence (No Fill): Ring of pixels compresses from outer edge of panel
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeCompress, OptNoFill): ExpandCompress(5, ExpandCompressModeCompress, OptNoFill);
      turnOff();  
      break;
    }
    case 20:              //  20 = Begins Cross Sequence: Panel is lit to display an X for 3s
    {
      if (firstTimeCode)
        allOFF();
      Cross();
      turnOff();
      break;
    }
    case 21:              //  21 = Begins Cylon Column Sequence: Each column illuminated one at a time from left to right back to left. (like the Cylons from Battlestar Galactica)
    {
      allOFF();
      alwaysOn ? Cylon(1, 140,CylonModeLeftRight): Cylon(2, 140,CylonModeLeftRight);
      turnOff();
      break;
    }
    case 22:              //  22 = Begins Cylon Row Sequence: Each row illuminated one at a time from top to bottom back to top. (like the Cylons from Battlestar Galactica)
    {
      allOFF();
      alwaysOn ? Cylon(1, 140,CylonModeUpDown): Cylon(2, 140,CylonModeUpDown);
      turnOff();
      break;
    }
    case 23:              // 23 = Begins Eye Scan Sequence:  Each row is illuminated from top to bottom followed by each column left to right. (like the eye ball scanners in the Mission Impossible movies)
    {
      alwaysOn ? EyeScan(1, 100): EyeScan(2, 100);
      turnOff(); 
      break;
    }    
    case 24:             //  24 = Begins Fade Out/In Sequence:  MP gradually and randomly fades out and then fades back in the same manner. 
    {
      FadeOutIn(FadeOutInModeOutIn);
      turnOff();
      break;
    }
    case 25:             //  25 = Begins Fade Out Sequence:  MP gradually and randomly fades out (Similar to the Short Circuit Sequence on Teeces). 
    {
      FadeOutIn(FadeOutInModeOut);
      turnOff();
      break;
    }           
    case 26:              //  26 = Begins Flash Sequence:  MP flashes rapidly for 5 seconds (Similar to Alarm Sequence)
    {
      alwaysOn ? Flash(1, 200, FlashModeAll): Flash(8, 200, FlashModeAll);
      turnOff();
      break;
    }
    case 27:              //  27 = Begins Flash V Sequence: Left and Right Half of Panel Alternate
    {
      alwaysOn ? Flash(1, 200, FlashModeLeftRight): Flash(8, 200, FlashModeLeftRight);
      turnOff();
      break;
    }
    case 28:              //  28 = Begins Flash Q Sequence:  Alternating quadrants of MP flash rapidly
    {
      alwaysOn ? Flash(1, 200, FlashModeQuadrant): Flash(8, 200, FlashModeQuadrant);
      turnOff();  
      break;
    }
    case 29:              //  29 = Begins Two Loop Sequence: Dual pixels are lit opposite each other completing a loop around the 2nd ring from panel edge.
    {
      if (firstTimeCode)
        allOFF();
      alwaysOn ? Looper(1,LoopModeTwo): Looper(2,LoopModeTwo);      
      turnOff();
      break;
    }
    case 30:              //  30 = Begins One Loop Sequence: A single pixel is lit individually completing a loop around the 2nd ring from panel edge.
    {
      if (firstTimeCode)
        allOFF();
      alwaysOn ? Looper(1,LoopModeOne): Looper(2,LoopModeOne);      
      turnOff();
      break;
    }
    case 31:              //  31 = Begins Test Sequence (Type 1):  Each pixel of the MP is lit sequentially from row 0, column 7 to row 7, column 0 until panel is filled, then unlit in the same order.
    {
      allOFF();
      TheTest(30);
      turnOff();
      break;
    }
    case 32:              //  32 = Begins Test Sequence (Type 2):  Each pixel of the MP is lit indivually from row 0, column 7 to row 7, column 0. 
    {
      allOFF();
      OneTest(30);
      turnOff();
      break;
    } 
    case 33:              //  33 = Begins AI Logo Sequence:  Displays the AI Aurebesh characters for 3s (...that we see all over our awesome packages from Rotopod and McWhlr) 
    {
      if (firstTimeCode)
        allOFF();
      Symbol();
      turnOff();
      break;
    } 
    case 34:              //  34 = Begins 2GWD Logo Sequence: Displays the Characters 2-G-W-D sequentially every 1s (...shameless, I know.) 
    {
      allOFF();
      MySymbol();
      turnOff();
      break;
    }
    case 35:              //  35 = Begins Quadrant Sequence (Type 1):  Each Panel Quadrant lights up individually (TL, TR, BR, BL) 
    {
      allOFF();
      alwaysOn ? Quadrant(1, QuadrantMode1): Quadrant(5, QuadrantMode1);
      turnOff();
      break;
    }
    case 36:              //  36 = Begins Quadrant Sequence (Type 2):  Each Panel Quadrant lights up individually (TR, TL, BL, BR) 
    {
      allOFF();
      alwaysOn ? Quadrant(1, QuadrantMode2): Quadrant(5, QuadrantMode2);
      turnOff();
      break;
    } 
    case 37:              //  37 = Begins Quadrant Sequence (Type 3):  Each Panel Quadrant lights up individually (TR, BR, BL, TL) 
    {
      allOFF();
      alwaysOn ? Quadrant(1, QuadrantMode3): Quadrant(5, QuadrantMode3);
      turnOff();
      break;
    } 
    case 38:              //  38 = Begins Quadrant Sequence (Type 4):  Each Panel Quadrant lights up individually (TL, BL, BR, TR) 
    {
      allOFF();
      alwaysOn ? Quadrant(1, QuadrantMode4): Quadrant(5, QuadrantMode4);
      turnOff();
      break;
    }  
    case 39:              //  39 = Begins Random Pixel Sequence:  Random pixels flash individually for 6s  
    {
      allOFF();
      RandomPixel(40);
      turnOff();
      break;
    }
    case 40:             //  40 = Begins countdown from 9 to 0 (takes 10 seconds)
    {
      Countdown();
      turnOff();
      break; 
    }
    case 41:             //  40 = Begins countdown from 3 to 0 (takes 4 seconds)
    {
      ShortCountdown();
      turnOff();
      break;
    }
    case 42:              // 42 = Begins Alert Sequence (4s):  Panel Randomly Flashes On & Off, mimics MarcDuino Code
    {
      allOFF();
      RandomAlert(20);
      turnOff();
      break;
    }
    case 43:              // 43 = Begins Alert Sequence (8s):  Panel Randomly Flashes On & Off, mimics MarcDuino Code
    {
      allOFF();
      RandomAlert(40);
      turnOff();
      break;
    }  
    case 44:              // 44 = Begins Smiley Face
    {
      if (firstTimeCode)
        allOFF();
      Smile();
      turnOff();
      break;
    }  
    case 45:              // 45 = Begins Sad Face
    {
      if (firstTimeCode)
        allOFF();
      Sad();
      turnOff();
      break;
    } 
    case 46:              // 46 = Begins Heart
    {
      if (firstTimeCode)
        allOFF();
      Heart();
      turnOff();
      break;
    }
    case 47:              // 47 = Begins Checkerboard flash
    {
      if (firstTimeCode)
        allOFF();
      alwaysOn ? CheckerBoard(1, 200): CheckerBoard(8, 200);
      turnOff();  
      break;
    }    
    case 48:              // 48 = Begins Compress In Type 1 - Fills the rows from TL/BR in half row increments 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeCompress, OptFill, true, false): CompressExplode(5, CompressExplodeModeCompress, OptFill, true, false);
      turnOff();  
      break;
    }   
    case 49:              // 49 = Begins Compress In Type 2 - Fills the rows from TL/BR in half row increments then clears in the same order 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeCompress, OptNoFill, true, false): CompressExplode(5, CompressExplodeModeCompress, OptNoFill, true, false);
      turnOff();  
      break;
    }  
    case 50:              // 50 = Begins Explode Out Type 1 - Fills the rows from Centre Out 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeExplode, OptFill, true, false): CompressExplode(5, CompressExplodeModeExplode, OptFill, true, false);
      turnOff();  
      break;
    }   
    case 51:              // 51 = Begins Explode Out Type 2 - Fills the rows from Centre Out then clears in the same order 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeExplode, OptNoFill, true, false): CompressExplode(5, CompressExplodeModeExplode, OptNoFill, true, false);
      turnOff();  
      break;
    }
    case 52:              // 52 = Begins VU Meter - Randomly ramp up/down on the columns to simulate a VU Meter. (Type 1)
    {
      allOFF();
      alwaysOn ? VUMeter(1, VUModeUp): VUMeter(15, VUModeUp);
      turnOff();  
      break;
    }
    case 53:              // 53 = Begins VU Meter - Randomly ramp up/down on the rows to simulate a VU Meter. (Type 2)
    {
      allOFF();
      alwaysOn ? VUMeter(1, VUModeRight): VUMeter(15, VUModeRight);
      turnOff();  
      break;
    }  
    case 54:              // 54 = Begins VU Meter - Randomly ramp up/down on the columns from the top to simulate a VU Meter. (Type 3)
    {
      allOFF();
      alwaysOn ? VUMeter(1, VUModeDown): VUMeter(15, VUModeDown);
      turnOff();  
      break;
    }
    case 55:              // 55 = Begins VU Meter - Randomly ramp up/down on the rows from the right to simulate a VU Meter. (Type 4)
    {
      allOFF();
      alwaysOn ? VUMeter(1, VUModeLeft): VUMeter(15, VUModeLeft);
      turnOff();  
      break;
    }          
    case 56:              // 56 = Begins Expand/Compress/Rebound Sequence (Filled): Panel expands from center filling entire panel 
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeRebound, OptFill): ExpandCompress(5, ExpandCompressModeRebound, OptFill);
      turnOff();  
      break;
    }
    case 57:              // 57 = Begins Expand/Compress/Rebound Sequence (NoFill): Panel expands from center filling entire panel 
    {
      allOFF();
      alwaysOn ? ExpandCompress(1, ExpandCompressModeRebound, OptFill): ExpandCompress(5, ExpandCompressModeRebound, OptFill);
      turnOff();  
      break;
    }
    case 58:              // 58 = Begins Compress In Type Fill, halfrows = false, rotated = true - Fills the rows from L/R in full row increments 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeCompress, OptFill, false, true): CompressExplode(5, CompressExplodeModeCompress, OptFill, false, true);
      turnOff();  
      break;
    }   
    case PATTERN_MAX:     // 59 = Begins Compress In Type NoFill, halfrows = false, rotated = true - Fills the rows from L/R in full row increments then clears in the same order 5 times
    {
      allOFF();
      alwaysOn ? CompressExplode(1, CompressExplodeModeCompress, OptNoFill, false, true): CompressExplode(5, CompressExplodeModeCompress, OptNoFill, false, true);
      turnOff();  
      break;
    }  
    default:
      // Do nothing
      break;    
  }
  
  LastPatternTimeMS = time_ms;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
// decodeJawaCommand
//
// Function to centralise decoding of Jawa Commands for both i2c and serial interfaces 
// The currently supported JawaLite commands are:                                      
// T - Set the mode (triggers a pattern)                                               
//

int decodeJawaCommand(char jawaCommand, int commandValue[])
{
  // In i2c we have already stripped the address from the start of the JawaLiteCommand
  // ie 20T0 is now just T0, so we can ignore the address parsing.  On Serial, we have no
  // need to send an address, so again let's just ignore it.

  // Check that the first Character we have received is a valid JawaLite command ie:
  // A,B,C,D,I,L,M,N,O,P,S,R,T,V,W,Z
  // And limit that to only the commands supported in the MP currently:
  // A,D,T

  String value_string = "";

  //char jawaCommand = commandReceived[0];
  //commandReceived.remove(0, 1);
  

  if (jawaCommand == 'A' ||
      jawaCommand == 'D' ||
      jawaCommand == 'T' ||
      jawaCommand == 'P')
      {
        #ifdef DEBUG
          Serial.print("Jawa Command Processing ="); Serial.println(jawaCommand); // debug
        #endif
        if (jawaCommand == 'T')
        {
          // This is kinda clunky and I should probably rewrite it, but for now it works
          // Since we support only up to 99 commands (as opposed to 999 in JawaLite)
          // We can just take the first byte after the command and convert it/
          //TODO:  Add some error checking in here!
          //String value = commandReceived.substring(0, 1);
          //commandReceived.copy(value, 0, 1);
          //return value.toInt();
          return commandValue[0];
        }
        else if (jawaCommand == 'A')
        {
          // Main mode of operation is to turn the panel on?
          // I have no idea, but it seems sensible enough!
          // Lets turn the panel on.
          allON();
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
        else if (jawaCommand == 'D')
        {
          // Go to Default mode.  So turn the panel off?
          // Seems sensible enough as a default mode!
          // Lets turn the panel off.
          allOFF();
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
        else if (jawaCommand == 'P')
        {
          // Set the Panel Mode.  
          // Note this isn't quite right, as there is only one parameter, so I ignore the parameter number.
          // Setting P0 will set the panel so that Patterns will turn off after the given time
          // Setting P1 will leave the patterns on until the T00 (off) command is received.
          if (commandValue[0] == 0)
          {
            alwaysOn = false;
            confirm(0);
          }
          if (commandValue[0] == 1)
          {
            alwaysOn = true;
            confirm(1);
          }
          // Return something to trigger the default (do nothing) in the pattern routine.
          return 1024;
        }
      }
      else
      {
        #ifdef DEBUG
          Serial.print("Unsupported Command ="); Serial.println(jawaCommand); // debug
        #endif
        return 0;
      }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// confirm
//
// Provides visual feedback displaying a numeral 1 or 0. Ie. A single bit console display!

void confirm(int param)
{
  allOFF();
  allON();
  delay(200);
  allOFF();
  delay(100);
  allON();
  delay(200);
  allOFF();
  if (param == 0)
  {
    LoadPmemPanel(&numTable[MP_ROWS * 0]);
    PrintGrid();
  }
  else if (param == 1)
  {
    LoadPmemPanel(&numTable[MP_ROWS * 1]);
    PrintGrid();
  }
  delay(2000);
  allOFF();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// receiveEvent
//
// function that executes whenever data is received from an I2C master
// this function is registered as an event, see setup()

void receiveEvent(int eventCode)
{
  char i2cCommand;
  int i2cValue[3] = {0,0,0};
  int i2cCommandLen;
  int event;
  
  // Check to see how much data is on the bus
  i2cCommandLen = Wire.available();

  // If the command is too big, return effectively ignoring it
  // 4 characters is the current max we support
  if (i2cCommandLen > 4)
  {
    #ifdef DEBUG
      Serial.print("Command too large. "); Serial.print(i2cCommandLen); Serial.println(" bytes.  Ignoring.");
    #endif
    for (int i = 0; i < i2cCommandLen; i++)
    {
      Wire.read();
    }
    return;
  }

  #ifdef DEBUG
    Serial.print("Available len? ="); Serial.println(i2cCommandLen);
  #endif

  for (int i = 0; i < i2cCommandLen; i++)
  {

    // First data should be the command type, it's a hex value / character
    if (i == 0)
    {
      i2cCommand = Wire.read();
    }
    // Everything else should be int if it's a good JawaLite Command
    else
    {
      i2cValue[i-1] = Wire.read();
      #ifdef DEBUG
        Serial.println(i2cValue[i-1]);
      #endif
    }

    // Don't seem to get the end character from MarcDuino, but for now leave this check as it is valid.
    if (i2cValue[i-1] == '\r')
    {
      break;
    }
    #ifdef DEBUG
      Serial.print("i2c loop ="); Serial.print(i2cCommand); Serial.print(i2cValue[0]); Serial.print(i2cValue[1]); Serial.println(i2cValue[2]); // debug
    #endif
  }

  #ifdef DEBUG
    Serial.print("Command ="); Serial.println(i2cCommand);
    Serial.print("Value ="); Serial.print(i2cValue[0]); Serial.print(i2cValue[1]); Serial.println(i2cValue[2]);
  #endif

  event = decodeJawaCommand(i2cCommand, i2cValue);

  RandomDurationMS = 0; // Cancel any running Random pattern
  RunPattern(event);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// serialEvent
//
// SerialEvent is invoked whenever a new data comes in the
// hardware serial RX.  This routine is run between each
// time loop() runs, so using delay inside loop can delay
// response.  Multiple bytes of data may be available.

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();                     // get the new byte

    if (inChar == '\r')
      stringComplete = true;
    else
      inputString += inChar;                                 // add it to the inputString

    if (stringComplete == true)
    {
#ifdef DEBUG
      Serial.print("In the eval... "); Serial.print(inputString);Serial.println('\r'); // debug
#endif

      // Get the length of the input string
      int len = inputString.length();

      char jawaCommand = inputString[0];
      inputString.remove(0,1);

      // Since everything on serial comes in one block, assume everything is a single uint_8.
      // This might be a bad assumption.
      int jawaValue[3] = {0,0,0};
      jawaValue[0] = inputString.toInt();

      int code = decodeJawaCommand(jawaCommand, jawaValue);

      stringEvaluated = true;

#ifdef DEBUG
      Serial.print("EventReceived ="); Serial.println(inputString); // debug
#endif
      if (stringEvaluated == true)
      {
        inputString ="";
        stringComplete = false;
        stringEvaluated = false;
#ifdef DEBUG
        Serial.println("inputString and String Complete reset");  //debug
        Serial.println(code);  //debug
#endif
        RandomDurationMS = 0; // Cancel any running Random pattern
        RunPattern(code);
      }
    }
  }
  
  sei(); // Why is there a lonely sei() here? Who called cli()?  (LostRebel)
}
