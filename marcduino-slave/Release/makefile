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
marcduino-slave.lss \

FLASH_IMAGE += \
marcduino-slave.hex \

EEPROM_IMAGE += \
marcduino-slave.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: marcduino-slave.elf secondary-outputs

# Tool invocations
marcduino-slave.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,marcduino-slave.map -mmcu=atmega328p -o "marcduino-slave.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

marcduino-slave.lss: marcduino-slave.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S marcduino-slave.elf  >"marcduino-slave.lss"
	@echo 'Finished building: $@'
	@echo ' '

marcduino-slave.hex: marcduino-slave.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex marcduino-slave.elf  "marcduino-slave.hex"
	@echo 'Finished building: $@'
	@echo ' '

marcduino-slave.eep: marcduino-slave.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex marcduino-slave.elf  "marcduino-slave.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: marcduino-slave.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p marcduino-slave.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) marcduino-slave.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
