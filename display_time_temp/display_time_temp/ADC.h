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
void init_adc(void);
void convert (void);


volatile uint16_t result;
volatile uint8_t temp;
volatile int boolean;



#endif /* ADC_H_ */