/*
 * LCD_I2C.c
 *
 *  Created on: Apr 11, 2023
 *      Author: Home Admin
 */
#ifdef __cplusplus
 extern "C" {
#endif

#include "LCD_I2C.h"

 // flags for backlight control
 /*!
  @defined
  @abstract   LCD_NOBACKLIGHT
  @discussion NO BACKLIGHT MASK
  */
 #define LCD_NOBACKLIGHT 0x00

 /*!
  @defined
  @abstract   LCD_BACKLIGHT
  @discussion BACKLIGHT MASK used when backlight is on
  */
 #define LCD_BACKLIGHT   0xFF


 // Default library configuration parameters used by class constructor with
 // only the I2C address field.
 // ---------------------------------------------------------------------------
 /*!
  @defined
  @abstract   Enable bit of the LCD
  @discussion Defines the IO of the expander connected to the LCD Enable
  */
 #define EN 2  // Enable bit

 /*!
  @defined
  @abstract   Read/Write bit of the LCD
  @discussion Defines the IO of the expander connected to the LCD Rw pin
  */
 #define RW 1  // Read/Write bit

 /*!
  @defined
  @abstract   Register bit of the LCD
  @discussion Defines the IO of the expander connected to the LCD Register select pin
  */
 #define RS 0  // Register select bit

 /*!
  @defined
  @abstract   LCD dataline allocation this library only supports 4 bit LCD control
  mode.
  @discussion D4, D5, D6, D7 LCD data lines pin mapping of the extender module
  */
 #define D4 4
 #define D5 5
 #define D6 6
 #define D7 7

 void LCD_I2C_PulseEnable(LCD_I2C_Struct *LCD_I2C, uint8_t data)
 {
    i2c1write (LCD_I2C->_Addr, data | LCD_I2C->_En);   // En HIGH
    i2c1write (LCD_I2C->_Addr, data & ~LCD_I2C->_En);  // En LOW
 }

 void LCD_I2C_write4bits(LCD_I2C_Struct *LCD_I2C, uint8_t value, uint8_t mode )
 {

    uint8_t pinMapValue = 0;

    // Map the value to LCD pin mapping
    // --------------------------------
    uint8_t i = 0;
    for (; i < 4; i++ )
    {
       if ( ( value & 0x1 ) == 1 )
       {
          pinMapValue |= LCD_I2C->_data_pins[i];
       }
       value = ( value >> 1 );
    }

    // Is it a command or data
    // -----------------------
    if ( mode == DATA )
    {
       mode = LCD_I2C->_Rs;
    }

    pinMapValue |= mode | LCD_I2C->_backlightStsMask;
    LCD_I2C_PulseEnable (LCD_I2C, pinMapValue );
 }

 void LCD_I2C_begin(LCD_I2C_Struct *LCD_I2C, uint8_t cols, uint8_t rows)
 {
    LCD_I2C_init(LCD_I2C);    // Initialise the I2C expander interface
    LCD_I2C->base.begin(((LCD_Struct *)LCD_I2C), cols, rows, LCD_5x8DOTS);
 }

 void LCD_I2C_send(LCD_I2C_Struct *LCD_I2C, uint8_t value, uint8_t mode)
 {
    // No need to use the delay routines since the time taken to write takes
    // longer that what is needed both for toggling and enable pin an to execute
    // the command.

    if ( mode == FOUR_BITS )
    {
       LCD_I2C_write4bits(LCD_I2C, (value & 0x0F), COMMAND );
    }
    else
    {
       LCD_I2C_write4bits(LCD_I2C, (value >> 4), mode );
       LCD_I2C_write4bits(LCD_I2C, (value & 0x0F), mode);
    }
 }

 void LCD_I2C_setBacklight (LCD_I2C_Struct *LCD_I2C, uint8_t value )
 {
    // Check if backlight is available
    // ----------------------------------------------------
    if ( LCD_I2C->_backlightPinMask != 0x0 )
    {
       // Check for polarity to configure mask accordingly
       // ----------------------------------------------------------
       if  (((LCD_I2C->base._polarity == POSITIVE) && (value > 0)) ||
            ((LCD_I2C->base._polarity == NEGATIVE ) && ( value == 0 )))
       {
          LCD_I2C->_backlightStsMask = LCD_I2C->_backlightPinMask & LCD_BACKLIGHT;
       }
       else
       {
          LCD_I2C->_backlightStsMask = LCD_I2C->_backlightPinMask & LCD_NOBACKLIGHT;
       }
       LCD_I2C->base.write((LCD_Struct *)LCD_I2C, LCD_I2C->_backlightStsMask);
    }
 }

 void LCD_I2C_setBacklightPin (LCD_I2C_Struct *LCD_I2C, uint8_t value, t_backlighPol pol )
 {
    LCD_I2C->_backlightPinMask = ( 1 << value );
    LCD_I2C->base._polarity = pol;
    //LCD_I2C_setBacklight(LCD_I2C, BACKLIGHT_OFF);
 }

 void LCD_I2C_config (LCD_I2C_Struct *LCD_I2C, uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs,
                                 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
 {
    LCD_I2C->_Addr = lcd_Addr;

    LCD_I2C->_backlightPinMask = 0;
    LCD_I2C->_backlightStsMask = LCD_NOBACKLIGHT;
    LCD_I2C->base._polarity = POSITIVE;

    LCD_I2C->_En = ( 1 << En );
    LCD_I2C->_Rw = ( 1 << Rw );
    LCD_I2C->_Rs = ( 1 << Rs );

    // Initialise pin mapping
    LCD_I2C->_data_pins[0] = ( 1 << d4 );
    LCD_I2C->_data_pins[1] = ( 1 << d5 );
    LCD_I2C->_data_pins[2] = ( 1 << d6 );
    LCD_I2C->_data_pins[3] = ( 1 << d7 );
 }

 LCD_I2C_Struct *new_LCD_I2C_Struct(uint8_t lcd_Addr, uint8_t backlighPin, t_backlighPol pol)
 {
    //LCD_Struct *LCD = new_LCD_Struct();
    LCD_I2C_Struct *LCD_I2C = (LCD_I2C_Struct *)malloc(sizeof(LCD_I2C_Struct));
    base_LCD_Struct((LCD_Struct *)LCD_I2C);
    //LCD_I2C->base = LCD;
    ((LCD_Struct *)LCD_I2C)->send = &LCD_I2C_send;
    ((LCD_Struct *)LCD_I2C)->setBacklight = &LCD_I2C_setBacklight;
    ((LCD_Struct *)LCD_I2C)->setBacklightPin = &LCD_I2C_setBacklightPin;

    LCD_I2C_config(LCD_I2C, lcd_Addr, EN, RW, RS, D4, D5, D6, D7);
    LCD_I2C_setBacklightPin(LCD_I2C, backlighPin, pol);

    return LCD_I2C;
 }

int LCD_I2C_init(LCD_I2C_Struct *LCD_I2C)
{
   int status = 0;

   // initialize the backpack IO expander
   // and display functions.
   // ------------------------------------------------------------------------
   if ( i2c1begin( LCD_I2C->_Addr ) == 1 )
   {
      // _i2cio.portMode ( OUTPUT );  // Set the entire IO extender to OUTPUT
      // I2C_LowLevel_Init(I2C1, 100000, 0x00);
      LCD_I2C->base._displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
      status = 1;
      //_i2cio.write(0);  // Set the entire port to LOW
      i2c1write(LCD_I2C->_Addr, 0);
   }
   return ( status );
}

void LCD_I2C_clear(LCD_I2C_Struct *LCD_I2C)
{
   LCD_clear((LCD_Struct *)LCD_I2C);
}

size_t LCD_I2C_print(LCD_I2C_Struct *LCD_I2C, const char c)
{
   return LCD_print((LCD_Struct *)LCD_I2C, c);
}

size_t LCD_I2C_printStr(LCD_I2C_Struct *LCD_I2C, const char *str)
{
   return LCD_printStr((LCD_Struct *)LCD_I2C, (const char *)str);
}

#ifdef __cplusplus
}
#endif

