#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int dayofweek(int d, int m, int y)
{
    int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

bool is_trade_date(const char * date)
{                                                                                                                                         
    if (!date)
        return false;

    int year, mon, day, weekday;
    int len = strlen(date);
    if (len < 8)
        return false;

    {   
        std::string str(date, 0, 4); 
        year = atoi(str.c_str());
    }   

    {   
        std::string str(date, 4, 2); 
        mon = atoi(str.c_str());
    }   


    {   
        std::string str(date, 6, 2); 
        day = atoi(str.c_str());
    }   

    weekday = dayofweek(day, mon, year);
    if (weekday > 5 || weekday < 1)
        return false;

    printf("year:%d mon:%d day:%d weekday:%d\n", year, mon, day, weekday);
    return true;
}

int main(int argc, char *argv[])
{
    is_trade_date("20180810");
    
    return 0;
}
