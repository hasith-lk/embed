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
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LCD_C.elf

dependents:
	-cd I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release && $(MAKE) clean all

# Tool invocations
LCD_C.elf: $(OBJS) $(USER_OBJS) I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release/libmycustomboard_stdperiph_lib.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU G++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -L"I:\STM32\Workspace\mycustomboard_stdperiph_lib\Release" -specs=nano.specs -T"I:\STM32\Workspace\LCD_C\LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -lm -o "LCD_C.elf" @"objects.list" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "LCD_C.elf" "LCD_C.bin"
	arm-none-eabi-size -A "LCD_C.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
I:/STM32/Workspace/mycustomboard_stdperiph_lib/Release/libmycustomboard_stdperiph_lib.a:

-include ../makefile.targets
