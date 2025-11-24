#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>   
#include <stdio.h>    

void util_init(void);
void util_sleep_ms(int ms);
void util_clear_screen(void);
int util_kbhit(void);
int util_getch(void);
char *util_timestamp(char *buf, size_t n);

int util_garantir_pasta_data(void);

#endif
