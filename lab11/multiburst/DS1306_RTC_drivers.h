/*
 * DS1306_RTC_drivers.h
 *
 * Created: 4/20/2021 6:23:52 PM
 *  Author: vleen
 */ 


#ifndef DS1306_RTC_DRIVERS_H_
#define DS1306_RTC_DRIVERS_H_


void SPI_rtc_ds1306_config (void);
void block_write_RTC (volatile unsigned char *array_ptr,
unsigned char strt_addr, unsigned char count);
void block_read_RTC (volatile unsigned char *array_ptr,
unsigned char strt_addr, unsigned char count);




#endif /* DS1306_RTC_DRIVERS_H_ */