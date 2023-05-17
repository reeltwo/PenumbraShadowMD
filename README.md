# Penumbra Shadow MD

This is a modified Shadow sketch to work with the Penumbra controller.

## Custom Sequences

The folder `marcduino-body` contains a customised version of the v3 MD firmware that has sequences for the body panels and tools. These have been ported from [Tim Hebel's SHADOW MD fork](https://github.com/Eebel/SHADOW_MD_EEBEL).

The `marcduino-dome` and `marcduino-slave` contain v3 MD firmware with additional sequences.

The PenumbraShadowMD sketch has also been updated to work with custom sketches for PrintedDroid DPL/CSL+/LDPL, Astropixels and the Lost Rebel MP..

Use the pin mapping below for wiring up your servos to your body MD:

1 - Data Panel Door
2 - Upper Utility Arm
3 - Lower Utility Arm
4 - Left Bread Pan Door
5 - Left Arm Raise/Lower (Gripper)
6 - Left Arm Tool (Gripper)
7 - Right Bread Pan Door
8 - Right Arm Raise/Lower (Interface Arm)
9 - Right Arm Tool (Interfface Arm)
10 - Charge Bay Door

Controller button mappings:

```
Control System:

PS + CROSS     (Disable Drive)
PS + CIRCLE    (Enable Drive)
L3 + L1        (Toggle Overspeed)
L2 + CROSS     (Disable Dome Automation)
L2 + CIRCLE    (Enable Dome Automation)

Drive Remote:

Arrow Up             (HP Random Move/On)
Arrow Down           (Quiet)
Arrow Left           (Mid Awake)
Arrow Right          (Holo Lights On/Off)

Arrow Up + CROSS     (Volume Up)
Arrow Down + CROSS   (Volume Down)
Arrow Keft + CROSS   (Previous Song)
Arrow Up + CROSS     (Next Song)

Arrow Up + CIRCLE    (Reset - Close Panels, Holos Reset, Logics Reset)

Dome Remote:

Arrow Up             (Scream, Wiggle Dome and Body)
Arrow Down           (Wave Bye, Sad Sound)
Arrow Left           (Disco)
Arrow Right          (Cantina)

Arrow Up + CIRCLE    (Leia)
Arrow Down + CIRCLE  (Knight Rider)
Arrow Left + CIRCLE  (Smirk)
Arrow Right + CIRCLE (Marching Ants)

Arrow Up + CROSS     (Utility Arms Open/Close)
Arrow Down + CROSS   (Data Port and Charge Bay Open/Close)
Arrow Left + CROSS   (Use Interface Arm)
Arrow Right + CROSS  (Use Gripper Arm)
```

## Building Instructions

Flashing MD firmware:

```
/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude -C  /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -p m328p -P usb -c usbtiny -U flash:w:./marcduino-dome.hex
```

Astropixels/Penumbra ShadowMD:

```
make clean
make build
make upload
```

Other sketches build + upload from Arduino IDE.

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
