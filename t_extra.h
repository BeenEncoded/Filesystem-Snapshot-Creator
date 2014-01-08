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
    explicit chrono_date() : asc_time(this->t()), year_v(this->year()) {}
    explicit chrono_date(const string& s) : asc_time(s), year_v(this->year()) {}
    explicit chrono_date(const char* s) : asc_time(s), year_v(this->year()) {}
    
    //destructor:
    ~chrono_date()
    {
        this->asc_time.erase();
    }
    
    //handy dandy functions...
    int hour() const;
    int minute() const;
    int second() const;
    string day_of_week() const;
    int num_day_of_week() const;
    int day_of_month() const;
    int month_number() const;
    string month_name() const;
    int year() const;
    string week_day_id_name(const int&);
    string week_day_id_name();
    bool operator<(const chrono_date&);
    bool is_past();

    //some more appropriate time display functions:
    int std_hour();
    int std_minute();
    string std_ampm();
    string std_time_disp();

    //day identification
    int day_number();
    void set_day_id(const int&);
    
    void reset();
    
    //getter:
    const string& gasc_time() const;

private:
    string t();
    bool is_leap_year(const int&);
    bool is_leap_year();
    
    chronoT asc_time;
    
public:
    int year_v;
};

#endif // T_EXTRA_H_INCLUDED
