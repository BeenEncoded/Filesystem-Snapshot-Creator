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

using namespace std;

//  delimiters
#define STRING_DELIM 1
#define DATAMEMBER_DELIM 2

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
        char delim(DATAMEMBER_DELIM);
        out<< this->path_list<< delim;
        out<< this->timestamp<< delim;
        return out;
    }
    
    istream& snapshot_class::in(istream& in)
    {
        stringstream ss;
        char delim(DATAMEMBER_DELIM);
        string temps("");
        
        /* retrieve the path_list */
        getline(in, temps, delim);
        if(!in.fail()) //we don't want to do anything if no delimiter was found.
        {
            ss<< temps;
            ss>> pathList_type(this->path_list);
            ss.str("");
        }
        
        /* Retrieve the timestamp. */
        getline(in, temps, delim);
        if(!in.fail())
        {
            ss<< temps;
            ss>> this->timestamp;
            ss.str("");
        }
        
        temps.erase();
        return in;
    }
    
    bool snapshot_class::take_snapshot(const string& root) const
    {
        if(!fsys_class(root).is_folder())
        {
            return false;
        }
        string filename(SNAPSHOT_FILE);
        snapshot_class tempsnap;
        ofstream out;
        pathList_type newpathlist({});
        
        //scope for boost iterator
        {
            boost_iter it(root);
            while(!it.at_end())
            {
                newpathlist.push_back(it.gdir());
                it++;
            }
        }
        if(newpathlist.size() == 0)
        {
            return false;
        }
        tempsnap = snapshot_class(newpathlist, chrono_date().gasc_time());
        newpathlist.erase(newpathlist.begin(), newpathlist.end());
        out.open(filename.c_str(), ios::app);
        out<< tempsnap<< endl;
        out.close();
        
        filename.erase();
        return true;
    }
    
}