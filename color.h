#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED
#include <string>

namespace color
{
    
    void initialize();
    
    //highlight colors
    namespace hl
    {
        void green(const std::string&);
        void blue(const std::string&);
        void dark_green(const std::string&);
        void red(const std::string&);
    }
    
    //center-highlight colors (good for titles and stuff)
    namespace centerhl
    {
        void blue(const std::string&);
        void green(const std::string&);
    }
    
    //... screen color sets...
    namespace set
    {
        void blackwhite();
        void greenblue();
        void blue();
        void def();
    }
    
}

#endif