#ifndef BOOST_WRAPPER_H_INCLUDED
#define BOOST_WRAPPER_H_INCLUDED
#include <string>
#include <boost/filesystem.hpp>
using namespace std;

//to make things easier.... also easy to modify
#define BOOST_IT boost::filesystem::recursive_directory_iterator


/* A class to compile all of the boost iterator
 functionalities into an easy to manage class.
 all checks should be handled internally.*/
class boost_iter{
public:
    
    explicit boost_iter(const string& s) : it(this->from_string(s)), end(), 
            parent(s), valid(this->is_valid_path(s)) {}
            
    boost_iter() : it(), end(), parent({}), valid(false) {}
    
    ~boost_iter(){}
    
    //modifiers
    void operator=(const boost_iter&);
    void operator=(const string&);
    
    boost_iter operator++(const int);
    boost_iter operator+=(const int&);
    boost_iter operator+(const int&);
    bool next();
    
    //some functions
    bool at_end() const;
    string gdir() const;
    
    bool is_file() const;
    bool is_folder() const;
    string extension() const;
    
private:
    
    //functions
    BOOST_IT from_string(const string&) const;
    void set_to_this(const boost_iter&);
    bool is_valid_path(const string&) const;
    
    //variables
    BOOST_IT it, end;
    string parent;
    bool valid;
};


#endif