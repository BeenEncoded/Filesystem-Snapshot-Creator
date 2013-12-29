/**
 * Jonathan Whitlock
 * 
 * Last Modified: 06/01/2013 06:56 PM
 */

#ifndef FSYS_CLASS_H_INCLUDED
#define FSYS_CLASS_H_INCLUDED
#include <string>
#include <vector>

//return value for errors on the operation
#define FSYS_ERROR 1
#define FSYS_FAILURE 2


class fsys_class{
public:
    
    explicit fsys_class(const char*);
    explicit fsys_class(const std::string&);
    explicit fsys_class();
    
    ~fsys_class();
    
    std::string gpath();
    std::string gfilename();
    
    //functions
    bool is_file();
    bool is_folder();
    
    int move_to(const std::string&);
    int copy_to(const std::string&);
    int del();
    int create(const std::string&); //only for folders
    int rename(const std::string&);
    
    //folder only functions
    std::vector<std::string> sub_stuff();
    
    //------------------------
    //variables
    bool use_system; //in case of copy/delete/create fail, it will use a command line command if this is true
    
    
private:
    
    std::string path;
    
};


#endif // FSYS_CLASS_H_INCLUDED
