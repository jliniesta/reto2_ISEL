
#include "kbd.h"

static char last_key = 0;

static int chk_input (fsm_t* this) {
	fd_set rdset;
	struct timeval timeout = {0,0};
	FD_ZERO (&rdset);
	FD_SET (0, &rdset);
	return select (1, &rdset, NULL, NULL, &timeout) > 0;
}

static void do_read (fsm_t* this) { 
	char c;
	fread (&c, 1, 1, stdin);
	printf ("\r\n--- Key pressed: %c ---\n\n", c);
	switch(c){

        case 'k' :
            pulsador_isr();
            break;
        case 'p' :
            presencia_isr();
            break;
        case 'q' :
            exit(-1);
            break;
        default : 
            break;
    }
}


//FSM CREATION
fsm_t* fsm_new_kbd() {
	static struct fsm_trans_t tt[] = {
		{ 0, chk_input, 0, do_read },
		{ -1, NULL, -1, NULL},
	};
	static int raw;
	if (!raw) {
		tty_raw();
		raw = 1;
	}
	return fsm_new(tt);
}