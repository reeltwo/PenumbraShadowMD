TARGET?=ESP32
PORT?=/dev/ttyUSB0
#ESP32_FILESYSTEM=littlefs
#ESP32_PSRAM=enabled
ESP32_FILESYSTEM=spiffs
ESP32_FILESYSTEM_PART=spiffs
ESP32_PARTSCHEME=min_spiffs
ESP32_FLASHSIZE=16MB
GITHUB_REPOS= \
reeltwo/Reeltwo \
rimim/espsoftwareserial \
felis/USB_Host_Shield_2.0

include ../Arduino.mk

POD_START:=$(shell grep 0xBA $(ESP32_PARTFILE) | awk -F',' '{print $$4}' | xargs printf "%d")

upload_pod:
	$(ESP32_UPLOAD) --chip $(UPLOAD_DEVICE) --port $(PORT) --baud $(BAUDRATE) $(ESP32_UPLOAD_OPTIONS) $(POD_START) persist/warbler.pod
