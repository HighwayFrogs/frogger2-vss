
#ifndef BBTIMER_H
#define BBTIMER_H

typedef struct
{
	int start;
	int total;
	int on;

} BBTIMER;

void ZeroTime(void);
void TimerStart(BBTIMER* t);
void TimerRestart(BBTIMER* t);
void TimerStop(BBTIMER* t);
void TimerStopAdd(BBTIMER* t);



#endif
