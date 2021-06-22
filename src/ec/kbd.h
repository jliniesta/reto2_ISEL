#ifndef KBD_H
#define KBD_H

#include "fsm.h"
#include "ttyraw.h"
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include "light.h"
#include "code.h"
#include "alarm.h"

fsm_t* fsm_new_kbd(void);

#endif