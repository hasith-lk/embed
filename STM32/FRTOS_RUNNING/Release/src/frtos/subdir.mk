################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/frtos/croutine.c \
../src/frtos/event_groups.c \
../src/frtos/heap_1.c \
../src/frtos/list.c \
../src/frtos/port.c \
../src/frtos/queue.c \
../src/frtos/tasks.c \
../src/frtos/timers.c 

OBJS += \
./src/frtos/croutine.o \
./src/frtos/event_groups.o \
./src/frtos/heap_1.o \
./src/frtos/list.o \
./src/frtos/port.o \
./src/frtos/queue.o \
./src/frtos/tasks.o \
./src/frtos/timers.o 

C_DEPS += \
./src/frtos/croutine.d \
./src/frtos/event_groups.d \
./src/frtos/heap_1.d \
./src/frtos/list.d \
./src/frtos/port.d \
./src/frtos/queue.d \
./src/frtos/tasks.d \
./src/frtos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/frtos/%.o: ../src/frtos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DSTM32F103C8Tx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"I:/STM32/Workspace/stm32nanoboard_stdperiph_lib" -I"I:/STM32/Workspace/FRTOS_RUNNING/inc" -I"I:/STM32/Workspace/FRTOS_RUNNING/inc/frtos" -I"I:/STM32/Workspace/stm32nanoboard_stdperiph_lib/CMSIS/core" -I"I:/STM32/Workspace/stm32nanoboard_stdperiph_lib/CMSIS/device" -I"I:/STM32/Workspace/stm32nanoboard_stdperiph_lib/StdPeriph_Driver/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


