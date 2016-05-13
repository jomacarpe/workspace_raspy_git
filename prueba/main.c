
/*#include <wiringPi.h>
#include <stdio.h>

int main (){

	 int wiringPiSetupGpio(void);
	  digitalWrite (1, HIGH) ;


}*/
#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sched.h>
#include "cab.h"

int cuenta=0;
/*int main(void)
{
	wiringPiSetupGpio();
	pinMode (17, OUTPUT) ;
	pinMode (27, OUTPUT) ;
	pinMode (18, OUTPUT) ;
	pinMode (23, INPUT) ;
    digitalWrite(17,0);
    timer_start(1000);

	//timer_start(1000);
  //  alarma();
   // alarma1();
	//wiringPiISR (24, INT_EDGE_FALLING, &metodoInterrupcionExterna) ;
	//wiringPiISR (23, INT_EDGE_FALLING, &metodo1) ;
	wiringPiISR (23, INT_EDGE_RISING, & metodoInterrupcionExterna);

	//wiringPiISR (18, INT_EDGE_FALLING, &metodo1) ;




	  while (1)
	        pause();
    return 0;
}*/


