
#include "time.h"
#include "stdio.h"

#include "reactor.h"

#include "kbd.h"
#include "light.h"
#include "alarm.h"
#include "code.h"

//FSM Declarations
static fsm_t* kbd_fsm;
static fsm_t* light_fsm;
static fsm_t* alarm_fsm;
static fsm_t* code_fsm;


//REACTOR EVENTS
//Process key task and update timers state
static void kbd_task (struct event_handler_t* this) {
    static const struct timeval period = {0, 0.05*1000000};
    fsm_fire(kbd_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &period);
}

//Alarm fsm task
static void alarm_task (struct event_handler_t* this) {
    static const struct timeval period = {0, 0.25*1000000};
    fsm_fire(alarm_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &period);
}

//Code fsm task
static void code_task (struct event_handler_t* this) {
    static const struct timeval period = {0, 0.05*1000000};
    fsm_fire(code_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &period);
}

//light fsm task
static void light_task (struct event_handler_t* this) {
    static const struct timeval period = {0, 0.5*1000000};
    fsm_fire(light_fsm);
    timeval_add (&this->next_activation, &this->next_activation, &period);
}




int main () {

    //Initialze input and output pins
    //initializePins()

    //Initialize events
    EventHandler task_kbd, task_alarm, task_code, task_light;
    reactor_init();

    /*
    * Finite States Machine
    * { OriginState, Trigger, DestinationState, Actions }
    */
    kbd_fsm = fsm_new_kbd();
    light_fsm = fsm_new_light();
    alarm_fsm = fsm_new_alarm();
    code_fsm = fsm_new_code();

    //WELCOME MESSAGE
    printf("\r\n-----------------------------------------------------------------------------------\n");
    printf("\rWELCOME!!! \n\n");
    printf("\rControls: \n");
    printf("\r    'k'    -> Enter digit of alarm code (wait 1sec to enter next digit). \n");
    printf("\r    'p'    -> Trigger presence sensor (PIR). \n");
    printf("\r    'q'    -> Exit program. \n");
    printf("\r-----------------------------------------------------------------------------------\n\n");

    //Initialize and add tasks
    event_handler_init (&task_kbd, 4, kbd_task);
    reactor_add_handler (&task_kbd);

    event_handler_init (&task_code, 3, code_task);
    reactor_add_handler (&task_code);

    event_handler_init (&task_alarm, 2, alarm_task);
    reactor_add_handler (&task_alarm);

    event_handler_init (&task_light, 1, light_task);
    reactor_add_handler (&task_light);

    //Start handling them
    while (1) {
        reactor_handle_events ();
    }

}