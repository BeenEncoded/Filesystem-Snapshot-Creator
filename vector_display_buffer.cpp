#include <iostream>
#include "vector_display_buffer.h"
#include <string>
#include <vector>

using namespace std;

//display stream operators:
namespace displayBuffer
{
    ostream& operator<<(ostream& out, buffer_type& buffer)
    {
        for(auto memb : buffer) out<< memb<< endl;
        return out;
    }
    
}

namespace displayBuffer
{
    /* Re-syncs the size of the window to the buffer. Corrects
     for any mis-placement (ex: part of the window, or current position, is in un-allocated space) */
    void list_buffer_class::reset_window()
    {
        //if the display's size is zero, the set all of out placeholders
        if(this->display.size() == 0)
        {
            this->beg = 0;
            this->end = 0;
            this->cur = 0;
            this->p.whole = 0;
            this->p.part = 0;
            //there's also nothing to do...
            return;
        }
        
        //make sure that we initially have a valid current position.
        while(this->cur >= this->display.size())
        {
            this->cur--;
        }
        
        unsigned int buf_size(VDB_CLASS_WINDOWSIZE);
        
        
        buf_size++;
        
        //initial values:
        this->beg = 0;
        this->end = (this->beg + buf_size);
        
        //now lets make sure they are in range of the full buffer
        while(this->end > this->display.size())
        {
            this->end--;
            if((this->beg > 0) && (this->end > buf_size))
            {
                this->beg = (this->end - buf_size);
            }
        }
        
        //the last step is to make sure that the current position is inside the window:
        switch(this->buffer().size() == (this->end - this->beg))
        {
            case true:
            {
                //buffer reaches both ends.
                if(this->end > 0)
                {
                    while(cur >= this->end)
                    {
                        this->cur--;
                    }
                }
            }
            break;
            
            //the window needs to be moved so that the current position will be displayed
            case false:
            {
                //the buffer window is part of the full buffer, we can move it:
                while(this->cur >= this->end)
                {
                    this->end++;
                    this->beg++;
                }
                while(this->cur < this->beg)
                {
                    this->beg--;
                    this->end--;
                }
            }
            break;
            
            default:
            {
            }
            break;
        }
        this->p.whole = this->cur;
        this->p.part = (this->cur - this->beg);
    }
}

