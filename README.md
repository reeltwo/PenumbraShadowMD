# Penumbra Shadow MD

This is a modified Shadow sketch to work with the Penumbra controller.

## Libraries Used

<ul>
<li>https://github.com/reeltwo/Reeltwo</li>
<li>https://github.com/rimim/espsoftwareserial</li>
<li>https://github.com/reeltwo/USB_Host_Shield_2.0</li>
<li>https://github.com/DFRobot/DFRobotDFPlayerMini</li>
</ul>

## Web Installer

A web-based installer for the latest firmware can be found here: https://reeltwo.github.io/Penumbra-Installer/

## Build using Arduino IDE

The firmware should build with esp32 board package 1.0.6 or 2.0.5. For ESP32S2 support 2.0.5 is required.

## Build using command line tools (Mac)

    cd ~/Documents/Arduino
    curl -o Arduino.mk https://raw.githubusercontent.com/reeltwo/Arduino/master/Arduino.mk
    # Fetch repository
    git clone https://github.com/reeltwo/PenumbraShadowMD
    # Enter directory
    cd PenumbraShadowMD
    # Build firmware
    make


## Build using command line tools (Linux)

    cd ~/Arduino
    wget https://raw.githubusercontent.com/reeltwo/Arduino/master/Arduino.mk
    # Fetch repository
    git clone https://github.com/reeltwo/PenumbraShadowMD
    # Enter directory
    cd PenumbraShadowMD
    # Build firmware
    make

## Sample wiring diagram for Penumbra Shadow

![PenumbraShadow](https://user-images.githubusercontent.com/16616950/222179232-cd7f6191-de23-43d3-b792-a73715196444.png)

## Command Reference

### #SMRESTART
Restart the controller.
```
#SMRESTART
````
### #SMZERO
Delete all preferences, reset button triggers to sketch defaults, unpair controllers.
```
#SMZERO
````
### #SMLIST
List all button triggers and associated commands.
```
#SMLIST
```
### #SMVOLUME<0-1000>
Set the volume to the specified value.
```
#SMVOLUME500
````
### #SMSTARTUP _file number_
Set the startup sound to the specified file number.
```
#SMSTARTUP255
````
### #SMRANDMIN _milliseconds_
Set the minimum delay before playing the next random sound.
```
#SMRANDMIN600
````
### #SMRANDMAX _milliseconds_
Set the maximum delay before playing the next random sound.
```
#SMRANDMAX10000
````
### #SMRAND0
Disable random sound playback.
```
#SMRAND0
````
### #SMRAND1
Enable random sound playback.
```
#SMRAND1
````
### #SMSET _trigger_ _action_
Set the button trigger (_trigger_) to the specified action (_action_).
```
#SMSET btnUP_MD $83

To send direct Marcduino commands prefix the command with " and separate with ,
#SMSET btnUP_MD "BM:OP03,"BM*ON01
#SMSET btnUP_CIRCLE_MD ":OP03,"BM*ON01

" followed by BM is sent to body Marcduino
````
### #SMPLAY _trigger_
Play any action associated with the specified trigger
```
#SMPLAY btnUP_MD
````
### #SMDEL _trigger_
Reset command for specified trigger to sketch default.
```
#SMDEL btnUP_MD
````
### #SMSOUND [0,1,2,3]
Set the sound output module to of the following:
* 0: Disabled
* 1: MP3 Trigger
* 2: DFMini Player
* 3: HCR Vocalizer
```
#SMSOUND 3
````
### #SMCONFIG
Display the current drive configuration
```
#SMCONFIG
```
### #SMNORMALSPEED[0..127]
Set the normal drive speed: set this to whatever speeds works for you. 0-stop, 127-full speed. Default is 70.
```
#SMNORMALSPEED70
```
### #SMMAXSPEED[0..127]
Set the over throttle drive speed: set this to whatever speeds works for you. 0-stop, 127-full speed. Default is 100.
```
#SMMAXSPEED100
```
### #SMTURNSPEED[0..127]
Set the drive turn speed. The higher this number the faster it will spin in place, lower - the easier to control.
Recommend beginner: 40 to 50, experienced: 50+, I like 75. Default is 50.
```
#SMTURNSPEED50
```
### #SMDOMESPEED[0..127]
Set the dome drive top speed. Default is 100.
```
#SMDOMESPEED100
```
### #SMRAMPING[0..10]
Set drive speed ramping. The lower this number the longer it will take to speedup or slow down. Default is 1.
```
#SMRAMPING1
```
### #SMFOOTDB[0..127]
Set the foot controller joystick deadband. Use the lowest number with no drift. Default is 15
```
#SMFOOTDB15
```
### #SMDOMEDB[0..127]
Set the dome controller joystick deadband. Use the lowest number with no drift. Default is 10
```
#SMDOMEDB15
```
### #SMDRIVEDB[0..127]
Set the drive motor controller deadband. Use the lowest number with no drift. Default is 10
```
#SMDRIVEDB10
```
### #SMINVERT[0..1]
Set the turn direction inversion. Default is 0.
```
#SMINVERT0
```
### #SMAUTOSPEED[50..100]
Set the dome speed used when dome automation is active. Default is 70.
```
#SMAUTOSPEED70
```
### #SMAUTOTIME[2500..8000]
Set the number of milliseconds for dome to complete 360 turn at #SMAUTOSPEED. Default is 2500.
```
#SMAUTOTIME2500
```
### #SMMOTORBAUD[2400..115200]
Sets the baud rate of the packet serial connection to the motor controller. Default is 9600.
```
#SMMOTORBAUD9600
```
### #SMMARCBAUD[2400..115200]
Sets the baud rate of the command seriall connection. Default is 9600.
```
#SMMARCBAUD9600
```