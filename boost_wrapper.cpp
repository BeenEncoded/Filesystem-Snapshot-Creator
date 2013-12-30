#include <iostream>
#include <boost/filesystem.hpp>
#include "boost_wrapper.h"
#include <string>
#include <sstream>
#include <exception>

using namespace std;

template<class type1, class type2>
type2 conv(const type1& t1)
{
    type2 t2 = type2();
    stringstream ss;
    ss<< t1;
    ss>> t2;
    return t2;
}


/** ***********************************************
 *         constructors                          |
 VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV*/

/**              ***OPERATORS***          */

void boost_iter::operator=(const boost_iter& b)
{
    this->valid = b.valid;
    this->it = b.it;
    this->end = b.end;
    this->parent = b.parent;
}

void boost_iter::operator=(const string& s)
{
    this->parent = s;
    this->valid = this->is_valid_path(s);
    if(this->valid)
    {
        this->it = this->from_string(s);
        this->end = BOOST_IT();
    }
    if(!(this->valid))
    {
        this->it = BOOST_IT();
        this->end = BOOST_IT();
    }
}

/* Calls the member function next() in boost_iter.
 Added for convenience. */
boost_iter boost_iter::operator++(const int i)
{
    this->next();
    return *this;
}

/* Calls the member function next() in boost_iter.
 Added for convenience.*/
boost_iter boost_iter::operator+=(const int& i)
{
    for(int x = 0; ((x < i) && !this->at_end()); x++)
    {
        this->next();
    }
    return *this;
}

boost_iter boost_iter::operator+(const int& i)
{
    for(int x = 0; ((x < i) && !this->at_end()); x++)
    {
        this->next();
    }
    return *this;
}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *        constructors                           |
 ********************************************** */







/** ***********************************************
 *          Member Functions                     |
 VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV*/

/* Moves the iterator to the next path.  Will return 1
 if it fails, or the iterator has reached the end.*/
bool boost_iter::next()
{
    if(!(this->valid))
    {
        return true;
    }
    if(!(this->at_end()))
    {
        if(boost::filesystem::is_directory(this->it->path()) && boost::filesystem::is_symlink(this->it->path()))
        {
            this->it.no_push();
        }
        try
        {
            this->it++;
        }
        catch(...)
        {
            this->it.no_push();
            try
            {
                this->it++;
            }
            catch(const exception& e)
            {
                cout<< "Boost iterator: exception thrown-> "<< e.what()<< endl;
                return 1;
            }
        }
    }
    
    //return 1 if the iterator is at the end, 0 otherwise.
    return this->at_end();
}

/* Returns true or false based on whether the 
 * iterator has reached the end.  Will return
 true for the last valid iteration as a precaution.*/
bool boost_iter::at_end() const
{
    return (this->it == this->end);
}

/* Returns the full path as a string.*/
string boost_iter::gdir() const
{
    if(this->valid)
    {
        if(!this->at_end())
        {
            return this->it->path().string();
        }
    }
    return this->parent;
}

/* Returns true if the path is a file.*/
bool boost_iter::is_file() const
{
    if(this->valid && !this->at_end())
    {
        return boost::filesystem::is_regular_file(this->it->path());
    }
    return false;
}

/* Returns true if the path is not a symlink, and is
 a directory.*/
bool boost_iter::is_folder() const
{
    if(this->valid)
    {
        return (boost::filesystem::is_directory(this->it->path()) && !boost::filesystem::is_symlink(this->it->path()));
    }
    return false;
}

/* Tries to return an extension. */
string boost_iter::extension() const
{
    if(!boost::filesystem::is_regular_file(this->it->path()) || !(this->valid))
    {
        return "";
    }
    string temps = "";
    bool dotmet = false;
    for(unsigned int x = 0; x < this->it->path().filename().string().size(); x++)
    {
        if(this->it->path().filename().string()[x] == '.')
        {
            dotmet = true;
        }
        if(dotmet)
        {
            temps += this->it->path().filename().string()[x];
        }
    }
    return temps;
}

/* ATTEMPTS TO CONSTRUCT a recursive_directory_iterator. */
BOOST_IT boost_iter::from_string(const string& s) const
{
    if(!(this->is_valid_path(s)))
    {
       return BOOST_IT();
    }
    try
    {
        BOOST_IT(boost::filesystem::path(s));
    }
    catch(const exception& e)
    {
        cout<< endl<< endl;
        cout<< "class boost_iter EXCEPTION thrown: initialization of recursive_directory_"<< 
                "iterator could not be carried out: "<< endl;
        cout<< e.what()<< endl<< endl<< endl;
        return BOOST_IT();
    }
    return BOOST_IT(boost::filesystem::path(s));
}

bool boost_iter::is_valid_path(const string& s) const
{
    boost::filesystem::path p(s);
    return ((boost::filesystem::is_directory(p) && !boost::filesystem::is_symlink(p)) || 
            boost::filesystem::is_regular_file(p));
}

/** ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *         Member Functions                      |
 ********************************************** */




