

#include "light.h"

#define DEBOUNCE_TIME 200
#define TIMER_PERIOD_SEG 30 //30seg

static int presencia2 = 0;
static int timer_ended = 0;

static struct timespec now;
static struct timespec end;
static struct timespec T = {TIMER_PERIOD_SEG, 0};

static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void presencia2_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  presencia2 = 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}

void timer_light_isr(void) {
  timer_ended = 1;
}


//TIMER START
static void start_light_timer(){
  //Start time
  clock_gettime(CLOCK_REALTIME, &end);
  //End time
  timespec_add(&end,&end,&T);
}


//STATE CHECKING FUNCTIONS
static int check_presencia2 (fsm_t* this) { 
  return presencia2; 
}

static int check_light_timer_ended (fsm_t* this) {
  clock_gettime(CLOCK_REALTIME, &now); 
  return timespec_less (&end, &now); //True if end < now
}


//OUTPUT FUNCTIONS
static void turn_on_light (fsm_t* this) {
  presencia2 = 0;
  timer_ended = 0;

  printf("\rLIGHT ON! \n");

  start_light_timer();

  digitalWrite (GPIO_LIGHT, 1); 
}

static void turn_off_light (fsm_t* this) { 
  timer_ended = 0;

  printf("\rLIGHT OFF! \n");

  digitalWrite (GPIO_LIGHT, 0); 
}


//FSM CREATION
fsm_t* fsm_new_light (void) {
	static fsm_trans_t tt[] = {
    { OFF, check_presencia2, ON, turn_on_light },
    { ON, check_presencia2, ON, turn_on_light },
    { ON, check_light_timer_ended, OFF, turn_off_light },
    {-1, NULL, -1, NULL },
  };
	return fsm_new(tt);
}
