################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DS3231RTC.c \
../src/delay.c \
../src/i2c.c \
../src/serialPrint.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

CPP_SRCS += \
../src/LCD.cpp \
../src/LiquidCrystal_I2C.cpp \
../src/main.cpp 

OBJS += \
./src/DS3231RTC.o \
./src/LCD.o \
./src/LiquidCrystal_I2C.o \
./src/delay.o \
./src/i2c.o \
./src/main.o \
./src/serialPrint.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/DS3231RTC.d \
./src/delay.d \
./src/i2c.d \
./src/serialPrint.d \
./src/syscalls.d \
./src/system_stm32f10x.d 

CPP_DEPS += \
./src/LCD.d \
./src/LiquidCrystal_I2C.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib" -I"I:/STM32/Workspace/I2C_LCD/inc" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/core" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/device" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib" -I"I:/STM32/Workspace/I2C_LCD/inc" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/core" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/device" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -mslow-flash-data -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


