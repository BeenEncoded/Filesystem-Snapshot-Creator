#include <iostream>
#include "global_vars.hpp"
#include <string>
#include <sstream>

using namespace std;

#define SETTINGS_DELIM 2

std::string settings::report_folder = fsys_class().gpath();
std::string settings::target_folder = common::filesystem::retrieve_root_dir();
bool settings::use_color = true;


namespace
{
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        stringstream *ss(new stringstream());
        type2 t2;
        (*ss)<< t1;
        (*ss)>> t2;
        delete ss;
        return t2;
    }
}

ostream& operator<<(ostream& out, settings& s)
{
    char delim(SETTINGS_DELIM);
    out<< settings::report_folder<< delim;
    out<< settings::target_folder<< delim;
    out<< settings::use_color<< delim;
    return out;
}

istream& operator>>(istream& in, settings& s)
{
    char delim(SETTINGS_DELIM);
    if(!in.good())
    {
        return in;
    }
    
    string *temps(new string(""));
    getline(in, *temps, delim);
    
    if(!in.fail())
    {
        settings::report_folder = *temps;
    }
    
    if(in.good())
    {
        getline(in, *temps, delim);
        if(!in.fail())
        {
            settings::target_folder = *temps;
        }
    }
    
    if(in.good())
    {
        getline(in, *temps, delim);
        if(!in.fail())
        {
            settings::use_color = conv<string, bool>(*temps);
        }
    }
    return in;
}

