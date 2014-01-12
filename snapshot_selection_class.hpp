#ifndef SNAPSHOT_SELECTION_CLASS_HPP_INCLUDED
#define SNAPSHOT_SELECTION_CLASS_HPP_INCLUDED
#include <vector>
#include <string>
#include "snapshot_class.h"
#include "global_defines.h"

/* Stores pointers to selected snapshots. */
namespace snapshotSelection
{
    class snapshot_selection_class
    {
    public:
        explicit snapshot_selection_class() : snaps({}) {}
        ~snapshot_selection_class()
        {
            this->snaps.erase(this->snaps.begin(), this->snaps.end());
        }
        
        const snapshot_selection_class& operator=(const snapshot_selection_class &s)
        {
            if(this != &s)
            {
                this->snaps.erase(this->snaps.begin(), this->snaps.end());
                this->snaps = s.snaps;
            }
            return *this;
        }
        
        bool operator==(const snapshot_selection_class &s) const
        {
            return ((this == &s) || (this->snaps == s.snaps));
        }
        
        bool operator!=(const snapshot_selection_class &s) const
        {
            return !(this->operator==(s));
        }
        
        void clear()
        {
            this->snaps.erase(this->snaps.begin(), this->snaps.end());
        }
        
        const std::vector<snapshot::basic_snapshot_data*> snapshots() const
        {
            return this->snaps;
        }
        
        bool selected(const snapshot::basic_snapshot_data &s) const
        {
            if(this->snaps.size() == 0)
            {
                return false;
            }
            for(snapshot::basic_snapshot_data *dat : this->snaps)
            {
                if(dat == &s)
                {
                    return true;
                }
            }
            return false;
        }
        
        void toggle(snapshot::basic_snapshot_data &s)
        {
            switch(this->selected(s))
            {
                case true:
                {
                    for(std::vector<snapshot::basic_snapshot_data*>::iterator it = this->snaps.begin(); it != this->snaps.end(); )
                    {
                        if((*it)->id == s.id)
                        {
                            it = this->snaps.erase(it);
                            continue;
                        }
                        it++;
                    }
                }
                break;
                
                case false:
                {
                    this->snaps.push_back(&s);
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        
    private:
        std::vector<snapshot::basic_snapshot_data*> snaps;
        
    };
    
}

#endif