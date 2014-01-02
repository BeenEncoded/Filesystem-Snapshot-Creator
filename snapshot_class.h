#ifndef SNAPSHOT_CLASS_H_INCLUDED
#define SNAPSHOT_CLASS_H_INCLUDED
#include <string>
#include <vector>
#include "global_defines.h"
#include <fstream>



namespace snapshot
{
    class snapshot_class;
    
    // typedefs:
    typedef std::string path_type;//make a pos_type container to load when we need to use the data at that pos
    typedef std::string time_type;
    typedef std::vector<path_type> pathList_type;
    
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
        
        /* Returns true if it contains data.  False if not.*/
        static bool is_valid(const snapshot_class& snap)
        {
                return ((snap.get_pathList().size() > 0) && (snap.get_timestamp().size() > 0));
        }
        
    private:
        
        //  Variables:
        pathList_type path_list;
        time_type timestamp;
        id_type id;
        
    };
    
    
}

#endif