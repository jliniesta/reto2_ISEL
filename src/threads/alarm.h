

#ifndef _ALARM_H_
#define _ALARM_H_

#include "stdlib.h"
#include "wiringPi.h"
#include "fsm.h"
#include "stdio.h"

#define GPIO_PRESENCIA  12
#define GPIO_ALARM_ST	14
#define GPIO_LIGHT	  4
#define GPIO_BUZZER	  2

enum alarm_fsm_states {
  INACTIVA,
  ACTIVA,
  SUENA
};

//INTERRUPTIONS
void presencia_isr         (void);
void codigo_ok_isr (void);

//FSM DEFINITION
fsm_t* fsm_new_alarm (void);

#endif