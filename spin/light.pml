/* 
* ISEL 2021
*
* RETO 2: Sistema de alarma domestica
*
* Javier López Iniesta Díaz del Campo
* Ivan Martín Cantón
* Fernando García Gutiérrez
*
* Light FSM
*
*/

// ESPECIFICACIONES LTL

// Si hay presencia, la luz se enciende.
ltl presencia_luz_enciende {
	[](presencia -> <>light);
}

// Si han pasado 30 segundos desde la última vez que se detectó presencia, la luz se apaga.
ltl  no_presencia_luz_apaga{
    [] ((T30 && (!presencia W !light)) -> <> !light) 
}

//Entradas
int presencia = 0;
int T30 = 0;

//Salidas
int light = 0;

//Estados
mtype = {OFF,ON}
byte led_state;

// MODELO DEL LIGHT FSM 
active proctype switch_fsm () {

	//Estado inicial
	led_state = OFF;
	T30 = 0;
	light = 0;

    printf("\n----------------------------------\n");
    printf("Estado inicial: OFF\n");
    printf("T30: %d, light: %d, presencia: %d \n", T30, light, presencia);
    printf("\n----------------------------------\n");

	do
	::	led_state == OFF -> atomic {
		if 
		:: presencia -> led_state = ON;
						presencia = 0;
						light = 1;
						T30 = 0;
						printf("(button) Transicion desde el estado OFF a ON \n");

		fi;
	}
	::	led_state == ON -> atomic {
		if 
		:: presencia ->	presencia = 0;
						T30 = 0;
						printf("(presencia) Transicion desde el estado ON a ON \n");

		:: (T30 && !presencia) -> led_state = OFF;
								  T30 = 0;
							      light = 0;
								  printf("(T30 && !presencia)  Transicion desde el estado ON a OFF \n");
		fi;
	}

    // printf("\n--------------------------------\n");
    // printf("Estado: %d\n", led_state);
    // printf("T30: %d, light: %d, presencia: %d \n", T30, light, presencia);
    // printf("\n--------------------------------\n");

	od
}


// MODELO DEL ENTORNO
active proctype environment () {
	
    printf("\n--------------------------------\n");
    printf("Estado: %e\n", led_state);
    printf("T30: %d, light: %d, presencia: %d \n", T30, light, presencia);
    printf("\n--------------------------------\n");

	do
    :: skip -> skip
    :: presencia = 1
    :: T30 = 1
	printf("\n--------------------------------\n");
	printf("Estado: %e\n", led_state);
	printf("T30: %d, light: %d, presencia: %d \n", T30, light, presencia);
	printf("\n--------------------------------\n");
	od

}