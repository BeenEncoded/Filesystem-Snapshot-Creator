#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fsysclass.h"
#include "global_defines.h"

namespace common
{
    
    inline void cl()
    {
        Sleep(10);
        while(kbhit())
        {
            getch();
        }
    }
    
    inline void wait()
    {
        for(short x = 0; x < 3; x++)
        {
            std::cout<< std::endl;
        }
        std::cout<< "Press any key to continue..."<< std::endl;
        cl();
        getch();
        cl();
    }
    
    void cls();
    bool is_number(const char&);
    bool is_letter(const char&);
    bool is_special(const char&);
    bool is_char(const char&);
    
    inline std::string lcase(const std::string& s)
    {
        std::string temps(s);
        for(std::string::iterator it = temps.begin(); it != temps.end(); it++)
        {
            *it = tolower(*it);
        }
        return temps;
    }
    
    inline bool string_is_int(const std::string& s)
    {
        if(s.size() == 0)
        {
            return false;
        }
        for(auto ch : s)
        {
            if(!is_number(ch))
            {
                return false;
            }
        }
        return true;
    }
    
    inline void center(const std::string& s)
    {
        int cent = (int(HCENTER) - (s.size() / 2));
        if(cent < 0)
        {
            cent = 0;
        }
        for(unsigned int x = 0; x < unsigned(cent); x++)
        {
            std::cout<< " ";
        }
        std::cout<< s;
    }
    
    namespace input
    {
        
        inline char gkey()
        {
            cl();
            return getch();
        }
        
        inline void ccin()
        {
            std::cin.rdbuf()->in_avail();
        }
        
        inline bool is_control_key(const char& ch)
        {
            return (int(ch) <= 0);
        }
        
        std::string get_user_string(const std::string&);
        
        bool is_sure(const std::string&);
        
    }
    
    namespace filesystem
    {
        inline unsigned int size(std::istream& in)
        {
            if(!in.good())
            {
                return 0;
            }
            unsigned int s(0);
            std::ifstream::pos_type p, temp(in.tellg());
            in.seekg(0, std::ifstream::beg);
            p = in.tellg();
            in.seekg(0, std::ifstream::end);
            s = (in.tellg() - p);
            in.seekg(temp);
            return s;
        }
        
        std::string loadline(std::istream&, const char&);
        std::string loadline(std::istream&);
        bool loadline(std::istream&, std::stringstream&, const char&);
        bool loadline(std::istream&, std::stringstream&);
    }
    
}

#endif