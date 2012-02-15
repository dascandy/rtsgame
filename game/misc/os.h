#ifndef OS_H
#define OS_H

typedef unsigned int msec_t;
msec_t get_msec();
void delay(msec_t time); 
void platInit();
void Fatal(const char *error);

#endif

