
#ifndef Y_SUFFIX_KNOT_INCLUDED
#define Y_SUFFIX_KNOT_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    template <typename CODE,typename T>
    class suffix_knot
    {
    public:
        typedef suffix_node<CODE>          hook_type;
        typedef core::list_of<suffix_knot> list_;
        Y_DECL_ARGS(T,type);
        
        
        
        inline type       & operator*() throw() { return data; }
        inline const_type & operator*() const throw() { return data; }
        
        
        suffix_knot *next;
        suffix_knot *prev;
        hook_type   *hook;
        type         data;
        
        inline  suffix_knot(const_type &args) : next(0), prev(0), hook(0), data(args) {}
        inline ~suffix_knot() throw() { assert(0==next); assert(0==prev); }
        
       
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_knot);
    };
    
    
}


#endif
