#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include "t_extra.h"
#include <sstream>

using namespace std;

//Wed Mar 13 15:34:46 2013

/**
0-2: day of week (name)
4-6: name of month
8-9: day of month
11-12: hour
14-15: minute
17-18: second
20-23: year
*/

//handy dandy functions...

/**All function members of class chrono_date take asc_time and extract
data from it.  asc_time is defaulted to the current time as returned
by the system.   This allows for only 1 piece of data, to represent many
 different pieces of data, as well as makes it easier to retrieve the
 individual pieces.*/

template<class type1, class type2>
type2 conv(const type1& t1)
{
    stringstream ss;
    type2 t2 = type2();
    ss<< t1;
    ss>> t2;
    ss.str("");
    return t2;
}


/*-------------------------------------- 
 * Constructors                        |
 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

chrono_date::chrono_date()
{
    chrono_date::asc_time = chrono_date::t();
    chrono_date::year_v = chrono_date::year();
}

chrono_date::chrono_date(const string& s)
{
    chrono_date::asc_time = s;
    chrono_date::year_v = chrono_date::year();
}

chrono_date::chrono_date(const char* ch)
{
    chrono_date::asc_time = ch;
    chrono_date::year_v = chrono_date::year();
}

chrono_date::~chrono_date()
{
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * Constructors                        |
 *------------------------------------*/


/*****************************************
 * Member functions of chrono_date():    |
 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

const string chrono_date::gasc_time()
{
    return chrono_date::asc_time;
}

/* Returns the military hour as an integer.  This is
usful for calculations.*/
int chrono_date::hour()
{
    stringstream ss;
    int hourtime = 0;
    ss.str("");

    string temps = "";
    if(asc_time.size() > 11)
    {
        for(unsigned int x = 11; x < 13; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }

    //convert the (suppposed) hour into an integer value now
    ss<< temps;
    ss>> hourtime;
    ss.str("");
    return hourtime;
}

/*Returns raw asctime as a string.  All time information
is gathered through this.*/
string chrono_date::t()
{
    string tme;
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    tme = asctime(timeinfo);
    tme.resize((tme.size() - 1));
    return tme;
}

/* Returns the minute value as an integer.  Usflul
for calculations.*/
int chrono_date::minute()
{
    stringstream ss;
    ss.str("");
    int minutetime = 0;
    string temps = "";

    if(asc_time.size() > 14)
    {
        for(unsigned int x = 14; x < 16; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }

    ss<< temps;
    ss>> minutetime;
    ss.str("");
    return minutetime;
}

/* Returns the second value as an integer.  This
is useful for calculations.*/
int chrono_date::second()
{
    stringstream ss;
    ss.str("");

    int secondtime = 0;
    string temps = "";

    if(asc_time.size() > 17)
    {
        for(unsigned int x = 17; x < 19; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }

    ss<< temps;
    ss>> secondtime;
    ss.str("");
    return secondtime;
}

/* This returns the name of the day of the week as
a string.*/
string chrono_date::day_of_week()
{
    string temps = "";
    if(asc_time.size() > 1)
    {
        for(unsigned int x = 0; x < 3; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }
    return temps;
}

/* Returns the numeric value which represents the day of the week.
1 = sunday, and 7 = saturday*/
int chrono_date::num_day_of_week()
{
    vector<string> week_names = {
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat"
    };
    string day = "";
    day = chrono_date::day_of_week();
    for(unsigned int x = 0; x < week_names.size(); x++)
    {
        if(week_names[x] == day)
        {
            return (x + 1);
        }
    }
    return 0;
}

/* Returns the integer which represents the day of the month.*/
int chrono_date::day_of_month()
{
    stringstream ss;
    ss.str("");

    //0 cant be a month day... :|
    int monthday = 1;
    string temps = "";
    if(asc_time.size() > 8)
    {
        for(unsigned int x = 8; x < 10; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }

    ss<< temps;
    ss>> monthday;
    ss.str("");
    return monthday;
}

/* Returns a string which represents the month name.*/
string chrono_date::month_name()
{
    string temps = "";
    if(asc_time.size() > 5)
    {
        for(unsigned int x = 4; x < 7; x++)
        {
            temps = (temps + asc_time[x]);
        }
    }
    return temps;
}

/* Returns the integer value of the month.  1 = january, 12 = december.*/
int chrono_date::month_number()
{
    string mname = chrono_date::month_name();
    vector<string> months = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
    };

    for(unsigned int x = 0; x < months.size(); x++)
    {
        if(mname == months[x])
        {
            return (x + 1);
        }
    }
    return 0;
}

/* Returns the integer value of the year.*/
int chrono_date::year()
{
    string temps = "";
    int yeari = 0;
    stringstream ss;
    ss.str("");
    if(chrono_date::asc_time.size() > 22)
    {
        for(unsigned int x = 20; x < 24; x++)
        {
            temps = (temps + chrono_date::asc_time[x]);
        }
    }
    ss<< temps;
    ss>> yeari;
    ss.str("");
    return yeari;
}

/* Returns an string value which represents
the day of the week.  This can be used for calculations.*/
string chrono_date::week_day_id_name(const int& id)
{
    vector<string> week_names = {
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat"
    };
    string temps = chrono_date::day_of_week();
    int i = id;
    i--;

    //so that we do not "step out-of-bounds", lets modulate if its too big
    while(i > 6)
    {
        i = (i - 7);
    }

    for(unsigned int x = 0; x < week_names.size(); x++)
    {
        if(unsigned(i) == x)
        {
            return week_names[x];
        }
    }
    return "Mon";
}

string chrono_date::week_day_id_name()
{
    return chrono_date::week_day_id_name(chrono_date::day_number());
}

/* Returns true if asc_time represents a date which
is earlier than the present time, aka: it's the past (relative to the current time)
 
 NOTE: May help to think of this is chrono_date::is_earlier_than()*/
bool chrono_date::is_past(const string& curt)
{
    chrono_date pres_time = chrono_date();
    pres_time.asc_time = curt;
    chrono_date temp;

    //using a temporary class so that we can also change this to
    //somthing other than the current time if we want to.
    temp.asc_time = asc_time;
    /*
    from the biggest value, to the smallest:

    chrono_date::time < pres_time to be true

    year
    month
    day of month
    hour
    minute
    second
    */

    if(temp.year() < pres_time.year())
    {
        return true;
    }
    if(temp.year() == pres_time.year())
    {
        if(temp.month_number() < pres_time.month_number())
        {
            return true;
        }
        if(temp.month_number() == pres_time.month_number())
        {
            if(temp.day_of_month() < pres_time.day_of_month())
            {
                return true;
            }
            if(temp.day_of_month() == pres_time.day_of_month())
            {
                if(temp.hour() < pres_time.hour())
                {
                    return true;
                }
                if(temp.hour() == pres_time.hour())
                {
                    if(temp.minute() < pres_time.minute())
                    {
                        return true;
                    }
                    if(temp.minute() == pres_time.minute())
                    {
                        if(temp.second() < pres_time.second())
                        {
                            return true;
                        }
                        if(temp.second() == pres_time.second())
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool chrono_date::is_past()
{
    return chrono_date::is_past(chrono_date::t());
}

/* Returns a string: "AM" or "PM", depending
on the value of asc_time.*/
string chrono_date::std_ampm()
{
    string ampm = "AM";
    int h = 1;
    h = chrono_date::hour();
    if(h > 11)
    {
        ampm = "PM";
    }
    return ampm;
}

/* Returns hour as standard hour, instead of
military hour.*/
int chrono_date::std_hour()
{
    int h = 1;
    h = chrono_date::hour();

    //this is because when h = 0, h = 12 AM
    if(h == 0)
    {
        h = 24;
    }

    while(h > 12)
    {
        h = (h - 12);
    }
    return h;
}

/* This is just for context.  Thought it would be nice.*/
int chrono_date::std_minute()
{
    return chrono_date::minute();
}

/* "mm/dd/yyyy HH:MM AMP/PM" */
string chrono_date::std_time_disp()
{
    string temps = "";
    stringstream ss;

    ss.str("");

    //format: mm/dd/yyyy  hh:mm AM/PM
    if(chrono_date::month_number() < 10)
    {
        temps = (temps + "0");
    }
    ss<< chrono_date::month_number();
    temps = (temps + ss.str() + "/");

    ss.str("");

    if(chrono_date::day_of_month() < 10)
    {
        temps = (temps + "0");
    }
    ss<< chrono_date::day_of_month();
    temps = (temps + ss.str() + "/");

    ss.str("");

    ss<< chrono_date::year();
    temps = (temps + ss.str() + "  ");

    ss.str("");

    if(chrono_date::hour() < 10)
    {
        temps = (temps + "0");
    }
    ss<< chrono_date::std_hour();
    temps = (temps + ss.str() + ":");

    ss.str("");

    if(chrono_date::std_minute() < 10)
    {
        temps = (temps + "0");
    }
    ss<< chrono_date::minute();
    temps = (temps + ss.str() + " " + chrono_date::std_ampm());

    ss.str("");

    return temps;
}

bool chrono_date::is_leap_year(const int& setty)
{
    int y = setty;
    while(y > 4)
    {
        y = (y - 4);
    }
    if(y == 4)
    {
        return true;
    }
    return false;
}

bool chrono_date::is_leap_year()
{
    return chrono_date::is_leap_year(chrono_date::year());
}

/* Returns the day number relative to the first day this year.*/
int chrono_date::day_number()
{
    vector<int> month_days = {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };
    if(chrono_date::is_leap_year() == true)
    {
        if(month_days[1] == 28)
        {
            month_days[1]++;
        }
    }

    int dom = chrono_date::day_of_month(), dayid = 0;
    dayid = (dayid + dom);
    for(int x = 0; x < (chrono_date::month_number() - 1); x++)
    {
        dayid = (dayid + month_days[x]);
    }
    return dayid;
}

/* Sets chrono_date::asc_time to the date of the current
 date's id + arg id*/
void chrono_date::set_day_id(const int& newnum)
{   
    int year = chrono_date::year();
    int day_id_of_week = chrono_date::num_day_of_week();
    int day_of_month = chrono_date::day_of_month();
    vector<int> month_days = {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };
    vector<string> month_names = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
    };
    int month = chrono_date::month_number();
    month--;
    while(month < 0)
    {
        month += 12;
    }
    chrono_date::is_leap_year(year) ? (month_days[1] = 29) : (month_days[1] = 28);
    
    /*Calculate the new day of the week by
     modulating the new id by 7.*/
    day_id_of_week += newnum;
    while(day_id_of_week > 7)
    {
        day_id_of_week -= 7;
    }
    while(day_id_of_week < 1)
    {
        day_id_of_week += 7;
    }
    
    //so, now we have the new id for the day of the week
    //now for the day of the month
    
    day_of_month += newnum;
    
    //so, we are setting the date to one of previous months
    while(day_of_month < 1)
    {
        month--;
        while(month < 0)
        {
            month += 12;
            year--;
        }
        chrono_date::is_leap_year(year) ? (month_days[1] = 29) : (month_days[1] = 28);
        day_of_month += month_days[month];
    }
    while(day_of_month > month_days[month])
    {
        chrono_date::is_leap_year(year) ? (month_days[1] = 29) : (month_days[1] = 28);
        day_of_month -= month_days[month];
        month++;
        while(month > 11)
        {
            month -= 12;
            year++;
        }
    }
    
    /*Now to construct the asc string*/
    chrono_date::asc_time = (chrono_date::week_day_id_name(day_id_of_week) + " ");
    chrono_date::asc_time += (month_names[month] + " ");
    if(day_of_month < 10)
    {
        chrono_date::asc_time += "0";
    }
    chrono_date::asc_time += (conv<int, string>(day_of_month) + " 12:00:00 ");
    chrono_date::asc_time += conv<int, string>(year);
}

/* Resets the class using the no-arg constructor*/
void chrono_date::reset()
{
    chrono_date::asc_time = chrono_date::t();
}

