################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f10x_it.c \
../src/system_stm32f10x.c \
../src/utility.c 

OBJS += \
./src/main.o \
./src/stm32f10x_it.o \
./src/system_stm32f10x.o \
./src/utility.o 

C_DEPS += \
./src/main.d \
./src/stm32f10x_it.d \
./src/system_stm32f10x.d \
./src/utility.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib" -I"I:/STM32/Workspace/GPIO_TEST1/inc" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/core" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/device" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


