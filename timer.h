#ifndef TIMER_H_
#define TIMER_H_

void timer_setup(unsigned frequency);
void timer_shutdown();
unsigned timer_get_elapsed();

void hlt(void);
#pragma aux hlt = "hlt";

#endif
