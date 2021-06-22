

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"
#include "timer.h"

#define GPIO_PRESENCIA	12
#define GPIO_LIGHT	  4

enum light_fsm_states {
  ON,
  OFF,
};

//INTERRUPTIONS
void presencia2_isr (void);
void timer_light_isr (void);

//FSM CREATION
fsm_t* fsm_new_light (void);

#endif
