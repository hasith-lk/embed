# Embedded Coding repo
Embedded coding using STM32 F103 Microprocessor and STM8 S103F3 Microprocessor

This repo contains the embedded codes written for STM32 MCPU, STM8 MCPU and for Arduino boards.
Some simple stuff that will be used to perform IO operations using microcontroller unit.

STM32 codes are compiled for ARM Cortex M3 architecture but can used with other Cortex M architectures. However codes are using STM32 libraries so can ported only inbetween STM32 microprocessors.

STM8 codes are compiled using SDCC compiler for STM8. Used platformIo for coding and and for debugging. Code shold be able to use out of the box. Due to the flash capacity limit, Standrd Peripharal Library (SPL) is splitted into multiple files and include only when necessary.
