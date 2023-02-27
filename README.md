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

TODO

## Serial Configuration commands

`#SMZERO`:
Clear stored preferences in flash/EEPROM.
*Examples*:

    #SMZERO

`#SMRESTART`:
Reboot the controller.
*Examples*:

    #SMRESTART
