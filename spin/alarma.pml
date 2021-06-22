/* 
* ISEL 2021
*
* RETO 2: Sistema de alarma domestica
*
* Javier López Iniesta Díaz del Campo
* Ivan Martín Cantón
* Fernando García Gutiérrez
*
* Alarma FSM
*
*/

// ESPECIFICACIONES LTL

// Si la alarma está activa y se detecta presencia, la alarma suena.
ltl alarma_activa_detecta_presencia_entonces_alarma_suena {
	[](((alarm_state==ACTIVA) && presencia ) -> <>((alarm_state==SUENA) led && sonido));
}

// Si la alarma no está activa, la alarma no suena.
ltl alarma_no_activa_alarma_no_suena {
	[](((alarm_state==INACTIVA) && presencia) -> (!led && !sonido));
}

// Si la alarma no está activa y se presiona el pulsador, la alarma se activa.
ltl alarma_no_activa_pulsador_alarma_activa {
	[](((alarm_state==INACTIVA) && codigo_ok) -> <>(alarm_state==ACTIVA));
}

// Si la alarma está activa y se presiona el pulsador, la alarma se desactiva.
ltl alarma_activa_pulsador_alarma_desactiva {
	[]((((alarm_state==ACTIVA) || (alarm_state==SUENA)) && codigo_ok) -> <>((alarm_state==INACTIVA) && !led && !sonido));
}

//Entradas
int presencia = 0;
int codigo_ok = 0;

//Salidas
int led = 0;
int sonido = 0;

// Estados
mtype = {INACTIVA,ACTIVA,SUENA}
byte alarm_state;

// MODELO DE LA ALARMA FSM 
active proctype alarma_fsm () {

	//Estado inicial
	alarm_state = INACTIVA;
	led = 0;
	sonido = 0;

    printf("\n----------------------------------\n");
    printf("Estado inicial: INACTIVA\n");
    printf("Led: %d, Sonido: %d\n", led, sonido);
    printf("\n----------------------------------\n");

	do
	::	alarm_state == INACTIVA -> atomic {
		if 
		:: codigo_ok -> alarm_state = ACTIVA;
						presencia = 0;
						codigo_ok = 0; 
						led = 0;
						sonido = 0;
                        printf("codigo_ok -> Transicion desde el estado INACTIVA a ACTIVA\n");
		fi;
	}
	::	alarm_state == ACTIVA -> atomic {
		if 
		:: codigo_ok ->	alarm_state = INACTIVA;
						presencia = 0;
						codigo_ok = 0; 
						led = 0;
						sonido = 0;
                        printf("codigo_ok -> Transicion desde el estado ACTIVA a INACTIVA\n");
		:: presencia ->	alarm_state = SUENA;
						presencia = 0;
						codigo_ok = 0; 
						led = 1;
						sonido = 1;
                        printf("presencia -> Transicion desde el estado ACTIVA a SUENA\n");
		fi;
	}
	::	alarm_state == SUENA -> atomic {
		if 
		:: codigo_ok -> alarm_state = INACTIVA;
						presencia = 0;
						codigo_ok = 0; 
						led = 0;
						sonido = 0;
                        printf("codigo_ok -> Transicion desde el estado SUENA a INACTIVA\n");
		fi;
	}

    // printf("\n--------------------------------\n");
    // printf("Estado: %d\n", alarm_state);
    // printf("Led: %d, Sonido: %d\n", led, sonido);
    // printf("\n--------------------------------\n");

	od
}

// MODELO DEL ENTORNO
active proctype environment () {
	
	// printf ("\n---------------------------------------------------------------------------\n");
	// printf ("\n       ALARMA FSM 1--- estado:%e, codigo_ok:%d, presencia:%d, led:%d, sonido:%d \n", alarm_state, codigo_ok, presencia, led, sonido);
	// printf ("\n---------------------------------------------------------------------------\n");

	do
    :: skip -> skip
    :: presencia = 1
    :: codigo_ok = 1

	printf ("\n---------------------------------------------------------------------------\n");
	printf ("\n      ALARMA FSM --- estado:%e, codigo_ok:%d, presencia:%d, led:%d, sonido:%d \n", alarm_state, codigo_ok, presencia, led, sonido);
	printf ("\n---------------------------------------------------------------------------\n");
	
	od
    
}

// MODELO DEL CODIGO FSM MODEL -> Simplificación: activa la señal codigo_ok eventualmente.
active proctype codigo_fsm () {
	do
	:: codigo_ok = 1
    :: !(codigo_ok) -> skip
	od;
}