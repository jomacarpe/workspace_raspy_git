/*
 * timer.c
 *
 *  Created on: 3 de may. de 2016
 *      Author: miguelsanchez
 */

#include "timer.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>
tmr_t*
tmr_new (notify_func_t isr)
{
	tmr_t* this = (tmr_t*) malloc (sizeof (tmr_t));
	tmr_init (this, isr);
	return this;
}
void tmr_init (tmr_t* this, notify_func_t isr) {
	this->se.sigev_notify = SIGEV_THREAD;
	this->se.sigev_value.sival_ptr = &(this->timerid);
	this->se.sigev_notify_function = isr;
	this->se.sigev_notify_attributes = NULL;
	timer_create (CLOCK_REALTIME, &(this->se), &(this->timerid));
}
void tmr_destroy(tmr_t* this)
{
    tmr_stop (this);
    free(this);
}
int readTime(tmr_t* this,int TIMEOUT){
	int a=0;
 	timer_gettime (this->timerid, &(this->spec));
 	a= (this->spec.it_value.tv_sec*1000) +
 			(this->spec.it_value.tv_nsec/1000000);
    return TIMEOUT-a;
 //	return ((this->spec.it_value.tv_sec)*1000)+((this->spec.it_value.tv_nsec)/1000000);

}

void tmr_startms(tmr_t* this, int ms) {
	this->spec.it_value.tv_sec = ms / 1000;
	this->spec.it_value.tv_nsec = (ms % 1000) * 1000000;
	this->spec.it_interval.tv_sec = 0;
	this->spec.it_interval.tv_nsec = 0;
	timer_settime(this->timerid, 0, &(this->spec),NULL);
}

void tmr_stop (tmr_t* this) {
    timer_delete (this->timerid);
}

