CC = gcc
CFLAGS = -Wall - Werror

eliminar ficheros generados automaticamente:
	rm -rf ./pan* ./_spin_nvr.tmp ./*.pml.trail ./._n_i_p_s_

alarma:
	spin -a ./alarma.pml
	gcc -o ./pan ./pan.c
	./pan -a -f -N alarma_activa_detecta_presencia_entonces_alarma_suena
	./pan -a -f -N alarma_no_activa_alarma_no_suena
	./pan -a -f -N alarma_no_activa_pulsador_alarma_activa
	./pan -a -f -N alarma_activa_pulsador_alarma_desactiva

codigo:
	spin -a ./codigo.pml
	gcc -o ./pan ./pan.c
	./pan -a -f -N diez_sec_sin_pulsar_codigo_correcto_activa_codigo_ok_signal
	./pan -a -f -N diez_sec_sin_pulsar_codigo_incorrecto_no_activa_codigo_ok_signal

light:
	spin -a ./light.pml
	gcc -o ./pan ./pan.c
	./pan -a -f -N presencia_luz_enciende
	./pan -a -f -N no_presencia_luz_apaga