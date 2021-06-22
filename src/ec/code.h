

#ifndef _CODE_H_
#define _CODE_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"
#include "timer.h"

#include "alarm.h"

#define GPIO_CODE_BUTTON 16

enum code_fsm_states {
  IDLE,
  CUENTA,
  BUFFER_DIGITOS
};

//INTERRUPTIONS
void pulsador_isr   (void);


//FSM CREATION
fsm_t* fsm_new_code (void);

#endif