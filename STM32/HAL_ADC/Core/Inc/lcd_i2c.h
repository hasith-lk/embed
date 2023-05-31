/*
 * LCD_I2C.h
 *
 *  Created on: Apr 11, 2023
 *      Author: Home Admin
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include "LCD.h"
#include "i2c.h"

typedef struct {
   LCD_Struct base;
   uint8_t _Addr;             // I2C Address of the IO expander
   uint8_t _backlightPinMask; // Backlight IO pin mask
   uint8_t _backlightStsMask; // Backlight status mask
   //I2CIO   _i2cio;            // I2CIO PCF8574* expansion module driver I2CLCDextraIO
   uint8_t _En;               // LCD expander word for enable pin
   uint8_t _Rw;               // LCD expander word for R/W pin
   uint8_t _Rs;               // LCD expander word for Register Select pin
   uint8_t _data_pins[4];     // LCD data lines

   // virtual functions
   /*void (*send)(struct LCD_I2C_Struct *, uint8_t value, uint8_t mode);
   void (*write)(struct LCD_I2C_Struct *, uint8_t value);
   void (*setBacklight)(uint8_t value);
   void (*setBacklightPin)(uint8_t value, t_backlighPol pol);*/
} LCD_I2C_Struct;

//LCD_I2C_Struct *LCD_I2C;

LCD_I2C_Struct *new_LCD_I2C_Struct(uint8_t lcd_Addr, uint8_t backlighPin, t_backlighPol pol);

// Constructor with backlight control
 /*void LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t backlighPin, t_backlighPol pol);*/

/*!
 @method
 @abstract   Initializes the LCD class
 @discussion Initializes the LCD class and IO expansion module.
 */
int LCD_I2C_init(LCD_I2C_Struct *);

/*!
 @function
 @abstract   Initialises class private variables
 @discussion This is the class single point for initialising private variables.

 @param      lcd_Addr[in] I2C address of the IO expansion module. For I2CLCDextraIO,
 the address can be configured using the on board jumpers.
 @param      En[in] LCD En (Enable) pin connected to the IO extender module
 @param      Rw[in] LCD Rw (Read/write) pin connected to the IO extender module
 @param      Rs[in] LCD Rs (Reset) pin connected to the IO extender module
 @param      d4[in] LCD data 0 pin map on IO extender module
 @param      d5[in] LCD data 1 pin map on IO extender module
 @param      d6[in] LCD data 2 pin map on IO extender module
 @param      d7[in] LCD data 3 pin map on IO extender module
 */
void LCD_I2C_Config (LCD_I2C_Struct *, uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );

/*!
 @method
 @abstract   Writes an 4 bit value to the LCD.
 @discussion Writes 4 bits (the least significant) to the LCD control data lines.
 @param      value[in] Value to write to the LCD
 @param      more[in]  Value to distinguish between command and data.
 COMMAND == command, DATA == data.
 */
void LCD_I2C_Write4bits(LCD_I2C_Struct *, uint8_t value, uint8_t mode);

/*!
 @method
 @abstract   Pulse the LCD enable line (En).
 @discussion Sends a pulse of 1 uS to the Enable pin to execute an command
 or write operation.
 */
void LCD_I2C_PulseEnable(LCD_I2C_Struct *, uint8_t);
void LCD_I2C_begin(LCD_I2C_Struct *, uint8_t cols, uint8_t rows);
void LCD_I2C_send(LCD_I2C_Struct *, uint8_t value, uint8_t mode);
void LCD_I2C_setBacklight (LCD_I2C_Struct *, uint8_t value );
void LCD_I2C_setBacklightPin (LCD_I2C_Struct *, uint8_t value, t_backlighPol pol );
void LCD_I2C_clear(LCD_I2C_Struct *);
size_t LCD_I2C_print(LCD_I2C_Struct *, const char c);
size_t LCD_I2C_printStr(LCD_I2C_Struct *, const char *str);
void LCD_I2C_setCursor(LCD_I2C_Struct *LCD, uint8_t col, uint8_t row);

#endif /* INC_LCD_I2C_H_ */
