################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
marcduino-dome.lss \

FLASH_IMAGE += \
marcduino-dome.hex \

EEPROM_IMAGE += \
marcduino-dome.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: marcduino-dome.elf secondary-outputs

# Tool invocations
marcduino-dome.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,marcduino-dome.map -mmcu=atmega328p -o "marcduino-dome.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

marcduino-dome.lss: marcduino-dome.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S marcduino-dome.elf  >"marcduino-dome.lss"
	@echo 'Finished building: $@'
	@echo ' '

marcduino-dome.hex: marcduino-dome.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex marcduino-dome.elf  "marcduino-dome.hex"
	@echo 'Finished building: $@'
	@echo ' '

marcduino-dome.eep: marcduino-dome.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex marcduino-dome.elf  "marcduino-dome.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: marcduino-dome.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p marcduino-dome.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) marcduino-dome.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
