//CoinSlot Interactive Animations
//2021-11 made by Michael Whitley for the Printed-Droid.com CSL

#include <FastLED.h>

#define BUTTON1_PIN 3
#define BUTTON2_PIN 4
#define BUTTON3_PIN 5
#define BUTTON4_PIN 6
#define BUTTON5_PIN 7
#define BUTTON6_PIN 8

//main
#define NUMLEDS 36
CRGB leds[NUMLEDS];
#define PIN 2
#define BRIGHTNESS 100

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;
bool wasLongKeyPress = false;

byte prevKeyState = HIGH;         // button is active low

#define BLUE_HUE 160 // Main FastLED Hue to use for animations when in single color mode. https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors
bool singleColorMode = true; // Use the BLUE_HUE color for animations. False = Multicolored
int demoLength = 10; // Number of seconds to display animations when in demo mode

int mode = 0; //mode=0 means animations, mode=1 means simon-game
int animationMode = 1;
int buttonList[] = {BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN, BUTTON5_PIN, BUTTON6_PIN};
CHSV blueColorHue(BLUE_HUE, 255, 255);
CRGB blueColor(blueColorHue);

#define RED_HUE 0
CHSV redColorHue(RED_HUE, 255, 255);
CRGB redColor(redColorHue);

//Effect-settings
#define BPM 60
uint8_t gHue = 0;

String inputString = "";
bool stringComplete = false;

void setup() {

  // Set up button pins
  pinMode(BUTTON1_PIN, INPUT); 
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  pinMode(BUTTON4_PIN, INPUT);
  pinMode(BUTTON5_PIN, INPUT);
  pinMode(BUTTON6_PIN, INPUT);

  digitalWrite(BUTTON1_PIN, HIGH);
  digitalWrite(BUTTON2_PIN, HIGH);
  digitalWrite(BUTTON3_PIN, HIGH);
  digitalWrite(BUTTON4_PIN, HIGH);
  digitalWrite(BUTTON5_PIN, HIGH);
  digitalWrite(BUTTON6_PIN, HIGH);

  // reserve 200 bytes for the inputString:
  Serial.begin(9600);

  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUMLEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

int effect = 0;

void loop() {
  //Serial Command arrived!
  if (stringComplete) {
    inputString.trim();
    
    if (inputString == "CS0"){
      effect = 0;
    }
    if (inputString == "CS1"){
      effect = 1;
    }
    if (inputString == "CS2"){
      effect = 2;
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  // key management section
  if (millis() - keyPrevMillis >= keySampleIntervalMs) {
      keyPrevMillis = millis();
      
      byte currKeyState = digitalRead(BUTTON6_PIN);
      
      if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
          keyPress();
      }
      else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
          keyRelease();
      }
      else if (currKeyState == LOW) {
          longKeyPressCount++;
      }
      
      prevKeyState = currKeyState;
  }

  if (effect == 1) {
      animate(); 
  } else if (effect == 2) {
      animateKnightRider(); 
  } else {
    fill_solid(leds, NUMLEDS, CRGB::Black);
    resetBrightness();
  }
  
  FastLED.show();
}

void animateKnightRider() {
  fill_solid(leds, NUMLEDS, CRGB::Black);

  resetBrightness();
  sinelon(singleColorMode, redColor);
}

void animate() {
  fill_solid(leds,NUMLEDS,CRGB::Black);

  checkMode();

  switch (animationMode)
  {
    case 0:
      resetBrightness();
      sinelon(singleColorMode, blueColor);
      break;
    case 1:
      resetBrightness(); // Only needed if using the Breathe animation someplace in the code
      beatit(singleColorMode);
      break;
    case 2:
      breathe(singleColorMode);
      break;
    case 3:
      resetBrightness(); // Only needed if using the Breathe animation someplace in the code
      juggle(singleColorMode);
      break;
    case 4:
      resetBrightness(); // Only needed if using the Breathe animation someplace in the code
      bpm(singleColorMode);
      break;
    default:
      // Off
      resetBrightness(); // Only needed if using the Breathe animation someplace in the code
      if(wasLongKeyPress)
      {
        singleColorMode = !singleColorMode;
        wasLongKeyPress = false;
      }
      break;
  } 
  
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
}

void checkMode()
{
  for( int i = 0; i < sizeof(buttonList)/sizeof(char *); i++) {
    if (digitalRead(buttonList[i]) == LOW)
    {
      animationMode = i;
    }
  }
}

void breathe(bool colorMode)
{
  // Be sure to use resetBrightness when exiting this animation!
  int hue = 0;
  int divisor = 30;
  float breath = (exp(sin(millis()/5000.0*PI)) - 0.36787944)*108.0;
  breath = map(breath, 0, 255, 3, BRIGHTNESS);
  FastLED.setBrightness(breath);
  
  if(colorMode)
  {
    fill_solid(leds,NUMLEDS,blueColor); // Use single color
  }
  else
  {
    fill_rainbow(leds, NUMLEDS, (hue++/divisor)); // Use multiple colors
  }
  
  if(hue == (255 * divisor)) {
   hue = 0;
  }
}

void rainbow()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  fill_rainbow( leds, NUMLEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUMLEDS) ] += CRGB::White;
  }
}

void confetti(bool colorMode) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUMLEDS, 10);
  int pos = random16(NUMLEDS);
  if(colorMode)
  {
    leds[pos] += blueColor; // Use single color
  }
  else
  {
    leds[pos] += CHSV( gHue + random8(64), 200, 255); // Use multiple colors
  }
}

void sinelon(bool colorMode, CRGB singleColor)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUMLEDS, 20);
  int pos = beatsin16( 13, 0, NUMLEDS-1 );
  if(colorMode)
  {
    leds[pos] += singleColor; // Use single color
  }
  else
  {
    leds[pos] += CHSV( gHue, 255, 192); // Use multiple colors
  }
}

void bpm(bool colorMode)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUMLEDS; i++) {
    if(colorMode)
    {
      leds[i] = CHSV(BLUE_HUE, 255, beat-gHue+(i*3)); // Use single color
    }
    else
    {
      leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*3)); // Use multiple colors
    }
  }
}

void juggle(bool colorMode) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUMLEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 7; i++) {
    
    if(colorMode)
    {
      leds[beatsin16( i+7, 0, NUMLEDS-1 )] |= blueColor; // Use single color
    }
    else
    {
      leds[beatsin16( i+7, 0, NUMLEDS-1 )] |= CHSV(dothue, 200, 255); // Use multiple colors
    }
    dothue += 32;
  }
}

void leftright(bool colorMode) {
  uint8_t beat = beatsin8(BPM,0,6);

  CRGBPalette16 palette = PartyColors_p;
  
  for(int i=0;i<6;i++){
    if (beat>0){
      int idx=beat-1;
      for(int x=0;x<6;x++){
        if(colorMode)
        {
          leds[idx] = blueColor; // Use single color
        }
        else
        {
          leds[idx] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10)); // Use multiple colors
        }
        idx+=6;
      }
    }
  }
}

void beatit(bool colorMode) {
  uint8_t beat = beatsin8(BPM,0,6);

  if ( beat >= 1){
    for(int i=30;i<=35;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }

  if (beat >= 2){
    for(int i=24;i<=29;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }
  
  if (beat >= 3){
    for(int i=18;i<=23;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }

  if (beat >= 4){
    for(int i=12;i<=17;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }

  if (beat >= 5){
    for(int i=6;i<=11;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }

  if (beat >= 6){
    for(int i=0;i<=5;i++){
      if(colorMode)
      {
        leds[i] = blueColor; // Use single color
      }
      else
      {
        leds[i] = CHSV( gHue, 255, 192); // Use multiple colors
      }
    }
  }
  
}

void resetBrightness()
{
  FastLED.setBrightness(BRIGHTNESS);
}

// Simon Game Functions Below

// called when button is kept pressed for less than 2 seconds
void shortKeyPress() {
    wasLongKeyPress = false;
}

// called when button is kept pressed for more than 2 seconds
void longKeyPress() {
    wasLongKeyPress = true;
}

// called when key goes from not pressed to pressed
void keyPress() {
    longKeyPressCount = 0;
}

// called when key goes from pressed to not pressed
void keyRelease() {
    if (longKeyPressCount >= longKeyPressCountMax) {
        longKeyPress();
    }
    else {
        shortKeyPress();
    }
}

void serialEvent() {
  while(!stringComplete && Serial.available()){
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
}
