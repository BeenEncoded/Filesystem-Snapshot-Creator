#ifndef SNAPSHOT_CLASS_H_INCLUDED
#define SNAPSHOT_CLASS_H_INCLUDED
#include <string>
#include <vector>
#include "global_defines.h"
#include <fstream>
#include "common.h"
#include <sstream>



namespace snapshot
{
    class snapshot_class;
    
    // typedefs:
    typedef std::string path_type;//make a pos_type container to load when we need to use the data at that pos
    typedef std::string time_type;
    typedef std::vector<path_type> pathList_type;
    
    /** Structs: */
    
    struct basic_snapshot_data
    {
        time_type t;
        id_type id;
        unsigned long int pathcount;
    };
    
    /** ------- other functions ------------------------------------ */
    
    
    /* Loads ids from the file.  May change with filesystem structure. */
    std::vector<id_type> load_ids();
    
    inline bool is_id(const id_type& id, const std::vector<id_type>& ids)
    {
        for(auto i : ids) if(i == id) return true;
        return false;
    }
    
    inline id_type newid()
    {
        id_type id(1);
        std::vector<id_type> ids(load_ids());
        while(is_id(id, ids))
        {
            id++;
        }
        return id;
    }
    
    /** -----------------------------------------------------------*/
    
    
    /** File stream operators: */
    std::ostream& operator<<(std::ostream&, pathList_type&);
    std::istream& operator>>(std::istream&, pathList_type&);
    
    /** File stream operators for the snapshot class: */
    std::ostream& operator<<(std::ostream&, snapshot_class&);
    std::istream& operator>>(std::istream&, snapshot_class&);
    
    class snapshot_class
    {
    public:
        explicit snapshot_class(const pathList_type& p, const time_type& t) : path_list(p), 
                timestamp(t), id(0) {}
        
        explicit snapshot_class() : path_list(), timestamp(""), id(0) {}
        
        ~snapshot_class()
        {
            this->clear();
        }
        
        snapshot_class& operator=(const snapshot_class& snap)
        {
            if(this != &snap)
            {
                this->clear();
                
                this->path_list = snap.path_list;
                this->timestamp = snap.timestamp;
            }
            return *this;
        }
        
        bool operator==(const snapshot_class& snap) const
        {
            return ((this == &snap) || ((this->timestamp == snap.timestamp) && 
                    (this->path_list == snap.path_list)));
        }
        
        bool operator!=(const snapshot_class& snap) const
        {
            return !(this->operator==(snap));
        }
        
        /* Erases all data in the object.*/
        void clear()
        {
            for(path_type s : this->path_list) s.erase();
            this->path_list.erase(this->path_list.begin(), this->path_list.end());
            this->timestamp.erase();
        }
        
        snapshot_class take_snapshot() const;
        snapshot_class take_snapshot(const std::string&) const;
        
        /** Getters. */
        const time_type& get_timestamp() const
        {
            return this->timestamp;
        }
        
        const pathList_type& get_pathList() const
        {
            return this->path_list;
        }
        
        const id_type& gid() const
        {
            return this->id;
        }
        
        /** Filesstream serializers. */
        std::ostream& out(std::ostream&) const;
        std::istream& in(std::istream&);
        
        /* Loads basic information about a snapshot. Namely, the timestamp and the id. */
        static void load_basic(std::istream& in, basic_snapshot_data& bsd)
        {
            common::input::ccin();
            bsd.t.erase();
            bsd.id = 0;
            bsd.pathcount = 0;
            if(!in.good())
            {
                return;
            }
            std::stringstream ss;
            std::string temps;
            char *ch(new char()), delim(2);
            
            //scope for temporary stuff:
            {
                //some temporary information
                snapshot_class *tempsnap(new snapshot_class());
                std::ifstream::pos_type *tpos(new std::ifstream::pos_type(in.tellg()));
                
                //because we will be dealing with a massive ammount of data (about 200,000 strings per snapshot is expected)
                //we want to be memory efficient here
                getline(in, temps);
                if(temps.size() > 0)
                {
                    for(std::string::iterator it = temps.begin(); it != temps.end();)
                    {
                        ss<< *it;
                        it = temps.erase(it);
                    }
                    ss>> *tempsnap;
                    bsd.pathcount = tempsnap->get_pathList().size();
                    delete tempsnap;
                    
                    //we jump back to our starting position so that we can sontinue the function
                    in.seekg(*tpos);
                    delete tpos;
                }
            } //scope end
            
            while(((in.get(*ch), *ch) != delim))
            {
            }
            delete ch;
            
            if(in.good())
            {
                std::getline(in, temps, delim);
                if(!in.fail())
                {
                    ss<< temps;
                    ss>> bsd.t;
                    ss.str("");
                }
            }
            if(in.good())
            {
                std::getline(in, temps, delim);
                if(!in.fail())
                {
                    ss<< temps;
                    ss>> bsd.id;
                    ss.str("");
                }
            }
        }
        
        /* Returns true if it contains data.  False if not.*/
        static bool is_valid(const snapshot_class& snap)
        {
                return ((snap.get_pathList().size() > 0) && (snap.get_timestamp().size() > 0));
        }
        
        /* Retrieves the id from a line that should contain a snapshot in a file.
         It does not change the state of the stream. */
        static id_type retrieve_id(std::istream& in)
        {
            std::stringstream *ss(new std::stringstream());
            id_type id;
            std::string *temps(new std::string(""));
            
            {
                std::istream::pos_type *temp_pos(new std::istream::pos_type(in.tellg()));
                char *delim(new char(2)), *ch(new char());
                
                for(short x = 0; ((x < 2) && in.good()); x++)
                {
                    while((in.get(*ch), *ch) != *delim);
                }
                if(in.good())
                {
                    getline(in, *temps);
                    if(in.fail())
                    {
                        in.seekg(*temp_pos);
                        delete temp_pos;
                        delete delim;
                        delete ch;
                        delete ss;
                        delete temps;
                        return 0;
                    }
                }
                in.seekg(*temp_pos);
                delete temp_pos;
                delete delim;
                delete ch;
            }
            
            (*ss)<< (*temps);
            (*ss)>> id;
            temps->erase();
            delete temps;
            delete ss;
            return id;
        }
        
    private:
        
        //  Variables:
        pathList_type path_list;
        time_type timestamp;
        id_type id;
        
    };
    
    
}

#endif