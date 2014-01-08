/* This program is written by Jonathan Whitlock.
 * 
 * For terms of use, please refer to the GNU General Public Liscense.
 */

#include <iostream>
#include "snapshot_class.h"
#include "common.h"
#include "color.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "global_defines.h"
#include "vector_display_buffer.h"
#include "t_extra.h"
#include <exception>
#include <windows.h>

using namespace std;

/* @TODO
 * 
 * -  Start on snapshotList_class object
 * 
 */


namespace
{
    template<class type1, class type2>
    inline type2 conv(const type1& t1)
    {
        type2 t2;
        stringstream ss;
        ss<< t1;
        ss>> t2;
        return t2;
    }
    
    /* Specializes in numbers */
    template<class type>
    inline string format_number(const type& t)
    {
        stringstream ss;
        string temps, newtemps;
        
        ss<< t;
        ss>> temps;
        ss.str("");
        if(temps.size() == 0) return "";
        
        //remove signs, we don't want them.
        if(temps[0] == '-') temps.erase(temps.begin());
        
        //remove any decimals... those need to go.
        for(char c : temps)
        {
            if(c == '.')
            {
                while(temps.back() != '.') temps.erase((temps.begin() + (temps.size() - 1)));
                temps.erase((temps.begin() + (temps.size() - 1)));
                break;
            }
        }
        
        for(unsigned int x = 0; x < temps.size(); x++)
        {
            if(((x + 1) % 3) == 0) newtemps += ',';
            newtemps += temps[(temps.size() - (x + 1))];
        }
        temps.erase();
        return newtemps;
    }
}


/* Some extra functions that would be better here than 
  with the class. */
namespace snapshot
{
    
    /* Sorts snapshot information by date: from most recent (snaps.begin()) to
     the least recent (snaps.end())*/
    inline void sort_basic_data(vector<basic_snapshot_data>& snaps)
    {
        if(snaps.size() <= 1)
        {
            if(snaps.size() == 1)
            {
                return;
            }
            throw string("inline basic_snapshot_data rec_sort_basic_data(vector<basic_snapshot_data>& snaps) ::: ERROR:" + 
                    string(" Sort error resulted in over-passing the end of the vector!"));
        }
        basic_snapshot_data tempsnap;
        
        if(snaps.size() > 1)
        {
            tempsnap = snaps[0];
            
            /* Get the most recent snapdata*/
            for(unsigned int x = 1; x < snaps.size(); x++)
            {
                if(chrono_date(snaps[x].t) < chrono_date(tempsnap.t))
                {
                    tempsnap = snaps[x];
                }
            }
            
            //recurse!
            sort_basic_data(snaps);
            snaps.insert(snaps.begin(), tempsnap);
        }
    }
    
    /* This loads a vector containing basic snapshot information. Good when
     we have 200,000 strings and we don't want to load 10 * 200,000 strings... 
     * at once... (2,000,000)*/
    inline vector<basic_snapshot_data> load_basic_snapshot_data()
    {
        vector<basic_snapshot_data> snaps;
        ifstream in;
        stringstream ss;
        string temps(""), filename(SNAPSHOT_FILE);
        
        in.open(filename.c_str(), ios::INFILE);
        while(in.good())
        {
            ss.str("");
            getline(in, temps);
            if(temps.size() == 0)
            {
                continue;
            }
            for(string::iterator it = temps.begin(); it != temps.end();)
            {
                ss<< *it;
                it = temps.erase(it);
            }
            snaps.push_back(basic_snapshot_data());
            snaps.back().t = chrono_date().gasc_time();
            snaps.back().id = 0;
            snaps.back().pathcount = 0;
            snapshot_class::load_basic(ss, snaps.back());
            if(snaps.back().id == 0)
            {
                snaps.pop_back();
            }
        }
        return snaps;
    }
    
    /* Loads a snapshot of the specified id */
    inline snapshot_class from_id(const id_type& id)
    {
        string filename(SNAPSHOT_FILE);
        if(!fsys_class(filename).is_file())
        {
            return snapshot_class();
        }
        ifstream in;
        string temps("");
        stringstream ss; //for good measure.
        snapshot_class snap;
        basic_snapshot_data basics;
        
        in.open(filename.c_str(), ios::INFILE);
        while(in.good())
        {
            ss.str("");
            getline(in, temps);
            if(temps.size() == 0)
            {
                continue;
            }
            
            //we want to be memory efficient here, we could be copying a LOT of characters:
            for(string::iterator it = temps.begin(); it != temps.end();)
            {
                ss<< *it;
                it = temps.erase(it);
            }
            snapshot_class::load_basic(ss, basics);
            if(basics.id == id)
            {
                ss.seekg(0, stringstream::beg);
                ss>> snap;
                
                //erase the vars; for good measure
                ss.str("");
                in.close();
                temps.erase();
                return snap;
            }
        }
        in.close();
        ss.str("");
        temps.erase();
        return snapshot_class();
    }
}

/* it takes a snapshot, and then saves it. */
inline void take_snapshot()
{
    using namespace snapshot;
    using namespace common;
    color::set::greenblue();
    
    snapshot_class snap;
    bool b(false);
    
    snap = snap.take_snapshot();
    if(snapshot_class::is_valid(snap))
    {
        ofstream out;
        ifstream in;
        in.open(string(SNAPSHOT_FILE).c_str(), ios::INFILE);
        b = (filesystem::size(in) > 0);
        in.close();
        out.open(string(SNAPSHOT_FILE).c_str(), ios::app);
        if(b)
        {
            out<< endl;
        }
        out<< snap;
        out.close();
    }
}

inline void manage_snapshots()
{
    using namespace common;
    color::set::blackwhite();
    
    char ch, control;
    displayBuffer::list_buffer_class buf;
    vector<snapshot::basic_snapshot_data> snaps(snapshot::load_basic_snapshot_data());
    vector<string> display;
    
    try
    {
        snapshot::sort_basic_data(snaps);
    }
    catch(const exception& e)
    {
        cout<< e.what();
        wait();
        abort();
    }
    
    for(unsigned int x = 0; x < snaps.size(); x++)
    {
        display.push_back(chrono_date(snaps[x].t).std_time_disp());
        display.back() += ("  " + format_number(snaps[x].pathcount) + 
                string(" paths  "));
        display.back() += ("(" + conv<id_type, string>(snaps[x].id) + ")");
    }
    buf.set_buffer(display);
    display.erase(display.begin(), display.end());
    display = buf.buffer();
    
    while(true)
    {
        cls();
        cout<< endl;
        center("Manage Snapshots");
        cout<< endl;
        for(short x = 0; x < 3; x++)
        {
            cout<< endl;
        }
        
    }
}

inline void main_menu()
{
    using namespace snapshot;
    using namespace common;
    color::set::blackwhite();
    
    char ch;
    while(true)
    {
        cls();
        cout<< endl;
        color::centerhl::blue("Snapshot Main Menu");
        cout<< endl;
        
        for(short x = 0; x < 3; x++)
        {
            cout<< endl;
        }
        
        cout<< " 1 -  Take snapshot"<< endl;
        cout<< " 2 -  Manage Snapshots"<< endl;
        cout<< " q -  Exit"<< endl;
        
        ch = input::gkey();
        switch(tolower(ch))
        {
            case '1':
            {
                take_snapshot();
                color::set::blackwhite();
            }
            break;
            
            case '2':
            {
            }
            break;
            
            case 'q':
            {
                return;
            }
            
            default:
            {
            }
            break;
        }
    }
}

int main()
{
    color::initialize();
    
    main_menu();
    
    return 0;
}

