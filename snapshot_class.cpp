#include <iostream>
#include "snapshot_class.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "common.h"
#include "boost_wrapper.h"
#include "fsysclass.h"
#include "t_extra.h"
#include <conio.h>
#include "global_defines.h"

using namespace std;

//  delimiters
#define STRING_DELIM 1
#define DATAMEMBER_DELIM 2

template<class type1, class type2>
inline type2 conv(const type1& t1)
{
    type2 t2;
    stringstream ss;
    ss<< t1;
    ss>> t2;
    return t2;
}

namespace
{
    inline id_type load_id(ifstream& in)
    {
        id_type id(0);
        stringstream ss;
        char delim(DATAMEMBER_DELIM);
        
        for(short x = 0; ((x < 3) && in.good()); x++)
        {
            switch(x < 2)
            {
                case true:
                {
                    if(!common::filesystem::loadline(in, ss, delim) || !in.good())
                    {
                        return 0;
                    }
                    ss.str("");
                }
                break;
                
                case false:
                {
                    if(!common::filesystem::loadline(in, ss, delim))
                    {
                        return 0;
                    }
                    return ((ss>> id), id);
                }
                break;
                
                default:
                    break;
            }
        }
        return id;
    }
    
    // Functions emplemented during a snapshot:
    
    inline void display_message()
    {
        using namespace common;
        cls();
        for(short x = 0; x < 6; x++)
        {
            cout<< endl;
        }
        center("Snapshot in progress, please wait...  Press any key to cancel");
    }
    
    inline bool snapshot_interrupt()
    {
        using namespace common;
        bool b;
        if(kbhit())
        {
            cl();
            b = input::is_sure("Do you want to cancel the snapshot in progress?");
            if(!b)
            {
                //if the user decides not to cancel, it should re-display the message.
                display_message();
            }
            return b;
        }
        return false;
    }
    
    /* Thoroushly checks to make sure the string is a drive 
     letter.*/
    inline bool is_drive_letter(const string& s)
    {
        if(s.size() == string("A:\\").size())
        {
            for(char ch = 'a'; ch < 'z'; ch++)
            {
                if(tolower(s[0]) == ch)
                {
                    return bool(string(conv<char, string>(ch) + ":\\") == common::lcase(s));
                }
            }
        }
        return false;
    }
    
    //-----------------------------------------
    
    
}

namespace snapshot
{
    
    // pathList_type operators:
    ostream& operator<<(ostream& out, pathList_type paths)
    {
        for(auto s : paths) out<< s<< char(STRING_DELIM);
        return out;
    }
    
    istream& operator>>(istream& in, pathList_type paths)
    {
        paths.erase(paths.begin(), paths.end());
        string temps("");
        while(in.good())
        {
            getline(in, temps, char(STRING_DELIM));
            if(temps.size() > 0)
            {
                paths.push_back(temps);
            }
        }
        return in;
    }
    
    /** Snapshot_class file stream operators: */
    istream& operator>>(istream& in, snapshot_class& snap)
    {
        return snap.in(in);
    }
    
    ostream& operator<<(ostream& out, snapshot_class& snap)
    {
        return snap.out(out);
    }
    
    
    
    /** Snapshot_class member functions: */
    
    ostream& snapshot_class::out(ostream& out) const
    {
        
        /* if an ID is 0, then it's invalid.  We should never ever
         save an invalid id!! */
        if(this->id == 0)
        {
            throw "C++ exception: \"ostream& snapshot_class::out(ostream& out) const\" INVALID ID value";
        }
        
        char delim(DATAMEMBER_DELIM);
        out<< this->path_list<< delim;
        out<< this->timestamp<< delim;
        out<< this->id<< delim;
        return out;
    }
    
    istream& snapshot_class::in(istream& in)
    {
        this->clear();
        char delim(DATAMEMBER_DELIM);
        stringstream temps[3];
        
        for(short x = 0; x < 3; x++)
        {
            common::filesystem::loadline(in, temps[x], delim);
            if(temps[x].str() == GSTRING_CANCEL)
            {
                return in;
            }
        }
        
        temps[0]>> pathList_type(this->path_list);
        temps[1]>> this->timestamp;
        temps[2]>> this->id;
        
        for(short x = 0; x < 3; x++)
        {
            temps[x].str("");
        }
        
        return in;
    }
    
    /* Returns a snapshot class that contains the snapshot.  Argument passed is
     the folder from which to take a snapshot of.  The default is the entire C drive. 
     this is also when the object is assigned an ID.*/
    void snapshot_class::take_snapshot(const string& root)
    {
        if(!fsys_class(root).is_folder())
        {
            return;
        }
        this->clear();
        
        display_message();
        
        //scope for boost iterator
        {
            boost_iter it(root);
            if(!it.at_end()) it++;
            while(!it.at_end())
            {
                //check to see if the user wants to cancel. (button is pressed)
                if(snapshot_interrupt())
                {
                    return;
                }
                this->path_list.push_back(it.gdir());
                it++;
            }
        }
        this->timestamp = chrono_date().gasc_time();
        this->id = newid();
    }
    
    /* Returns a snapshot class that contains the snapshot.  Argument passed is
     the folder from which to take a snapshot of.  The default is the entire C drive. */
    void snapshot_class::take_snapshot()
    {
        this->take_snapshot(string("C:\\"));
    }
    
    vector<id_type> load_ids()
    {
        ifstream in;
        string filename(SNAPSHOT_FILE);
        if(!fsys_class(filename).is_file())
        {
            return vector<id_type>();
        }
        vector<id_type> ids;
        in.open(filename.c_str(), ios::INFILE);
        if(common::filesystem::size(in) == 0)
        {
            return vector<id_type>();
        }
        while(in.good())
        {
            ids.push_back(id_type());
            ids.back() = load_id(in);
            if(ids.back() == 0)
            {
                ids.pop_back();
            }
        }
        in.close();
        return ids;
    }
    
}