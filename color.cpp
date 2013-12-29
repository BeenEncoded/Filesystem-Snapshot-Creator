#include <iostream>
#include "color.h"
#include "concol.h"
#include <string>
#include "global_defines.h"

using namespace std;

namespace
{
    
    inline void setcol(const eku::concol& text, const eku::concol& back)
    {
        if(USECOLOR)
        {
            if((eku::textcol != text) || (eku::backcol != back))
            {
                eku::setcolor(text, back);
            }
        }
    }
    
    inline void highlight(const eku::concol& text, const eku::concol& back, const string& s)
    {
        if(USECOLOR)
        {
            eku::concol deftext(eku::textcol), defback(eku::backcol);
            setcol(text, back);
            cout<< s;
            setcol(deftext, defback);
        }
    }
    
    //center-highlight
    inline void chighlight(const eku::concol& text, const eku::concol& back, const string& s)
    {
        if(USECOLOR)
        {
            int cent = (int(HCENTER) - (s.size() / 2));
            if(cent < 0)
            {
                cent = 0;
            }
            for(unsigned int x = 0; x < unsigned(cent); x++)
            {
                cout<< " ";
            }
            highlight(text, back, s);
        }
    }
    
}


namespace color
{
    
    void initialize()
    {
        if(USECOLOR)
        {
            eku::concolinit();
        }
    }
    
    namespace hl
    {
        void green(const string& s)
        {
            highlight(eku::black, eku::green, s);
        }
        
        
    }
    
    namespace centerhl
    {
        void blue(const string& s)
        {
            chighlight(eku::dark_blue, eku::aqua, s);
        }
        
        
    }
    
    namespace set
    {
        void blackwhite()
        {
            setcol(eku::black, eku::white);
        }
        
        
    }
    
}