
#include "time.h"
#include "stdio.h"
#include "pthread.h"

#include "timer.h"
#include "ttyraw.h"
#include "termios.h"

#include "light.h"
#include "alarm.h"
#include "code.h"
#include "kbd.h"



int main () {

    //Clock registers
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	struct timespec T = {0, 0.050*1000000000}; //T = 50ms (Hs) 

    //Initialze input and output pins
    //initializePins()

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    fsm_t* kbd_fsm = fsm_new_kbd();
    fsm_t* light_fsm = fsm_new_light();
    fsm_t* alarm_fsm = fsm_new_alarm();
    fsm_t* code_fsm = fsm_new_code();

    //WELCOME MESSAGE
    printf("\r\n---------------------------------------------------------------------------------\n");
    printf("\rWELCOME!!! \n\n");
    printf("\rControls: \n");
    printf("\r    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("\r    'p'    -> Trigger presence sensor (presencia). \n");
    printf("\r    'q'    -> Exit program. \n");
    printf("\r-----------------------------------------------------------------------------------\n\n");

    //Frame index
    int frame = 0;

    while (1) {
        /* 
        CYCLIC EXECUTIVE (H = 500ms)
            KBD_FSM    -> T = 50ms
            CODE_FSM   -> T = 50ms
            ALARM_FSM  -> T = 250ms
            LIGHT_FSM -> T = 500ms
        */
        switch (frame) {
			case 0:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
                fsm_fire (alarm_fsm);
                fsm_fire (light_fsm);
				break;
			case 1:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
			case 2:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
			case 3:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
            case 4:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
			case 5:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
                fsm_fire (alarm_fsm);
                break;
			case 6:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
            case 7:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
			case 8:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
			case 9:
                fsm_fire (kbd_fsm);
				fsm_fire (code_fsm);
				break;
		}

        //Delay next execution
        timespec_add(&next,&next,&T);
        delay_until (&next);

        //Increment frame index
        frame = (frame+1)%10;
    }
}