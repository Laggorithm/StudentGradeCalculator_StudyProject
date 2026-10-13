#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <threads.h>
#include <time.h>
#endif
#include "app.h"

int main(void)
{
    int result = app_run(stdin, stdout);
    if (result == EXIT_SUCCESS) {
        printf("\nProgram will close in 10 seconds...\n");
        fflush(stdout);
#ifdef _WIN32
        Sleep(10000);
#else
        struct timespec delay = {10, 0};
        while (thrd_sleep(&delay, &delay) == -1) {
        }
#endif
    }
    return result;
}
