/*
 * timer.h
 *
 * Created: 12/7/2014 8:20:53 PM
 *  Author: eric
 */ 


#ifndef TIMER_H_
#define TIMER_H_


#define ISLEAP(n) (n%4==0 && (n%100!=0 || n%400==0))
#define YEAR_SEC (365L * 24 * 60 * 60)
#define LYEAR_SEC (366L * 24 * 60 * 60)
#define DAYS_31 (31L * 24 * 60 * 60)
#define DAYS_29 (29L * 24 * 60 * 60)
#define DAYS_28 (28L * 24 * 60 * 60)
#define DAYS_30 (30L * 24 * 60 * 60)
#define DAY_1 (24LL * 60 * 60)

void init_timer();
char *ctime();
void printtime();

#endif /* TIMER_H_ */