#ifndef GLOBAL_VARS_HPP_INCLUDED
#define GLOBAL_VARS_HPP_INCLUDED
#include <string>
#include <iostream>
#include "fsysclass.h"

namespace common
{
    namespace filesystem
    {
        std::string retrieve_root_dir();
    }
}

class settings;

std::ostream& operator<<(std::ostream&, settings&);
std::istream& operator>>(std::istream&, settings&);

class settings
{
public:
    explicit settings(){}

    ~settings(){}

    //folder to save reports in
    static std::string report_folder;

    //whether or not to use color for the program
    static bool use_color;

    //folder that a snapshot will be taken of
    static std::string target_folder;
};

#endif