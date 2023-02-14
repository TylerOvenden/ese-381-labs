/*
 * DS1306_driver.h
 *
 * Created: 4/20/2021 12:14:28 PM
 *  Author: vleen
 */ 


#ifndef DS1306_DRIVER_H_
#define DS1306_DRIVER_H_
#define CTRL_REG 0x8F
unsigned char RTC_byte[10];

void SPI_rtc_ds1306_config (void);
void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);


void write_read_RTC_test(void);


#endif /* DS1306_DRIVER_H_ */