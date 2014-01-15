/* This program is written by Jonathan Whitlock.
 * 
 * For terms of use, please refer to the GNU General Public Liscense at
 * http://www.gnu.org/licenses/gpl.html
 */

#include "snapshot_class.h"
#include "common.h"
#include "color.h"
#include "global_defines.h"
#include "vector_display_buffer.h"
#include "t_extra.h"
#include "fsysclass.h"
#include "snapshot_selection_class.hpp"

#include <iostream>
#include <exception>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>
#include <map>

using namespace std;

/* @todo
 * 
 * -  Write up the menus
 * -  Snapshot comparison functions
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
        
        //add commas to denote hundreds, thousands, millions... placeholders:
        for(unsigned int x = 0; x < temps.size(); x++)
        {
            newtemps += temps[x];
            if(x < (temps.size() - 1))
            {
                if(((x + 1) % 3) == 0) newtemps += ',';
            }
        }
        temps.erase();
        return newtemps;
    }
    
    inline string sanitize_string(const string& s)
    {
        if(s.size() == 0)
        {
            return "";
        }
        string temps(s);
        
        //remove specials
        for(string::iterator it = temps.begin(); it != temps.end(); it++)
        {
            if(common::is_special(*it) && (*it != ' '))
            {
                *it = ' ';
            }
        }
        
        if(temps.size() > 0)
        {
            //make sure that the string ends with a character that can be shown
            while(common::is_special(temps.back()))
            {
                temps.erase((temps.begin() + (temps.size() - 1)));
            }
        }
        
        return temps;
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
            abort();
        }
        basic_snapshot_data tempsnap;
        
        if(snaps.size() > 1)
        {
            //move the first basic_snapshot_data into a temporary storage
            tempsnap = *(snaps.begin());
            snaps.erase(snaps.begin());
            
            /* Get the most recent snapdata*/
            for(vector<snapshot::basic_snapshot_data>::iterator it = snaps.begin(); it != snaps.end(); it++)
            {
                if((it->t.size() != chrono_date().gasc_time().size()) || 
                        (tempsnap.t.size() != chrono_date().gasc_time().size()))
                {
                    throw "ERROR: Invalid Time Value!!";
                    abort();
                }
                if(!(chrono_date(it->t) < chrono_date(tempsnap.t)) && !(it->t == tempsnap.t))
                {
                    swap(tempsnap, *it);
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
        string filename(SNAPSHOT_FILE);
        stringstream ss;
        
        in.open(filename.c_str(), ios::INFILE);
        while(in.good())
        {
            snaps.push_back(basic_snapshot_data());
            if(common::filesystem::loadline(in, ss))
            {
                snapshot::snapshot_class::load_basic(ss, snaps.back());
            }
        }
        in.close();
        for(vector<snapshot::basic_snapshot_data>::iterator it = snaps.begin(); 
                it != snaps.end(); )
        {
            if(it->id == 0)
            {
                it = snaps.erase(it);
                continue;
            }
            it++;
        }
        
        return snaps;
    }
    
    /* Loads a snapshot of the specified id */
    inline snapshot_class from_id(const id_type& id)
    {
        common::input::ccin();
        string filename(SNAPSHOT_FILE);
        if(!fsys_class(filename).is_file())
        {
            return snapshot_class();
        }
        ifstream in;
        stringstream *ss(new stringstream());
        snapshot_class tempsnap;
        
        in.open(filename.c_str(), ios::INFILE);
        while(in.good())
        {
            if(snapshot_class::retrieve_id(in) == id)
            {
                if(common::filesystem::loadline(in, *ss))
                {
                    in.close();
                    (*ss)>> tempsnap;
                    delete ss;
                    return tempsnap;
                }
            }
            if(common::filesystem::loadline(in, *ss))
            {
                delete ss;
                ss = new stringstream();
            }
        }
        in.close();
        delete ss;
        return snapshot_class();
    }
    
    /* Re-writes the file without the snapshot bearing
     the specified id.*/
    inline void delete_snapshot(const id_type& id)
    {
        common::input::ccin();
        ifstream in;
        ofstream out;
        if(!fsys_class(string(SNAPSHOT_FILE)).is_file())
        {
            return;
        }
        
        /* Temporary scope.  I do not want these variables around
         when the function ends. */
        {
            string *snapfile(new string(SNAPSHOT_FILE)), *tempfile(new string(fsys_class().gpath() + "\\Temp.dat"));
            char *ch(new char());
            bool *temp_b(new bool());
            
            rename(snapfile->c_str(), tempfile->c_str());
            out.open(snapfile->c_str(), ios::OUTFILE);
            in.open(tempfile->c_str(), ios::INFILE);
            
            while(in.good())
            {
                //we found our snapshot, so let's skip over it:
                if(snapshot_class::retrieve_id(in) == id)
                {
                    while(((in.get(*ch), *ch) != '\n') && in.good());
                    continue;
                }
                *ch = '\0';
                *temp_b = false;
                //copy the next snapshot:
                while(in.good() && (char(in.peek()) != '\n'))
                {
                    in.get(*ch);
                    out<< (*ch);
                    if(!(*temp_b)) *temp_b = true;
                }
                if(char(in.peek()) == '\n')
                {
                    in.get(*ch);
                }
                if(*temp_b) out<< '\n';
            }
            in.close();
            out.close();
            if(fsys_class(*tempfile).del() == 1)
            {
                tempfile->erase();
                snapfile->erase();
                delete temp_b;
                delete tempfile;
                delete snapfile;
                delete ch;
                
                common::cls();
                for(short x = 0; x < 10; x++) cout<< endl;
                common::center("ERROR: unable to delete temporary file!");
                cout<< endl;
                common::wait();
                return;
            }
            
            tempfile->erase();
            snapfile->erase();
            delete temp_b;
            delete tempfile;
            delete snapfile;
            delete ch;
        }
        
    }
    
    vector<string> create_snapshot_display(vector<basic_snapshot_data>& snaps)
    {
        vector<string> display;
        if(snaps.size() > 0)
        {
            try
            {
                snapshot::sort_basic_data(snaps);
            }
            catch(const exception& e)
            {
                cout<< e.what();
                common::wait();
                abort();
            }
        }

        for(unsigned int x = 0; x < snaps.size(); x++)
        {
            display.push_back(chrono_date(snaps[x].t).std_time_disp());
            display.back() += ("  " + format_number(snaps[x].pathcount) + 
                    string(" paths  "));
            display.back() += ("(" + conv<id_type, string>(snaps[x].id) + ")");
        }
        return display;
    }
    
    /* Returns elements in base that are not found in changed. */
    inline vector<string> differences(const map<string, char>& base, const map<string, char>& changed)
    {
        vector<string> differences;
        for(map<string, char>::const_iterator it = base.begin(); it != base.end(); ++it)
        {
            if(changed.find(it->first) == changed.end())
            {
                differences.push_back(it->first);
            }
        }
        return differences;
    }
    
    inline void compare(const snapshotSelection::snapshot_selection_class& selected)
    {
        {
            using namespace common;
            cls();
            for(short x = 0; x < 10; x++) cout<< endl;
            center("Please wait... Comparison under way...");
        }
        
        snapshot_class snap1(from_id(selected.snapshots()[0]->id)), snap2(from_id(selected.snapshots()[1]->id));
        map<string, char> before, after;
        vector<string> created, deleted;
        ofstream out;
        string filename(string(REPORT_FOLDER) + "\\" + sanitize_string(chrono_date().gasc_time()) + ".txt");
        
        if(!fsys_class(REPORT_FOLDER).is_folder())
        {
            fsys_class().create(fsys_class(REPORT_FOLDER).gfilename());
        }
        
        if(chrono_date(snap2.get_timestamp()) < chrono_date(snap1.get_timestamp()))
        {
            swap(snap1, snap2);
        }
        
        for(string s : snap1.get_pathList()) before[s] = 'a';
        for(string s : snap2.get_pathList()) after[s] = 'a';
        
        created = differences(after, before);
        deleted = differences(before, after);
        
        out.open(filename.c_str(), ios::OUTFILE);
        
        out<< "Snapshot time frame: ["<< chrono_date(snap1.get_timestamp()).std_time_disp()<< "], ["<< 
                chrono_date(snap2.get_timestamp()).std_time_disp()<< "]"<< endl;
        
        switch((created.size() + deleted.size()) > 0)
        {
            case true:
            {
                for(short x = 0; x < 3; x++) out<< endl;
                
                switch(created.size() > 0)
                {
                    case true:
                    {
                        out<< "CREATED PATHS: "<< endl<< endl;
                        for(vector<string>::const_iterator it = created.begin(); it != created.end(); ++it) out<< *it<< endl;
                    }
                    
                    case false:
                    {
                        if(deleted.size() > 0)
                        {
                            if(created.size() > 0)
                            {
                                for(short x = 0; x < 3; x++) out<< endl;
                            }
                            out<< "DELETED PATHS: "<< endl<< endl;
                            for(vector<string>::const_iterator it = deleted.begin(); it != deleted.end(); ++it) out<< *it<< endl;
                        }
                    }
                    break;
                    
                    default:
                    {
                    }
                    break;
                }
            }
            break;
            
            case false:
            {
                out<< "No Changes!";
            }
            break;
            
            default:
            {
            }
            break;
        }
        
        out.close();
        
        {
            using namespace common;
            cls();
            input::ccin();
            if(input::is_sure("Would you like to open the report now?"))
            {
                cls();
                for(short x = 0; x < 10; x++) cout<< endl;
                center("Waiting for you to finish...");
                system(string("\"" + filename + "\"").c_str());
                cls();
            }
        }
        
    }
}

/* it takes a snapshot, and then saves it. */
inline void take_snapshot()
{
    using namespace common;
    color::set::greenblue();
    
    for(short x = 0; x < 3; x++) cout<< endl;
    center("Snapshot in progress...");
    cout<< endl;
    
    snapshot::snapshot_class snap;
    
    snap.take_snapshot(common::filesystem::retrieve_root_dir());
    switch(snapshot::snapshot_class::is_valid(snap))
    {
        case true:
        {
            ofstream out;
            out.open(string(SNAPSHOT_FILE).c_str(), ios::app);
            out<< snap<< '\n';
            out.close();
        }
        break;
        
        case false:
        {
        }
        
        default:
        {
            cls();
            for(short x = 0; x < 5; x++) cout<< endl;
            center("Error: Could not take a valid snapshot!");
            cout<< endl;
            wait();
            cls();
        }
        break;
    }
}

inline void manage_snapshots()
{
    using namespace common;
    color::set::blackwhite();
    
    char ch, control;
    displayBuffer::list_buffer_class buf;
    vector<snapshot::basic_snapshot_data> snaps(snapshot::load_basic_snapshot_data());
    vector<string> display(snapshot::create_snapshot_display(snaps));
    snapshotSelection::snapshot_selection_class snap_selection;
    bool temp_b(false);
    
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
        if(snaps.size() == 0)
        {
            center("NO SNAPSHOTS");
            cout<< endl;
        }
        for(unsigned int x = 0; x < display.size(); x++)
        {
            temp_b = snap_selection.selected(snaps[((buf.pos().whole - buf.pos().part) + x)]);
            
            switch(temp_b)
            {
                case true:
                {
                    ((buf.pos().part == x) ? color::hl::dark_green("[") : color::hl::blue("["));
                }
                break;
                
                case false:
                {
                    switch(x == buf.pos().part)
                    {
                        case true:
                        {
                            color::hl::green("[");
                        }
                        break;
                        
                        case false:
                        {
                            cout<< " ";
                        }
                        break;
                        
                        default:
                        {
                            cout<< " ";
                        }
                        break;
                    }
                }
                break;
                
                default:
                {
                    cout<< " ";
                }
                break;
            }
            
            cout<< display[x];
            
            switch(x == buf.pos().part)
            {
                case true:
                {
                    (temp_b ? color::hl::dark_green("]") : color::hl::green("]"));
                }
                break;
                
                case false:
                {
                    if(temp_b)
                    {
                        color::hl::blue("]");
                    }
                }
                break;
                
                default:
                {
                }
                break;
            }
            cout<< endl;
        }
        ch = getch();

        //scope for namespace common::input
        {
            using namespace common::input;
            switch(is_control_key(ch))
            {
                case true:
                {
                    control = getch();
                    switch(int(control))
                    {
                        //up down end home pgup pgdown
                        case UP_KEY:
                        {
                            buf.plus();
                            switch((buf.buffer().size() > 0) && (display.size() > 0))
                            {
                                case true:
                                {
                                    if(buf.buffer()[0] != display[0])
                                    {
                                        display.pop_back();
                                        display.insert(display.begin(), buf.buffer()[0]);
                                    }
                                }
                                break;

                                case false:
                                {
                                    display = buf.buffer();
                                }
                                break;

                                default:
                                {
                                    display = buf.buffer();
                                }
                                break;
                            }
                        }
                        break;

                        case DOWN_KEY:
                        {
                            buf.minus();
                            switch((buf.buffer().size() > 0) && (display.size() > 0))
                            {
                                case true:
                                {
                                    if(buf.buffer()[0] != display[0])
                                    {
                                        display.erase(display.begin());
                                        display.push_back(buf.buffer()[0]);
                                    }
                                }
                                break;

                                case false:
                                {
                                    display = buf.buffer();
                                }
                                break;

                                default:
                                {
                                    display = buf.buffer();
                                }
                                break;
                            }
                        }
                        break;
                        
                        case DELETE_KEY:
                        {
                            if(is_sure(("Do you want to delete snapshot taken on " + snaps[buf.pos().whole].t)))
                            {
                                cls();
                                for(short x = 0; x < 10; x++) cout<< endl;
                                center("Please wait...");
                                cout<< endl;
                                
                                if(snap_selection.selected(snaps[buf.pos().whole]))
                                {
                                    snap_selection.toggle(snaps[buf.pos().whole]);
                                }
                                
                                snapshot::delete_snapshot(snaps[buf.pos().whole].id);
                                snaps.erase((snaps.begin() + buf.pos().whole));
                                buf.set_buffer(snapshot::create_snapshot_display(snaps));
                                display = buf.buffer();
                            }
                            color::set::blackwhite();
                        }
                        break;

                        default:
                        {
                        }
                        break;
                    }
                }
                break;

                case false:
                {
                    switch(is_char(ch))
                    {
                        case true:
                        {
                            switch(ch)
                            {
                                case ' ':
                                {
                                    switch(snap_selection.selected(snaps[buf.pos().whole]))
                                    {
                                        case true:
                                        {
                                            snap_selection.toggle(snaps[buf.pos().whole]);
                                        }
                                        break;

                                        case false:
                                        {
                                            if(snap_selection.snapshots().size() < 2)
                                            {
                                                snap_selection.toggle(snaps[buf.pos().whole]);
                                            }
                                        }
                                        break;

                                        default:
                                        {
                                        }
                                        break;
                                    }
                                }
                                break;
                                
                                case '\\':
                                {
                                    snap_selection.clear();
                                }
                                break;
                                
                                default:
                                {
                                }
                                break;
                            }
                        }
                        break;

                        case false:
                        {
                            switch(int(ch))
                            {
                                //enter backspace escape

                                case ENTER_KEY:
                                {
                                    if(snap_selection.snapshots().size() == 2)
                                    {
                                        snapshot::compare(snap_selection);
                                        snap_selection.clear();
                                    }
                                }
                                break;

                                case BACKSPACE_KEY:
                                {
                                    return;
                                }
                                break;

                                default:
                                {
                                }
                                break;
                            }
                        }
                        break;

                        default:
                        {
                        }
                        break;
                    }
                }
                break;

                default:
                {
                }
                break;
            }
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
                try
                {
                    take_snapshot();
                }
                catch(const exception &e)
                {
                    cls();
                    center(e.what());
                    cout<< endl;
                    wait();
                    abort();
                }
                color::set::blackwhite();
            }
            break;
            
            case '2':
            {
                cls();
                for(short x = 0; x < 10; x++) cout<< endl;
                center("Loading...");
                cout<< endl;
                manage_snapshots();
                color::set::blackwhite();
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

