#include "code.h"

#define DEBOUNCE_TIME 50
#define TIMER_PERIOD_SEG 1 //1seg
#define TIMER_PERIOD_SEG_1 10 //10seg
#define CUENTA_LIMIT 10
#define CODE_LENGTH 3

static const int correct_code[CODE_LENGTH] = {1, 2, 3};

static int pulsador = 0;

static int cuenta = 0;
static int index = 0;
static int current_code[CODE_LENGTH] = {0,0,0};

static struct timespec now;
static struct timespec end;
static struct timespec end10;
static struct timespec T = {TIMER_PERIOD_SEG, 0};
static struct timespec T10 = {TIMER_PERIOD_SEG_1, 0};

static int debounceTime = DEBOUNCE_TIME;

//INTERRUPTIONS ROUTINES
void pulsador_isr (void) { 

  //Debouncing procedure
  if (millis() < debounceTime){
    debounceTime = millis() + DEBOUNCE_TIME;
    return;
  }

  pulsador = 1;

  debounceTime = millis() + DEBOUNCE_TIME;
}




//TIMER START
static void start_code_timer(){
  //Start time
  clock_gettime(CLOCK_REALTIME, &end);
  //End time
  timespec_add(&end,&end,&T);
}

static void start_code_timer_1(){
  //Start time
  clock_gettime(CLOCK_REALTIME, &end10);
  //End time
  timespec_add(&end10,&end10,&T10);
}


//STATE CHECKING FUNCTIONS
static int check_pressed_pulsador (fsm_t* this) { 
  return pulsador & (index < CODE_LENGTH); 
}

static int check_cuenta_limit (fsm_t* this) { 
  return CUENTA_LIMIT <= cuenta; 
}

static int check_code_timer_ended (fsm_t* this) { 
  clock_gettime(CLOCK_REALTIME, &now);
  return timespec_less (&end, &now); //True if end < now
}

static int check_code_ended (fsm_t* this) { 
  return index >= CODE_LENGTH; 
}

static int check_code_timer1_ended (fsm_t* this) { 
  clock_gettime(CLOCK_REALTIME, &now);
  return timespec_less (&end10, &now); //True if end < now
}


//OUTPUT FUNCTIONS
static void increase_cuenta (fsm_t* this) {
    pulsador = 0;
 

    cuenta++;
    start_code_timer();

    printf("\rCUENTA INCREASED TO '%d' \n",cuenta);
}

static void process_digit (fsm_t* this) { 
    pulsador = 0;
  
    start_code_timer_1();
    if (cuenta < CUENTA_LIMIT) {
        current_code[index] = cuenta;
    } else {
        current_code[index] = 0;
    }

    
    if (index == 0) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,X,X]\n",index+1,current_code[0]);
    } else if (index == 1) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,X]\n",index+1,current_code[0],current_code[1]);
    } else if (index == 2) {
        printf("\rDIGIT '%d' ENTERED: CURRENT CODE = [%d,%d,%d]\n",index+1,current_code[0],current_code[1],current_code[2]);
    }
    

    cuenta = 0;
    index++;

    if (index < CODE_LENGTH){
        printf("\rENTER DIGIT '%d' \n",index+1);
    }
}

static void examine_code (fsm_t* this) { 
    pulsador = 0;
    

    int correct = 1;
    for(int i=0;i<CODE_LENGTH;i++){
        correct &= (current_code[i] == correct_code[i]);
        current_code[i] = 0;
        //printf("'%d' vs '%d' : %d\n",current_code[i],correct_code[i],correct);
    }

    if(correct){
        printf("\rENTERED CODE IS CORRECT. \n");
        codigo_ok_isr();
    }else{
        printf("\rENTERED CODE IS INCORRECT. TRY AGAIN. \n");
    }

    index = 0;
}


//FSM CREATION
fsm_t* fsm_new_code (void) {
	static fsm_trans_t tt[] = {
    { IDLE,  check_pressed_pulsador, CUENTA, increase_cuenta },
    { CUENTA, check_pressed_pulsador, CUENTA, increase_cuenta },
    { CUENTA, check_cuenta_limit, BUFFER_DIGITOS, process_digit },
    { CUENTA, check_code_timer_ended, BUFFER_DIGITOS, process_digit },
    { BUFFER_DIGITOS, check_pressed_pulsador, CUENTA, increase_cuenta },
    { BUFFER_DIGITOS, check_code_ended, IDLE, examine_code },
    { BUFFER_DIGITOS, check_code_timer1_ended, IDLE, examine_code},
    {-1, NULL, -1, NULL },
  };
	return fsm_new(tt);
}
