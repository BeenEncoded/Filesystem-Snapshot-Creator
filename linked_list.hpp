#ifndef LINKED_LIST_HPP_INCLUDED
#define LINKED_LIST_HPP_INCLUDED
#include <windows.h>

namespace linkedList
{
    
    /* Del_var deletes a pointer and sets it to NULL if it is not
     * already set to NULL. */
    template<class type>
    inline void del_var(type *t)
    {
        if(t != NULL)
        {
            delete t;
            t = NULL;
        }
    }
    
    /* Node_class represents a link in a chain of nodes. */
    template<class var_type>
    class node_class
    {
    public:
        explicit node_class() : back(NULL), forward(NULL), var(new var_type()) {}
        
        /* Destroys this single node, an all forward nodes. */
        ~node_class()
        {
            if(this->back != NULL)
            {
                throw "node_class::~node_class() ERROR: back != NULL";
                std::abort();
            }
            del_var(this->var);
            del_var(this->forward);
        }
        
        void operator=(node_class<var_type>& node)
        {
            del_var(this->var);
            if(this->forward != NULL)
            {
                throw "Error: node_class::operator=(node_class& node): forward != NULL";
                std::abort();
            }
            
            this->forward = new node_class(*(node.forward));
            
            if(this->back != NULL)
            {
                throw "Errorr: node_class::operator=(node_class& node): back != NULL";
                std::abort();
            }
            
            this->back = new node_class(*(node.back));
            this->var = new var_type(*(node.var));
        }
        
        /* Clears only the variable stored at this 
         * single node.  Does not clear it's links. */
        void clear()
        {
            switch(this->var == NULL)
            {
                case true:
                {
                    this->var = new var_type();
                }
                break;
                
                case false:
                {
                    delete this->var;
                    this->var = new var_type();
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        
        /* Returns a reference to the next node, and sets this instance to the nect node.
         *   if the next node does not exist, it returns itself. */
        node_class& next()
        {
            if(this->forward != NULL)
            {
                this = this->forward;
            }
            return *this;
        }
        
        /* Returns a reference to the previous node, and sets the current
         *  instance to the previous node.  if the previous node does 
         * not exist, it returns itself.  */
        node_class& previous()
        {
            if(this->back != NULL)
            {
                this = this->back;
            }
            return *this;
        }
        
        bool at_end() const
        {
            return (this->forward == NULL);
        }
        
        bool at_beg() const
        {
            return (this->back == NULL);
        }
        
        /* Removes this node from the linked list, severing it
         and mending the gap. Returns a pointer to the removed node. */
        node_class *remove()
        {
            if(this->back != NULL)
            {
                this->back->forward = this->forward;
            }
            if(this->forward != NULL)
            {
                this->forward->back = this->back;
            }
            node_class *tempnode(this);
            this->back = NULL;
            this->forward = NULL;
            this = this->back;
            return tempnode;
        }
        
        /* Adds a new node */
        void push_back(const node_class<var_type>& node)
        {
            if(node.back != NULL)
            {
                throw "node_class::push_back(node_class& node): ERROR: node.back != NULL!  Memory leak would occure!";
                std::abort();
            }
            while(this->forward != NULL)
            {
                this = this->forward;
            }
            this->forward = new node_class(node);
            this->forward->back = this;
        }
        
        var_type &data()
        {
            return *(this->var);
        }
        
    private:
        node_class *back, *forward;
        var_type *var;
        
    };
    
    
    template<class var_type>
    class linked_list_class
    {
    public:
        explicit linked_list_class() : node(NULL), beg(NULL), end(NULL), s(0) {}
        
        ~linked_list_class()
        {
            /* we must start delete at the beginning, or else it will fail. */
            if(this->beg != NULL) delete this->beg;
        }
        
        void push_back(const var_type& v)
        {
            switch(this->node == NULL)
            {
                //no linked list, so create one
                case true:
                {
                    this->node = new node_class<var_type>();
                    this->beg = this->node;
                    this->end = this->node;
                    this->node->data() = v;
                    this->s++;
                }
                break;
                
                //append to the existing linked list
                case false:
                {
                    this->node->push_back(node_class<var_type>());
                    this->s++;
                    while(!this->end->at_end())
                    {
                        this->end->next();
                    }
                    this->end->data() = v;
                }
                break;
                
                default:
                {
                }
                break;
            }
        }
        
        /* @todo: remove(), reference one with the index operator, clear... etc...*/
        
    private:
        //@nodes: @node current node @beg begining of linked list @end end of linked list
        node_class<var_type> *node, *beg, *end;
        
        //size
        unsigned int s;
        
    };
    
}

#endif