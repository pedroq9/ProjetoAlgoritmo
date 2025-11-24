#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

void util_init(void) {
    srand((unsigned) time(NULL));
}

/* sleep em ms */
void util_sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

/* limpar tela */
void util_clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* kbhit/getch portavel */
#ifdef _WIN32

int util_kbhit(void) {
    return _kbhit();
}

int util_getch(void) {
    return _getch();
}

#else

int util_kbhit(void) {
    struct timeval tv = {0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

int util_getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#endif

/* timestamp simples */
char *util_timestamp(char *buf, size_t n) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (!tm) { if (n) buf[0] = '\0'; return buf; }
    strftime(buf, n, "%Y-%m-%d_%H:%M:%S", tm);
    return buf;
}
