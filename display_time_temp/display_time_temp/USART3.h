/*
 * USART3.h
 *
 * Created: 4/27/2021 5:52:37 PM
 *  Author: vleen
 */ 


#ifndef USART3_H_
#define USART3_H_
#define BAU ((4)*(4)*1000000)/(9600)

void init_USART();

void transmit_USART(int data);

#endif /* USART3_H_ */


