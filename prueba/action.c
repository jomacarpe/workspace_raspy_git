/*
 * action.c
 *
 *  Created on: 30 de abr. de 2016
 *      Author: miguelsanchez
 */

#include <stdlib.h>  // para el NULL
#include <wiringPi.h>
#include "fsm.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <signal.h>
#include <time.h>

#define BUTTON_START 20 
#define LED_START 16

#define BUTTON_1 19
#define LED_1 26

#define BUTTON_2 21
#define LED_2 13

#define BUTTON_3 11
#define LED_3 5

#define BUTTON_4 27
#define LED_4 22

//#define NBUTTONS 4


#define FLAG_BUTTON_1 1
#define FLAG_BUTTON_2 2
#define FLAG_BUTTON_3 4
#define FLAG_BUTTON_4 8
#define FLAG_BUTTON_START 16
#define FLAG_TIMER	32
#define anti_rebote 300
#define btn_fail_max 5

/*#define buttonsArray [] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, 100};*/
#define ledsArray [] = {LED_1, LED_2, LED_3, LED_4};

long long int penalty_time= 3;


int button_start =0;

int ledOn =0;
long int timeSeg=0;
long int timeNseg=0;
long long int game_time=0;
int TIMEOUT= 5*1000000;
int fallos=0;
int round=0;


//digitalWrite (LED_START, 1);

int flags = 0;
void button_1_isr (void) { flags = 1; }
void button_2_isr (void) { flags = 2; }
void button_3_isr (void) { flags = 3; }
void button_4_isr (void) { flags = 4; }
void button_start_isr (void) { flags =10; printf("pulse boton start"); fflush(stdout);}


void timer_isr (union sigval value) { flags |= FLAG_TIMER; }
int timeout (fsm_t* this) { return (flags & FLAG_TIMER); }

int button1_pushed (fsm_t* this) { if(flags==1){return (1);}else {return 0;} }
int button2_pushed (fsm_t* this) { if(flags==2){return (1);}else {return 0;} }
int button3_pushed (fsm_t* this) { if(flags==3){return (1);}else {return 0;} }
int button4_pushed (fsm_t* this) { if(flags==4){return (1);}else {return 0;} }

int EVENT_BTN_START_END(fsm_t* this) { if(flags==10){return (1);}else {return 0;} }


void all_Led_Of(fsm_t* this){
	digitalWrite(LED_1,0);
	digitalWrite(LED_2,0);
	digitalWrite(LED_3,0);
	digitalWrite(LED_4,0);

}

void led_On(fsm_t* this, int led){
	all_Led_Of(this);
	digitalWrite(led,1);
	ledOn = led;
}


void randomLedOn(fsm_t* this){
	all_Led_Of(this);
	int randomN = rand() % 4; // Genero numero aleatorio
	round++;
	if(randomN == 0){    //enciendo el led
		led_On(this, LED_1);
	//	printf("led1"); fflush(stdout);
	} else if(randomN ==1){
		led_On(this, LED_2);
	//	printf("led2"); fflush(stdout);
	} else if(randomN ==2){
		led_On(this, LED_3);
	//	printf("led3"); fflush(stdout);
	} else if(randomN ==3){
		led_On(this, LED_4);
	//	printf("led4"); fflush(stdout);
	}
//	tmr_startms((tmr_t*)(this->user_data), TIMEOUT);
	flags=0;
}

void startGame(fsm_t* this){
	digitalWrite (LED_START, 0);
	round=0;
	fallos=0;
	flags=0;
	ledOn =0;
	game_time=0;
	randomLedOn(this);


}
void gameOver(fsm_t* this,tmr_t* this1){
	digitalWrite (LED_START, 1);
	digitalWrite (LED_1, 0);
	digitalWrite (LED_2, 0);
	digitalWrite (LED_3, 0);
	digitalWrite (LED_4, 0);

}


void turnOff(fsm_t* this){
	flags &= (~FLAG_TIMER);
	digitalWrite (LED_1, 0);
	digitalWrite (LED_2, 0);
	digitalWrite (LED_3, 0);
	digitalWrite (LED_4, 0);
	digitalWrite (LED_START, 0);
}

int time_out (fsm_t* this) { return (flags & FLAG_TIMER); }

/*int readTime(tmr_t* this){
 	timer_gettime (this->timerid, &(this->spec));
 	game_time= ((TIMEOUT/1000  - this->spec.it_value.tv_sec)*1000000000) +
 			(1000000000 -this->spec.it_value.tv_nsec);
    return game_time;

}*/

int EVENT_BTN_OK(fsm_t* this,tmr_t* this1){ //METODO QUE INDICA SI SE APRETO EL BOTON CORRECTO
	if(flags==1 || flags==2 || flags==3 || flags==4){  //compruebo si pulse algun boton
		//printf("pulso algo"); fflush(stdout);
	if(ledOn == LED_1){ if(flags==1){
		//readTime(this1);
					fallos++;
					delay(anti_rebote);
					return (1);}
				else{ delay(anti_rebote);return (0);
				}}
	if(ledOn == LED_2){if(flags==2){
		//readTime(this1);
					fallos++;
					delay(anti_rebote);
					return (1);}
				else{ delay(anti_rebote);return (0);
				}}
	if(ledOn == LED_3){if(flags==3){
		//readTime(this1);
					fallos++;
					delay(anti_rebote);
					return (1);}
				else{ delay(anti_rebote);return (0);
				}}
	if(ledOn == LED_4){if(flags==4){
		//readTime(this1);
					fallos++;
					delay(anti_rebote);
					return (1);}
				else{ delay(anti_rebote);return (0);
				}}}
	return 0;
}



int EVENT_BTN_FAIL(fsm_t* this,tmr_t* this1){
	if(flags==1 || flags==2 || flags==3 || flags==4){  //compruebo si pulse algun boton
		if(ledOn == LED_1){ if(flags==2 || flags==3 || flags==4){
			//readTime(this1);
			printf("fallo"); fflush(stdout);
			fallos++;
			delay(anti_rebote);
			return (1);}
		else{ delay(anti_rebote);return (0);
		}}
		if(ledOn == LED_2){if(flags==1 || flags==3 || flags==4){
			//readTime(this1);
			printf("fallo"); fflush(stdout);

			fallos++;
			delay(anti_rebote);
			return (1);}
		else{ delay(anti_rebote);return (0);
		}}
		if(ledOn == LED_3){if(flags==1 || flags==2 || flags==4){
			//readTime(this1);
			printf("fallo"); fflush(stdout);
			fallos++;delay(anti_rebote);
			return (1);}
		else{ delay(anti_rebote);return (0);
		}}
		if(ledOn == LED_4){if(flags==1 || flags==2 || flags==3){
			//readTime(this1);
			printf("fallo"); fflush(stdout);

			fallos++;delay(anti_rebote);
			return (1);}
		else{ delay(anti_rebote);return (0);
		}}
		}
		return 0;

}

int EVENT_END_GAME(fsm_t* this,tmr_t* this1){
	if(round>=5 /*|| fallos >= btn_fail_max*/){
		return 1;
	}
	return 0;
}

void printData(fsm_t* this, tmr_t* this1){
	printf("tiempo queda = %lld\n", game_time);
	  //  fflush(stdout); // Will now print everything in the stout buffer
}


int main(){
	//Lista de transiciones
	// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
	tmr_t* tmr = tmr_new (timer_isr);
	fsm_trans_t actionList[] = {
			{WAIT_START, EVENT_BTN_START_END, WAIT_PUSH, startGame},
			{WAIT_PUSH, EVENT_BTN_OK, WAIT_PUSH, randomLedOn},
			{WAIT_PUSH, EVENT_BTN_FAIL, WAIT_PUSH, randomLedOn},
			{WAIT_PUSH, EVENT_END_GAME, WAIT_END, gameOver},
			{WAIT_END, EVENT_BTN_START_END, WAIT_START,printData},
			{-1, NULL, -1,NULL},
	};
	fsm_t* machine = fsm_new(WAIT_START,actionList,tmr);
	//unsigned int next;

	 wiringPiSetupGpio();
	 pinMode (LED_START, OUTPUT);
	 digitalWrite(LED_START, 1);
	 pinMode (LED_1, OUTPUT);
	 digitalWrite(LED_1, 0);
	 pinMode (LED_2, OUTPUT);
	 digitalWrite(LED_2, 0);
	 pinMode (LED_2, OUTPUT);
	 digitalWrite(LED_2, 0);
	 pinMode (LED_3, OUTPUT);
	 digitalWrite(LED_3, 0);
	 pinMode (LED_4, OUTPUT);
	 digitalWrite(LED_4, 0);
	 pinMode (BUTTON_START, INPUT);
	 pinMode (BUTTON_1, INPUT);
	 pinMode (BUTTON_2, INPUT);
	 pinMode (BUTTON_3, INPUT);
	 pinMode (BUTTON_4, INPUT);

	 wiringPiISR (BUTTON_1, INT_EDGE_RISING, button_1_isr);
	 wiringPiISR (BUTTON_2, INT_EDGE_RISING, button_2_isr);
	 wiringPiISR (BUTTON_3, INT_EDGE_RISING, button_3_isr);
	 wiringPiISR (BUTTON_4, INT_EDGE_RISING, button_4_isr);
	 wiringPiISR (BUTTON_START, INT_EDGE_RISING, button_start_isr);

//	 apagar (interruptor_tmr_fsm);
	// next = millis();
	 //unsigned int next = millis();
	 while (1) {
	 fsm_fire (machine);
	 }
	 tmr_destroy ((tmr_t*)(machine->user_data));
	 fsm_destroy (machine);
	 return 0;
	}



