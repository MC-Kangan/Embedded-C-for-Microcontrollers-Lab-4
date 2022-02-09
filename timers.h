#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void Timer0_init(void);
char get16bitTMR0val(void);
void Timer0_restart();

#endif
