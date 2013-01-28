#include "logger.h"

void flm_log(const char * msg)
{
    time_t current_time;
    current_time = time(NULL);
    struct tm *t = localtime(&current_time);
    printf("[%d/%d/%d %d:%d:%d] - %s\n", t->tm_mday, 
            t->tm_mon + 1, t->tm_year + 1900, t->tm_hour,
            t->tm_min, t->tm_sec, msg);
}
