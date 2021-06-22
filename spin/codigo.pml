/* 
* ISEL 2021
*
* RETO 2: Sistema de alarma domestica
*
* Javier López Iniesta Díaz del Campo
* Ivan Martín Cantón
* Fernando García Gutiérrez
*
* Codigo FSM
*
*/


// ESPECIFICACIONES LTL

// En cualquier momento, si espero 10 segundos sin pulsar e introduzco el código correcto, la señal codigo_ok se activa.
ltl diez_sec_sin_pulsar_codigo_correcto_activa_codigo_ok_signal {
    []((!pulsador W T10) && codigoCorrecto -> <>codigo_ok)
}

// En cualquier momento, si espero 10 segundos sin pulsar e introduzco el código incorrecto, la señal codigo_ok no se activa.
ltl diez_sec_sin_pulsar_codigo_incorrecto_no_activa_codigo_ok_signal {
    []((!pulsador W T10) && !codigoCorrecto -> <>!codigo_ok)
}

//Entradas
int pulsador = 0;

//Configuración de los timers
int T1 = 0; // Timer de 1 segundo
int T10 = 0; // Timer de 10 segundos

//Salidas
int codigo_ok = 0;

// Estados
mtype = {IDLE,CUENTA,BUFFER_DIGITOS}
byte codigo_state;

#define LIMITE_CUENTA  3 // Para reducir el tiempo de verificacion, en el modelo implementado será 10
#define CODIGO_LENGTH 3 // Numero de digitos del codigo

// Variables internas
byte codigo_correcto[CODIGO_LENGTH] = {1, 2,1};
byte codigo_actual[CODIGO_LENGTH] = {0,0,0};
int i;
int codigoCorrecto = 0;
int count = 0;
int index = 0;

// MODELO DEL CODIGO FSM 
active proctype codigo_fsm () {

	//Estado inicial
	codigo_state = IDLE;
	codigo_ok = 0;
	count = 0;
	index = 0;

    printf("\n----------------------------------\n");
    printf("Estado inicial: IDLE\n");
    printf("Cuenta: %d, codigo_ok: %d, index=%d\n", count, codigo_ok, index);
    printf("\n----------------------------------\n");

	do
	::	codigo_state == IDLE -> atomic {
		if 
		:: pulsador -> 
				codigo_state = CUENTA;
				pulsador = 0;	
				count++; 
				T1 = 0; 
				T10 = 0;
				index = 0;
				printf("pulsador -> Transicion desde el estado IDLE a CUENTA\n");
				printf("Cuenta: %d\n", count);
				// printf("Codigo introducido: [%d,%d,%d] \n", codigo_actual[0],codigo_actual[1],codigo_actual[2]);
		fi;
	}
	::	codigo_state == CUENTA -> atomic {
		if 
		:: pulsador ->
				pulsador = 0;
				count++; 
				T1 = 0; 
				T10 = 0;
				printf("pulsador -> Transicion desde el estado CUENTA a CUENTA\n");
				printf("Cuenta: %d\n", count);
				// printf("Codigo introducido: [%d,%d,%d] \n", codigo_actual[0],codigo_actual[1],codigo_actual[2]);

		:: (T1 || (LIMITE_CUENTA >= count)) ->
				codigo_state = BUFFER_DIGITOS;
				pulsador = 0;
				T1 = 0;				
				T10 = 0;
				//Añadir count al codigo actual
				if 
				:: count < LIMITE_CUENTA -> 
					codigo_actual[index] = count;
				:: else -> 
					codigo_actual[index] = 0; // Si sobrepasamos el limite de la cuenta se pone un 0
				fi
				count = 0;
				index++;
				printf("(T1 || (LIMITE_CUENTA <= count)) -> Transicion desde el estado CUENTA a BUFFER_DIGITOS\n");
				printf("El digito introducido ha sido el %d\n", codigo_actual[index-1]);
				printf("Codigo introducido: [%d,%d,%d] \n", codigo_actual[0],codigo_actual[1],codigo_actual[2]);

		fi;
	}
	::	codigo_state == BUFFER_DIGITOS -> atomic {
		if 
		:: (pulsador && (index < CODIGO_LENGTH)) ->
				codigo_state = CUENTA;
				pulsador = 0;
				count++;
				T1 = 0; 
				T10 = 0;
				printf("(pulsador && (index < CODIGO_LENGTH)) -> Transicion desde el estado BUFFER_DIGITOS a CUENTA\n");
				printf("Cuenta: %d\n", count);

		:: ((index == CODIGO_LENGTH) || T10)->
				codigo_state = IDLE;
				printf("((index == CODIGO_LENGTH) || T10) -> Transicion desde el estado BUFFER_DIGITOS a IDLE\n");
				pulsador = 0;
				i = 0;
				codigoCorrecto = 1;
				if
				:: T10 ->
					printf("10 segundos sin pulsar -> Estado inicial\n");
				fi;				
            	T1 = 0;
				T10 = 0;
				// Comprueba si el codigo introducido es correcto
				do 
				:: i < CODIGO_LENGTH -> 
					codigoCorrecto = (codigoCorrecto && (codigo_actual[i] == codigo_correcto[i]));
					codigo_actual[i] = 0;
					i++;
				:: else -> break
				od;

				if
				:: codigoCorrecto ->
					codigo_ok = 1;
					printf("Codigo introducido correcto !!!\n");
				:: !codigoCorrecto ->
					codigo_ok = 0;
					printf("Codigo introducido incorrecto !!!\n");
				fi;
				index = 0;
				count = 0;

		fi;
	}
	od;

    // printf("\n----------------------------------\n");
    // printf("Estado: %e\n", codigo_state);
    // printf("Cuenta: %d, codigo_ok: %d, index=%d\n", count, codigo_ok, index);
    // printf("\n----------------------------------\n");

}


// MODELO DEL ENTORNO
active proctype environment () {
	
	// printf("\n------------------CODIGO FSM------------------\n");
    // printf("Estado: %e\n", codigo_state);
    // printf("Cuenta: %d, codigo_ok: %d, pulsador: %d, index=%d\n", count, codigo_ok, pulsador, index);
    // printf("T1: %d, T10: %d\n", T1, T10);
    // printf("\n-----------------------------------------------\n");

	do
    :: skip -> skip
    :: pulsador = 1
    :: T1 = 1
    :: T10 = 1
    :: pulsador = 0
    :: T1 = 0
    :: T10 = 0	
	printf("\n-----------------------------------------------\n");
	printf("Estado: %e\n", codigo_state);
	printf("Cuenta: %d, codigo_ok: %d, pulsador: %d, digito=%d\n", count, codigo_ok, pulsador, index);
	printf("T1: %d, T10: %d\n", T1, T10);
	printf("Codigo introducido: [%d,%d,%d] \n", codigo_actual[0],codigo_actual[1],codigo_actual[2]);
	printf("\n-----------------------------------------------\n");
	od

}

// MODELO DEL CODIGO FSM MODEL -> Simplificación: desactiva la señal con el codigo correcto cuando esta a 1.
active proctype alarma_fsm () {
	do
	:: codigo_ok -> codigo_ok = 0
    :: !codigo_ok -> skip
	od;
}