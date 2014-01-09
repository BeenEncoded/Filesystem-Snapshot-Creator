#ifndef VECTOR_DISPLAY_BUFF_H_INCLUDED
#define VECTOR_DISPLAY_BUFF_H_INCLUDED
#include <string>
#include <vector>

// the number of elements to show in the window of the buffer
#define VDB_CLASS_WINDOWSIZE 20

namespace displayBuffer
{
    class list_buffer_class;
    
    typedef std::vector<std::string> buffer_type;
    
    /* A better way to represent the two positions
     relative to each buffer. */
    struct buffer_position_data
    {
        unsigned int whole, part;
    };
    
    //displays a buffer
    std::ostream& operator<<(std::ostream&, buffer_type&);
    
    class list_buffer_class
    {
    private:
        typedef unsigned int pos_type;
        
    public:
        explicit list_buffer_class() : beg(0), end(0), cur(0), p(), display({}) {}
        
        ~list_buffer_class()
        {
            this->display.erase(this->display.begin(), this->display.end());
        }
        
        /* For the purpose of referencing the full buffer, and member
         functions such as .size(). */
        const buffer_type& whole_buffer() const
        {
            return this->display;
        }
        
        /* Erases al data from the object. */
        void clear()
        {
            this->beg = 0;
            this->end = 0;
            this->cur = 0;
            this->setp();
            this->display.erase(this->display.begin(), this->display.end());
        }
        
        /* Returns the buffer window */
        const buffer_type buffer()
        {
            if(!this->window_is_valid())
            {
                this->reset_window();
            }
            buffer_type temp_buf;
            for(unsigned int x = beg; x < end; x++)
            {
                temp_buf.push_back(display[x]);
            }
            return temp_buf;
        }
        
        bool at_end() const
        {
            return (this->cur >= (this->display.size() - 1));
        }
        
        bool at_beg() const
        {
            return ((this->cur == 0) && (this->display.size() > 0));
        }
        
        /* Moves the window 'up' in the vector (decrements the current position) */
        void plus()
        {
            if(!this->window_can_be_moved() || this->at_beg())
            {
                //make sure that the window is valid
                if(!this->window_is_valid())
                {
                    this->reset_window();
                }
                return;
            }
            this->cur--;
            while(this->cur < this->beg)
            {
                this->beg--;
                this->end--;
            }
            this->setp();
            
            /* Hopefully, it doesn't come to this, but you never really know... */
            if(!this->window_is_valid())
            {
                this->reset_window();
            }
        }
        
        /* Moves the window 'down' in the vector (increments the current position) */
        void minus()
        {
            if(!this->window_can_be_moved() || this->at_end())
            {
                if(!this->window_is_valid())
                {
                    this->reset_window();
                }
                return;
            }
            
            this->cur++;
            while(this->end <= this->cur)
            {
                this->end++;
                this->beg++;
            }
            this->setp();
            
            if(!this->window_is_valid())
            {
                this->reset_window();
            }
        }
        
        void set_buffer(const buffer_type& b)
        {
            this->display = b;
            if(!this->window_is_valid())
            {
                this->reset_window();
            }
        }
        
        /* Returns position data pertaining to positions relative to
         the entire display, and that of the window. */
        const buffer_position_data& pos() const
        {
            return p;
        }
        
    private:
        pos_type beg, end, cur;
        buffer_position_data p;
        buffer_type display;
        
        void reset_window();
        
        /* Checks to make sure if the buffer window falls within the
         bounds of the whole display, and conforms to the size, and that
         the current position is within it. If ever this returns false, then
         reset_window should be called.*/
        bool window_is_valid() const
        {
            return (
                    //check bounds:
                    (this->cur >= this->beg) && (this->cur < this->end) && 
                    (this->end > this->beg) && ((this->end - this->beg) <= unsigned(VDB_CLASS_WINDOWSIZE)) && 
                    //check check that they fall within the actual display size:
                    (this->end <= this->display.size()) && 
                    ((this->end - this->beg) > 0) && 
                    //so, now we check to see if the window size matches up correctly with the display size:
                    (((this->display.size() > unsigned(VDB_CLASS_WINDOWSIZE)) && ((this->end - this->beg) == unsigned(VDB_CLASS_WINDOWSIZE))) ||
                    ((this->display.size() <= unsigned(VDB_CLASS_WINDOWSIZE)) && ((this->end - this->beg) == this->display.size()))));
        }
        
        /* Returns true if the window is smaller than the whole display */
        bool window_can_be_moved() const
        {
            return (((this->end - this->beg) == unsigned(VDB_CLASS_WINDOWSIZE)) && 
                    (this->display.size() > this->end));
        }
        
        void setp()
        {
            this->p.part = (this->cur - this->beg);
            this->p.whole = this->cur;
        }
        
    };
}

#endif