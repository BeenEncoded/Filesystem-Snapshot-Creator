/**
 * Jonathan Whitlock
 * 
 * Last Modified: 06/01/2013 06:56 PM
 */


#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include "fsysclass.h"
#include <fstream>
#include <windows.h>
#include <exception>

using namespace std;

/**Debug definition for debug mode*/
#ifdef DEBUG
#define DEBUG
#endif



//==================================================================


//include common.h if debug mode is on, and common.h is not included
#ifdef DEBUG
#ifndef COMMON_H_INCLUDED
#include "common.h"
#endif
#endif


//============================================
//prototypes
bool is_file_supliment(const string&);
bool is_folder_supliment(const string&);
vector<string> sub_stuff_suppliment(const string&);
int copy(const string&, const string&);
int delete_path(const string&, const bool&);
int delete_file_sup(const string&, const bool&);
int delete_folder_sup(const string&, const bool&);
int copy_file_sup(const string&, const string&, const bool&);
int copy_folder_sup(const string&, const string&, const bool&);
string extract_target_folders(const string&, const string&);
string parent(const string&);
void rstring(string&);
bool is_valid_filename(const string&);
bool is_valid_foldername_char(const char&);
void get_all_directories(const string&);
string current_directory();
string filename_sup(const string&);
int create_dir(const string&, const string&);






/* This function is littered with safety precautions so that we
 don't fail.  Useing the system command line is an option, and only available 
 as a last resort, and only if use_sys == true*/
int delete_file_sup(const string& s, const bool& use_sys)
{
    boost::system::error_code err;
    boost::filesystem::path p = s;
    bool sys = use_sys;
    string command = ("DEL /F /Q \"" + s + "\" >nul 2>nul");
    
    //if the path is a file
    if(boost::filesystem::is_regular_file(p, err))
    {
        try
        {
            remove(s.c_str());
        }
        catch(const exception& e)
        {
            cout<< e.what()<< endl;
            if(boost::filesystem::is_regular_file(p, err) && sys)
            {
                system(command.c_str());
                if(boost::filesystem::is_regular_file(p, err))
                {
                    return FSYS_ERROR;
                }
            }
            if(!sys)
            {
                return FSYS_ERROR;
            }
        }
    }
    
    //just being fool-proof here...
    if(!boost::filesystem::is_regular_file(p, err))
    {
        return 0;
    }
    return FSYS_FAILURE;
}

/* This function is littered with safety precautions so that we
 don't fail.  Useing the system command line is an option, and only available 
 as a last resort, and only if use_sys == true
 
 Deletes the files, then the folders.  It does not procede if the path passed
 is a file, or a folder-symlink*/
int delete_folder_sup(const string& s, const bool& use_sys)
{
    string command = ("RD /S /Q \"" + s + "\"");
    boost::filesystem::path p = s;
    //using the windows API RemoveDirectory()
    if(is_folder_supliment(s) == true)
    {
        try
        {
            boost::filesystem::remove_all(p);
        }
        catch(const exception& e)
        {
            cout<< e.what()<< endl;
        }
        /* Now, if it does not return an exception, we still want this to execute
         in the event of a failure*/
        
        //try the command line if the user wants to resort to it
        if((use_sys == true) && (is_folder_supliment(s) == true))
        {
            system(command.c_str());
            if(is_folder_supliment(s) == true)
            {
                return FSYS_FAILURE;
            }
        }
    }
    if(is_folder_supliment(s) == false)
    {
        return 0;
    }
    return FSYS_FAILURE;
}

/* forcibly attempts to delete any type of path; be it folder, or file.
 if it can't it will return 1 (FSYS_FAIL).  There are a lot of fail-safes,
 and safety measures.  You can even set use_sys to true and it
 will resort to using a system() command to remove the file/folder*/
int delete_path(const string& s, const bool& use_sys)
{
    bool sys = use_sys;
    int f = 0;
    if((sys != true) && (sys != false))
    {
        sys = false;
    }
    
    //function checks for path validation, so we don;t need to here
    f = delete_folder_sup(s, use_sys);
    if(f == FSYS_ERROR)
    {
        return FSYS_ERROR;
    }
    if(f == FSYS_FAILURE)
    {
        return FSYS_FAILURE;
    }
    
    //function checks for path validation, so we don't need to here
    f = delete_file_sup(s, use_sys);
    if(f == FSYS_ERROR)
    {
        return FSYS_ERROR;
    }
    if(f == FSYS_FAILURE)
    {
        return FSYS_FAILURE;
    }
    
    return 0;
}

int copy_file_sup(const string& sfrom, const string& sto, const bool& use_sys)
{
    boost::system::error_code err;

    //if the target is not a folder, then we can't copy anything!
    if((is_file_supliment(sfrom) == false) || (is_folder_supliment(sto) == false))
    {
        return 0;
    }
    boost::filesystem::path from = sfrom, to = sto;
    string newpath = (sto + "\\" + from.filename().string());
    to = newpath;
    if(is_file_supliment(newpath))
    {
        if(delete_path(newpath, use_sys) == 1)
        {
            return FSYS_ERROR;
        }
    }
    if(!is_file_supliment(newpath) && boost::filesystem::is_regular_file(from, err))
    {
        try
        {
            boost::filesystem::copy_file(from, to);
        }
        catch(const exception& e)
        {
            cout<< e.what()<< endl;
            if(!boost::filesystem::is_regular_file(to) && boost::filesystem::is_regular_file(from))
            {
                try
                {
                    boost::filesystem::copy_file(from, to);
                }
                catch(...)
                {
                    return FSYS_ERROR;
                }
            }
        }
    }
    
    //make sure the copy succeeded before returning a success value
    if(boost::filesystem::is_regular_file(to, err))
    {
        return 0;
    }
    return FSYS_FAILURE;
}

/* This function extract sub directories from a given parent:
 Example:
 
  from = C:\users\username\desktop
  to = C:\users\username\desktop\C++ projects\thisproject
 
   Return value:  C++ projects\thisproject
   This is so that when copying folders, we can be assured that the 
 folder tree is preserved.*/
string extract_target_folders(const string& from, const string& to)
{
    if(from.size() >= to.size())
    {
        return to;
    }
    string temps = "";
    for(unsigned int x = from.size(); x < to.size(); x++)
    {
        temps += to[x];
    }
    return temps;
}

//reverses a string  EG "my string" to "gnirts ym"
void rstring(string& s)
{
    if(s.size() == 0)
    {
        return;
    }
    string temps = s;
    s.clear();
    for(unsigned int x = (temps.size() - 1); x >= 0; x--)
    {
        s += temps[x];
        if(x == 0)
        {
            break;
        }
    }
}

//returns the parent folder of a path eg: C:\files\myfiles to C:\files
string parent(const string& s)
{
    string path = s, temps = "";
    bool slashmet = false;
    rstring(path);
    for(unsigned int x = 0; x < path.size(); x++)
    {
        if(slashmet == true)
        {
            temps += path[x];
        }
        if((slashmet == false) && (path[x] == '\\'))
        {
            slashmet = true;
        }
    }
    rstring(temps);
    return temps;
}

/* Copies a folder.*/
int copy_folder_sup(const string& sfrom, const string& sto, const bool& use_sys)
{
    
    //if the target is not a folder, then we can't copy anything!
    if(!is_folder_supliment(sfrom) || !is_folder_supliment(sto))
    {
        return 0;
    }
    boost::filesystem::path from = sfrom, to, temp;
    boost::filesystem::recursive_directory_iterator it(from), end;
    boost::system::error_code err;
    
    //create a string representing our folder, under the to folder
    to = (sto + "\\" + from.filename().string());
    
    /* if the from folder exists, and the to folder does not, then copy from, to to.*/
    if(is_folder_supliment(sto) && is_folder_supliment(sfrom))
    {
        
        //lets copy the folders first
        
        //create the root folder if it does not exist in the target
        if(!boost::filesystem::is_directory(to))
        {
            boost::filesystem::create_directories(to, err);
            if(!boost::filesystem::is_directory(to, err))
            {
                return FSYS_ERROR;
            }
        }
        
        //iterate through the folders, and copy them
        while(it != end)
        {
            //if the path is a folder...
            if(boost::filesystem::is_directory(*it, err) == true)
            {
                //constructing paths into to
                temp = (to.string() + extract_target_folders(from.string(), it->path().string()));
                
                /* Attempt the create the folders in the target, and if it fails,
                 try once more.*/
                boost::filesystem::create_directories(temp, err);
                
                //new scope:
                {
                    
                    //temporary bool.
                    bool no_error = (!err);
                    
                    if(!no_error)
                    {
                        cout<< err.message()<< endl;
                        err.clear();
                        if(!boost::filesystem::is_directory(temp, err))
                        {
                            boost::filesystem::create_directories(temp, err);
                            no_error = (!err);
                            if(!no_error)
                            {
                                return FSYS_ERROR;
                            }
                            err.clear();
                        }
                    }
                }
                
            }
            if(boost::filesystem::is_directory(*it) && boost::filesystem::is_symlink(*it))
            {
                it.no_push();
            }
            
            try
            {
                ++it;
            }
            catch(const exception& e)
            {
                cout<< e.what()<< endl;
                it.no_push();
                try
                {
                    ++it;
                }
                catch(...)
                {
                    return FSYS_ERROR;
                }
            }
        }
        
        err.clear();
        
        //now, lets copy the files
        it = boost::filesystem::recursive_directory_iterator(from, err);
        bool no_error(!err);
        
        if(!no_error)
        {
            cout<< err.message()<< endl;
            return FSYS_ERROR;
        }
        
        while(it != end)
        {
            /* Is the current path a file?*/
            if(boost::filesystem::is_regular_file(*it, err) == true)
            {
                no_error = (!err);
                if(!no_error)
                {
                    cout<< err.message()<< endl;
                    return FSYS_ERROR;
                }
                
                
                /* Construct a string representing the filename, under the folders
                 leading up to the root.*/
                temp = (to.string() + "\\" + extract_target_folders(from.string(), (parent(it->path().string()) + "\\" + it->path().filename().string())));
                
                boost::filesystem::copy_file(it->path(), temp, err);
                no_error = (!err);
                
                /* Try again, and if it fails then give up.*/
                if(!no_error)
                {
                    cout<< err.message()<< endl;
                    if(!boost::filesystem::is_regular_file(temp, err))
                    {
                        boost::filesystem::copy_file(it->path(), temp, err);
                        no_error = (!err);
                        if(!no_error)
                        {
                            return FSYS_ERROR;
                        }
                    }
                }
            }
            if((boost::filesystem::is_directory(*it, err) == true) && (boost::filesystem::is_symlink(*it, err) == true))
            {
                it.no_push();
            }
            try
            {
                ++it;
            }
            catch(const exception& e)
            {
                cout<< e.what()<< endl;
                it.no_push();
                try
                {
                    ++it;
                }
                catch(...)
                {
                    return FSYS_ERROR;
                }
            }
        }
        
        /* This checks to see if the contents of the folder, as well as the folder, was copied
         or not.  If the operation failed, we should not return 0.*/
        if((boost::filesystem::is_directory(to, err) == true) && (boost::filesystem::is_empty(to, err) == boost::filesystem::is_empty(from, err)))
        {
            return 0;
        }
    }
    return FSYS_FAILURE;
}

/* Copys path from, to path to.*/
int copy(const string& sfrom, const string& sto, const bool& use_sys)
{
    /* copy_folder_sup and copy_file_sup contain necessary checks, so
     we do not need them here.*/
    int tempi = 0;
    tempi = copy_folder_sup(sfrom, sto, use_sys);
    if(tempi == FSYS_ERROR)
    {
        return FSYS_ERROR;
    }
    if(tempi == FSYS_FAILURE)
    {
        return FSYS_FAILURE;
    }
    
    tempi = copy_file_sup(sfrom, sto, use_sys);
    if(tempi == FSYS_ERROR)
    {
        return FSYS_ERROR;
    }
    if(tempi == FSYS_FAILURE)
    {
        return FSYS_FAILURE;
    }
    return 0;
}

/*  
 * ===================================================
 Constructors
 
 */

fsys_class::fsys_class()
{
    fsys_class::use_system = false;
    fsys_class::path = current_directory();
}

fsys_class::fsys_class(const string& p)
{
    fsys_class::path = p;
    fsys_class::use_system = false;
}

fsys_class::~fsys_class()
{
}

fsys_class::fsys_class(const char* ch)
{
    string s = string(ch);
    fsys_class::path = s;
    fsys_class::use_system = false;
}

/*  
 * ==================================================
 * 
 * 
 */

/* Keeps boost out of the fsys class*/
bool is_file_supliment(const string& path)
{
    using namespace boost;
    filesystem::path p = path;
    system::error_code err;
    return filesystem::is_regular_file(p, err);
}

bool is_folder_supliment(const string& path)
{
    boost::system::error_code err;
    boost::filesystem::path p(path);
    bool b;
    try
    {
        b = boost::filesystem::is_directory(p, err);
    }
    catch(const exception& e)
    {
        cout<< e.what();
        return false;
    }
    return b;
}

bool fsys_class::is_file()
{
    return is_file_supliment(fsys_class::path);
}

bool fsys_class::is_folder()
{
    return is_folder_supliment(this->path);
}

vector<string> sub_stuff_suppliment(const string& parent)
{
    vector<string> directories = vector<string>();
    boost::filesystem::path p = parent;
    if(boost::filesystem::is_directory(p) == false)
    {
        return vector<string>();
    }
    if((boost::filesystem::is_directory(p) == true) && (boost::filesystem::is_symlink(p) == true))
    {
        return vector<string>();
    }
    boost::filesystem::directory_iterator it = boost::filesystem::directory_iterator(p);
    boost::filesystem::directory_iterator end;
    while(it != end)
    {
        directories.push_back(it->path().string());
        try
        {
            ++it;
        }
        catch(const exception& e)
        {
            cout<< e.what()<< endl;
            return directories;
        }
    }
    return directories;
}

vector<string> fsys_class::sub_stuff()
{
    if(fsys_class::is_folder() == false)
    {
        return vector<string>();
    }
    return sub_stuff_suppliment(fsys_class::path);
}

/* Copies a file or folder to 'to'.
 
 Returns 1 if fails.*/
int fsys_class::copy_to(const string& to)
{
    return copy(fsys_class::path, to, fsys_class::use_system);
}

/* Deletes a path that the class is set to.
 
 Returns 1 if fails*/
int fsys_class::del()
{
    return delete_path(fsys_class::path, fsys_class::use_system);
}

/* Moves a file or folder.
 
 Returns 1 if fails.*/
int fsys_class::move_to(const string& to)
{
    if(copy(fsys_class::path, to, fsys_class::use_system) == 1)
    {
        return 1;
    }
    if(delete_path(fsys_class::path, fsys_class::use_system) == 1)
    {
        return 1;
    }
    if((is_file_supliment(fsys_class::path) == false) && (is_folder_supliment(fsys_class::path) == false))
    {
        return 0;
    }
    return FSYS_FAILURE;
}

bool is_valid_foldername_char(const char& c)
{
    vector<char> invalids = {
        '\\',
        '|',
        '/',
        ':',
        '*',
        '\"',
        '?',
        '<',
        '>'
    };
    for(vector<char>::const_iterator it = invalids.begin(); it != invalids.end(); ++it)
    {
        if(*it == c)
        {
            return false;
        }
    }
    return true;
}

/* Checks to see whether a folder path has any bad characters
 in it.
 
 Converts a folder path to the folder's name.*/
bool is_valid_filename(const string& s)
{
    if(s.size() == 0)
    {
        return false;
    }
    boost::filesystem::path p = s;
    string temps = p.filename().string();
    for(string::const_iterator it = temps.begin(); it != temps.end(); ++it)
    {
        if(isalpha(*it) == false)
        {
            if(is_valid_foldername_char(*it) == false)
            {
                return false;
            }
        }
    }
    return true;
}

/* Renames a path.*/
int fsys_class::rename(const string& s)
{
    string temps = (parent(fsys_class::path) + "\\" + s);
    if(is_valid_filename(s) == false)
    {
        cout<< "ERROR: Invalid Filename"<< endl;
        return FSYS_ERROR;
    }
    try
    {
        std::rename(fsys_class::path.c_str(), temps.c_str());
    }
    catch(const exception& e)
    {
        cout<< e.what()<< endl;
        return FSYS_ERROR;
    }
    if((is_folder_supliment(temps) == true) || (is_file_supliment(temps) == true))
    {
        return 0;
    }
    return FSYS_FAILURE;
}

string current_directory()
{
    return boost::filesystem::current_path().string();
}

string fsys_class::gpath()
{
    return fsys_class::path;
}

string filename_sup(const string& s)
{
    boost::filesystem::path p = s;
    return p.filename().string();
}

/* Returns the filename of the path.*/
string fsys_class::gfilename()
{
    return filename_sup(fsys_class::path);
}

int create_dir(const string& parent, const string& foldername)
{
    boost::filesystem::path p = parent;
    p = (p.string() + "\\" + foldername);
    if(boost::filesystem::is_directory(p) == false)
    {
        try
        {
            boost::filesystem::create_directories(p);
        }
        catch(const exception& e)
        {
            cout<< e.what()<< endl;
            return FSYS_ERROR;
        }
        if(boost::filesystem::is_directory(p) == false)
        {
            return FSYS_FAILURE;
        }
    }
    return 0;
}

/* Creates a folder under the set path with the argument name.
 
   fsys_class::create("Jon stuff") will create a folder under
 the fsys_class::path if fsys_class::path is a folder.*/
int fsys_class::create(const string& sub_folder)
{
    if(is_folder_supliment(fsys_class::path) == false)
    {
        return FSYS_ERROR;
    }
    return create_dir(fsys_class::path, sub_folder);
}

