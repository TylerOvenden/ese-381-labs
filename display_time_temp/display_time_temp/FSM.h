/*
 * FSM.h
 * header for FSM, used in FSM & FSM_Methods
 * Created: 4/27/2021 12:23:42 PM
 *  Author: Tyler Ovenden 112122685
 */ 


#ifndef FSM_H_
#define FSM_H_

#define READ_HOUR 0x00
#define READ_MIN 0x10
#define READ_SEC 0x20
#define WRITE_HOUR 0x80
#define WRITE_MIN 0x80
#define WRITE_SEC 0x80



int vals[2];
int current;
int digit_val;
typedef enum {idle, set, hours, mins, secs} state;
typedef enum {s, h, m, sec, digit, enter, eol} key;
state present_state;
void fsm(state ps, key kevval);
typedef void (* task_fn_ptr) ();

volatile unsigned char read;

typedef struct {
	key keyval;
	state next_state;
	task_fn_ptr tf_ptr;
	}transition;
	



	void set_fn();
const transition idle_transitions[2];


	
	void error_fn();
	void set_hours_fn();
	void set_mins_fn();
	
	void set_sec_fn();
	void digit_in_fn();
	void enter_hour_fn();
	void enter_min_fn();
	void enter_sec_fn();


#endif /* FSM_H_ */