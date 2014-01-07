#include <iostream>
#include "common.h"
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include "global_defines.h"
#include "color.h"

using namespace std;

template<class type1, class type2>
inline type2 conv(const type1& t1)
{
    stringstream ss;
    type2 t2;
    ss<< t1;
    ss>> t2;
    return t2;
}

namespace
{
    
    inline bool char_match(const char& ch, const string& s)
    {
        for(auto c : s)
        {
            if(c == ch)
            {
                return true;
            }
        }
        return false;
    }
    
}


namespace common
{
    
    bool is_number(const char& ch)
    {
        return char_match(ch, NUMBERS);
    }
    
    bool is_letter(const char& ch)
    {
        return char_match(ch, LETTERS);
    }
    
    bool is_special(const char& ch)
    {
        return char_match(ch, SPECIALS);
    }
    
    bool is_char(const char& ch)
    {
        return (is_number(ch) || is_letter(ch) || is_special(ch));
    }
    
    /* Clears the screen (the RIGHT way...) */
    void cls()
    {
        HANDLE                     hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD                      count;
        DWORD                      cellCount;
        COORD                      homeCoords = { 0, 0 };

        hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
        if (hStdOut == INVALID_HANDLE_VALUE) return;

        /* Get the number of cells in the current buffer */
        if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
        cellCount = csbi.dwSize.X *csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
        if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
        )) return;

        /* Fill the entire buffer with the current colors and attributes */
        if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
            )) return;

        /* Move the cursor home */
        SetConsoleCursorPosition( hStdOut, homeCoords );
    }
    
    namespace input
    {
        
        string get_user_string(const string& message)
        {
            color::set::blackwhite();
            char ch(0), control(0);
            string inp("");
            
            while(true)
            {
                cls();
                for(unsigned int x = 0; x < 3; x++)
                {
                    cout<< endl;
                }
                cout<< ((message.size() > 0) ? message : ">> ");
                color::hl::green(inp);
                cl();
                do
                {
                    ch = getch();
                    
                    switch(is_control_key(ch))
                    {
                        //it's a control key
                        case true:
                        {
                            control = getch();
                            
                            switch(int(control))
                            {
                                case ESCAPE_KEY:
                                {
                                    return GSTRING_CANCEL;
                                }
                                break;
                                
                                default:
                                    break;
                            }
                        }
                        break;
                        
                        //ch is not a control key
                        case false:
                        {
                            switch(is_char(ch))
                            {
                                case true:
                                {
                                    inp += ch;
                                }
                                break;
                                
                                case false:
                                {
                                    switch(int(ch))
                                    {
                                        
                                        case ENTER_KEY:
                                        {
                                            return inp;
                                        }
                                        break;
                                        
                                        case BACKSPACE_KEY:
                                        {
                                            if(inp.size() > 0)
                                            {
                                                inp.resize((inp.size() - 1));
                                            }
                                        }
                                        break;
                                        
                                        default:
                                            break;
                                    }
                                }
                                break;
                                
                                default:
                                    break;
                            }
                        }
                        break;
                       
                        default:
                            break;
                    }
                }
                while(kbhit());
                
            }
            return GSTRING_CANCEL;
        }
        
        bool is_sure(const string& message)
        {
            char ch;
            while(true)
            {
                cls();
                for(short x = 0; x < 5; x++)
                {
                    cout<< endl;
                }
                center(((message.size() > 0) ? message : "Are you sure?"));
                cout<< endl;
                center("Y/N");
                cout<< endl;
                ch = gkey();
                switch(tolower(ch))
                {
                    case 'y':
                    {
                        return true;
                    }
                    break;
                    
                    case 'n':
                    {
                        return false;
                    }
                    break;
                    
                    default:
                        break;
                }
            }
            return false;
        }
        
    }
    
}