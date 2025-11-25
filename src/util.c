#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stddef.h>   

#if defined(_WIN32) || defined(_WIN64)
// EM windows
#include <conio.h>
#include <windows.h>
#include <direct.h>       
#include <sys/stat.h>     
#define MKDIR(path) _mkdir(path)
#else



// LINUX ou Mac OS
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>    
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

// inicialização / utilidades

void util_init(void) {
    srand((unsigned)time(NULL));
}

void util_sleep_ms(int ms) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void util_clear_screen(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

int util_kbhit(void) {
#if defined(_WIN32) || defined(_WIN64)
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
#endif
}

int util_getch(void) {
#if defined(_WIN32) || defined(_WIN64)
    return _getch();
#else
    int c = getchar();
    return c;
#endif
}

char *util_timestamp(char *buf, size_t n) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    snprintf(buf, n, "%02d/%02d/%04d %02d:%02d:%02d",
             tm_info->tm_mday,
             tm_info->tm_mon + 1,
             tm_info->tm_year + 1900,
             tm_info->tm_hour,
             tm_info->tm_min,
             tm_info->tm_sec
    );

    return buf;
}

int util_garantir_pasta_data(void)
{
    struct stat st;

    //se já existe diretório 
    if (stat("data", &st) == 0) {
        #ifndef _WIN32
        if (S_ISDIR(st.st_mode)) return 1;
        #else

        if ((st.st_mode & _S_IFDIR) != 0) return 1;
        #endif
        return 0;  
    }

    if (MKDIR("data") == 0) return 1;

    if (errno == EEXIST) return 1;

    return 0;
}
