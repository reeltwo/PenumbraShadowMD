################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Print.c \
../fifo.c \
../i2c.c \
../main.c \
../realtime.c \
../sequencer.c \
../serial.c \
../servo.c \
../suart.c \
../wmath.c 

OBJS += \
./Print.o \
./fifo.o \
./i2c.o \
./main.o \
./realtime.o \
./sequencer.o \
./serial.o \
./servo.o \
./suart.o \
./wmath.o 

C_DEPS += \
./Print.d \
./fifo.d \
./i2c.d \
./main.d \
./realtime.d \
./sequencer.d \
./serial.d \
./servo.d \
./suart.d \
./wmath.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


