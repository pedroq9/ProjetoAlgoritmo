#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void util_init(void);
void util_sleep_ms(int ms);
void util_clear_screen(void);
int util_kbhit(void);   /* retorna 1 se tecla pressionada */
int util_getch(void);   /* le caractere sem esperar ENTER */
char *util_timestamp(char *buf, size_t n);

#endif
