/*
 * DS1306_RTCDrivers.h
 *
 * Created: 4/20/2021 6:44:01 PM
 *  Author: vleen
 */ 


#ifndef DS1306_RTCDRIVERS_H_
#define DS1306_RTCDRIVERS_H_
#define RTCRead 0x00
#define RTCWrite 0x80
#define CTRL_REG 0x8F
char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];


volatile unsigned char RTC_time_date_read[7];
volatile unsigned char *p[7];

void SPI_rtc_ds1306_config (void);
void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);

void block_write_RTC (volatile unsigned char *array_ptr, 
unsigned char strt_addr, unsigned char count);
void block_read_RTC(volatile unsigned char *array_ptr, 
unsigned char strt_addr, unsigned char count);

void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);


void write_read_RTC_test(void);

#endif /* DS1306_RTCDRIVERS_H_ */CDRIVERS_H_ */