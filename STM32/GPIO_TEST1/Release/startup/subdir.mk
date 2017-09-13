################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib" -I"I:/STM32/Workspace/GPIO_TEST1/inc" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/core" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/CMSIS/device" -I"I:/STM32/Workspace/mycustomboard_stdperiph_lib/StdPeriph_Driver/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


