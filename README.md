# Penumbra Shadow MD

This is a modified Shadow sketch to work with the Penumbra controller.

## Libraries Used

<ul>
<li>https://github.com/reeltwo/Reeltwo</li>
<li>https://github.com/rimim/espsoftwareserial</li>
<li>https://github.com/reeltwo/USB_Host_Shield_2.0</li>
</ul>

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
