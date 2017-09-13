################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ADC_LCD.elf

dependents:
	-cd I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release && $(MAKE) all

# Tool invocations
ADC_LCD.elf: $(OBJS) $(USER_OBJS) I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release/libmycustomboard_stdperiph_lib.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -L"I:\STM32\Workspace\mycustomboard_stdperiph_lib\Release" -lc -specs=nano.specs -T"I:\STM32\Workspace\ADC_LCD\LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -lm -o "ADC_LCD.elf" @"objects.list" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "ADC_LCD.elf" "ADC_LCD.bin"
	arm-none-eabi-size -B "ADC_LCD.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release/libmycustomboard_stdperiph_lib.a:

-include ../makefile.targets
