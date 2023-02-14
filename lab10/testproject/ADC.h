/*
 * ADC.h
 *
 * header for ADC
 * Created: 4/14/2021 10:43:59 AM
 *  Author: Tyler Ovenden 112122685
 */ 



#ifndef ADC_H_
#define ADC_H_

#define RES12_Bm 0x00
void update_lcd_dog(void);
void convert (void);
void displayVolt(uint16_t r);
uint32_t convertToBCD(uint16_t dec);



char tempHex[3];
volatile uint16_t result;
volatile uint8_t temp;
volatile int boolean;



#endif /* ADC_H_ */