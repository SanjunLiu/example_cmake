#ifndef _CHECK_DATE_H_
#define _CHECK_DATE_H_


#include <time.h>
#define YEAR    2016
#define MONTH   10
#define DAY_E   28
#define DAY_S   10
#define HOUR    23
#define MIN     59
#define SEC     59


typedef struct CURDATE{
    unsigned int year, month, day;
    unsigned int hour, min, sec;
}CUR_DATE__;

inline int date_check(void){
    time_t time_data;
    struct tm *ptime;
    CUR_DATE__ cur_date;

    time(&time_data);
    ptime = localtime(&time_data);
    cur_date.year   = 1900 + ptime->tm_year;
    cur_date.month  = 1 + ptime->tm_mon;
    cur_date.day    = ptime->tm_mday;
    cur_date.hour   = ptime->tm_hour;
    cur_date.min    = ptime->tm_min;
    cur_date.sec    = ptime->tm_sec;

#if 0
    LOGI("y %d ", cur_date.year);
    LOGI("m %d ", cur_date.month);
    LOGI("d %d ", cur_date.day);
    LOGI("h %d ", cur_date.hour);  //+" "+pdate->min+" "+ pdate->sec
#endif

    if ((cur_date.year > YEAR)) {
        return -2;
    } else if ((cur_date.year == YEAR) && (cur_date.month > MONTH)) {
        return -3;
    }

//    if((cur_date.day < DAY_S) || (cur_date.day > DAY_E))
//        return -3;

    return 0;
}


#endif
