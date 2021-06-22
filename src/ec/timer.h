

#ifndef _TMR_H_
#define _TMR_H_

#include <signal.h>
#include <time.h>

//FSM Time Management functions
void timespec_sub (struct timespec *res, struct timespec *a, struct timespec *b);
void timespec_add (struct timespec *res, struct timespec *a, struct timespec *b);
int timespec_less (struct timespec *a, struct timespec *b);
void delay_until (struct timespec* next_activation);

#endif