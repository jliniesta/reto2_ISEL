

#include "timer.h"
#include <stdlib.h>

//FSM TIME MANAGEMENT
void timespec_sub (struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec;
	res->tv_nsec = a->tv_nsec - b->tv_nsec;
	if (res->tv_nsec < 0) {
		--res->tv_sec;
		res->tv_nsec += 1000000000;
	}
}

/* res = a + b */
void timespec_add (struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec + b->tv_sec;
	res->tv_nsec = a->tv_nsec + b->tv_nsec;
	if (res->tv_nsec > 1000000000) {
		++res->tv_sec;
		res->tv_nsec -= 1000000000;
	}
}

/* a < b */
int timespec_less (struct timespec *a, struct timespec *b)
{
  return (a->tv_sec < b->tv_sec) ||
    ((a->tv_sec == b->tv_sec) && (a->tv_nsec < b->tv_nsec));
}

/* wait until next_activation (absolute time) */
void delay_until (struct timespec* next_activation)
{
	struct timespec resto, now;
	clock_gettime(CLOCK_REALTIME, &now);
	timespec_sub(&resto, next_activation, &now);
	while (nanosleep(&resto,&resto)>0);
}