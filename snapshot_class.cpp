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

namespace
{    
    
    inline string loadline(istream& in, const char& delim)
    {
        stringstream s;
        string temps;
        if(in.good())
        {
            getline(in, temps, delim);
            if(!in.fail())
            {
                s<< temps;
                return s.str();
            }
        }
        s.str("");
        s<< string(GSTRING_CANCEL);
        return s.str();
    }
    
    inline string loadline(istream& in)
    {
        return loadline(in, '\n');
    }
    
    inline id_type load_id(ifstream& in)
    {
        id_type id(0);
        stringstream ss;
        char delim(DATAMEMBER_DELIM);
        
        for(short x = 0; x < 3; x++)
        {
            switch(x < 2)
            {
                case true:
                {
                    if((loadline(in, delim) == GSTRING_CANCEL) || !in.good())
                    {
                        return 0;
                    }
                }
                break;
                
                case false:
                {
                    ss.str("");
                    ss<< loadline(in, delim);
                    if(ss.str() == GSTRING_CANCEL)
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
            b = is_sure("Do you want to cancel the snapshot in progress?");
            if(!b)
            {
                display_message();
            }
            return b;
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
            temps[x]<< loadline(in, delim);
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
     the folder from which to take a snapshot of.  The default is the entire C drive. */
    snapshot_class snapshot_class::take_snapshot(const string& root) const
    {
        if(!fsys_class(root).is_folder())
        {
            return snapshot_class();
        }
        
        display_message();
        
        snapshot_class tempsnap, tempsnap2;
        pathList_type newpathlist({});
        
        //scope for boost iterator
        {
            
            if(snapshot_interrupt())
            {
                return snapshot_class();
            }
            
            boost_iter it(root);
            while(!it.at_end())
            {
                newpathlist.push_back(it.gdir());
                it++;
            }
        }
        tempsnap = snapshot_class(newpathlist, chrono_date().gasc_time());
        
        //now to assign the id:
        tempsnap.id = newid();
        /*
        tempsnap2 = *this; //save this instance temporarily
        *this = tempsnap; //because we need to modify it's private members, we need to do this...
        this->id = newid();
        tempsnap = *this;
        *this = tempsnap2;*/
        newpathlist.erase(newpathlist.begin(), newpathlist.end());
        return tempsnap;
    }
    
    /* Returns a snapshot class that contains the snapshot.  Argument passed is
     the folder from which to take a snapshot of.  The default is the entire C drive. */
    snapshot_class snapshot_class::take_snapshot() const
    {
        return this->take_snapshot("C:\\");
    }
    
    vector<id_type> load_ids()
    {
        ifstream in;
        string filename = SNAPSHOT_FILE;
        vector<id_type> ids;
        
        in.open(filename.c_str(), ios::in);
        while(in.good());
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