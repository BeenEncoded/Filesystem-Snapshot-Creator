/*
 * Jonathan Whitlock
 *
 */

#ifndef T_EXTRA_H_INCLUDED
#define T_EXTRA_H_INCLUDED
#include <string>
#include <vector>
using namespace std;

//type to represent the time
typedef string chronoT;

/**Note: Only variables can be used to set static defaults*/

class chrono_date{
public:

    //constructors:
    chrono_date();
    chrono_date(const string&);
    chrono_date(const char*);
    
    //destructor:
    ~chrono_date();
    
    //handy dandy functions...
    int hour();
    int minute();
    int second();
    string day_of_week();
    int num_day_of_week();
    int day_of_month();
    int month_number();
    string month_name();
    int year();
    string week_day_id_name(const int&);
    string week_day_id_name();
    bool is_past(const string&);
    bool is_past();

    //some more appropriate time display functions:
    int std_hour();
    int std_minute();
    string std_ampm();
    string std_time_disp();

    //day identification
    int day_number();
    void set_day_id(const int&);

    int year_v;
    
    void reset();
    
    //getter:
    const string gasc_time();

private:
    string t();
    bool is_leap_year(const int&);
    bool is_leap_year();
    
    chronoT asc_time;

};

#endif // T_EXTRA_H_INCLUDED
