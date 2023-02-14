/*
 * DOG_LCD.h
 *
 * header for DOG_LCD
 * Created: 4/14/2021 10:43:59 AM
 *  Author:  Tyler Ovenden 112122685
 */ 


#ifndef DOG_LCD_H_
#define DOG_LCD_H_

void lcd_spi_transmit_CMD (unsigned char cmd);
void lcd_spi_transmit_DATA (unsigned char cmd);
void init_spi_lcd (void);
void init_lcd_dog (void);
void update_lcd_dog(void);

char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];



#endif /* DOG_LCD_H_ */