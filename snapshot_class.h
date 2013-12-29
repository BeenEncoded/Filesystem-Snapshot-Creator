#ifndef SNAPSHOT_CLASS_H_INCLUDED
#define SNAPSHOT_CLASS_H_INCLUDED
#include <string>
#include <vector>
#include <fstream>

namespace snapshot
{
    class snapshot_class;
    
    // typedefs:
    typedef std::string path_type;//make a pos_type container to load when we need to use the data at that pos
    typedef std::string time_type;
    typedef std::vector<path_type> pathList_type;
    
    /* File stream operators: */
    std::ostream& operator<<(std::ostream&, pathList_type&);
    std::istream& operator>>(std::istream&, pathList_type&);
    
    
    class snapshot_class
    {
    public:
        explicit snapshot_class() : path_list(), timestamp("") {}
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
        
        void clear()
        {
            for(path_type s : this->path_list) s.erase();
            this->path_list.erase(this->path_list.begin(), this->path_list.end());
            this->timestamp.erase();
        }
        
        std::ostream& out(std::ostream&) const;
        
    private:
        
        //  Variables:
        pathList_type path_list;
        time_type timestamp;
        
    };
    
    
}

#endif