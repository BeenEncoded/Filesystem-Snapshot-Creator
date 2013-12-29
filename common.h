#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>
#include "global_defines.h"

namespace common
{
    
    inline void wait()
    {
        for(short x = 0; x < 3; x++)
        {
            std::cout<< std::endl;
        }
        std::cout<< "Press any key to continue..."<< std::endl;
    }
    
    inline void cl()
    {
        Sleep(10);
        while(kbhit())
        {
            getch();
        }
    }
    
    void cls();
    bool is_number(const char&);
    bool is_letter(const char&);
    bool is_special(const char&);
    bool is_char(const char&);
    
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
        
    }
    
}

#endif