#include <iostream>
#include "common.h"
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include "fsysclass.h"
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
            char ch(0);
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
    
    namespace filesystem
    {
        string loadline(istream& in, const char& delim)
        {
            common::input::ccin();
            stringstream s;
            string temps;
            if(in.good())
            {
                getline(in, temps, delim);
                
                //if the getlin() function left the deliminator in the stream, discard it.
                if(char(in.peek()) == delim)
                {
                    char *ch(new char());
                    in.get(*ch);
                    delete ch;
                }
                
                //check for failure:
                if(!in.fail() || (temps.size() == 0))
                {
                    s<< temps;
                    return s.str();
                }
            }
            s.str("");
            s<< string(GSTRING_CANCEL);
            return s.str();
        }
        
        string loadline(istream& in)
        {
            return loadline(in, '\n');
        }
        
        bool loadline(istream& in, stringstream& ss, const char& delim)
        {
            ss.str("");
            if(!in.good())
            {
                return false;
            }
            char ch;
            bool temp_b(false);
            while(in.good() && (char(in.peek()) != delim) && (char(in.peek()) != EOF))
            {
                in.get(ch);
                ss<< ch;
                if(!temp_b) temp_b = true;
            }
            
            //discard the deliminator in the stream
            if(char(in.peek()) == delim)
            {
                in.get(ch);
            }
            return temp_b;
        }
        
        bool loadline(istream& in, stringstream& ss)
        {
            return loadline(in, ss, '\n');
        }
        
        /** Returns the drive directory (aka "C:\\", or whatever the drive
         is)*/
        string retrieve_root_dir()
        {
            string temps(fsys_class().gpath());
            if(temps.size() > string("A:\\").size()) temps.resize(string("A:\\").size());
            return temps;
        }
    }
}