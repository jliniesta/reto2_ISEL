#include "alarm.h"

#define DEBOUNCE_TIME 200

static int codigo_ok = 0;
static int presencia = 0;
static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void presencia_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  presencia= 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}

void codigo_ok_isr (void) { 
  codigo_ok = 1;
}


//STATE CHECKING FUNCTIONS
static int check_presencia (fsm_t* this) { 
  return presencia; 
}

static int check_codigo_ok (fsm_t* this) { 
  return codigo_ok; 
}


//OUTPUT FUNCTIONS
static void activate_alarm (fsm_t* this) {
  presencia = 0;
  codigo_ok = 0;

  printf("\rALARM ACTIVATED \n");
}

static void deactivate_alarm (fsm_t* this) { 
  presencia = 0;
  codigo_ok = 0;

  printf("\rALARM DEACTIVATED \n");

  //Turn light and buzzer off
  digitalWrite (GPIO_LIGHT, 0); 
  digitalWrite (GPIO_BUZZER, 0); 
}

void trigger_alarm (fsm_t* this) { 
  presencia = 0;
  codigo_ok = 0;

  printf("\rPRESENCE DETECTED: ALARMA SUENA!!! \n");

  //Turn light and buzzer on
  digitalWrite (GPIO_LIGHT, 1); 
  digitalWrite (GPIO_BUZZER, 1); 
}


//FSM CREATION
fsm_t* fsm_new_alarm (void) {
	static fsm_trans_t tt[] = {
    { INACTIVA,  check_codigo_ok, ACTIVA, activate_alarm },
    { ACTIVA, check_codigo_ok, INACTIVA, deactivate_alarm },
    { ACTIVA, check_presencia, SUENA, trigger_alarm },
    { SUENA, check_codigo_ok, INACTIVA, deactivate_alarm },
    {-1, NULL, -1, NULL },
  };
	return fsm_new(tt);
}