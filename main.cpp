#include <iostream>
#include "snapshot_class.h"
#include "common.h"
#include "color.h"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/* @TODO
 * 
 * -  Start on snapshotList_class object
 * 
 */

void take_snapshot()
{
    using namespace snapshot;
    using namespace common;
    
    snapshot_class snap;
    bool b(false);
    
    snap = snap.take_snapshot();
    if(snapshot_class::is_valid(snap))
    {
        ofstream out;
        ifstream in;
        in.open(string(SNAPSHOT_FILE).c_str(), ios::in);
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
        cout<< " q -  Exit"<< endl;
        
        ch = input::gkey();
        switch(tolower(ch))
        {
            case '1':
            {
                
            }
            break;
            
            case 'q':
            {
                return;
            }
        }
    }
}

int main()
{
    color::initialize();
    
    
    
    return 0;
}

