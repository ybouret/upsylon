//! \file

#ifndef Y_SUFFIX_GRAPH_INCLUDED
#define Y_SUFFIX_GRAPH_INCLUDED 1

#include "y/suffix/knot.hpp"
#include "y/suffix/tree.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! compound class to build suffix classes
    //
    //__________________________________________________________________________
    template <
    typename CODE,
    typename T,
    typename BASE_CLASS>
    class suffix_graph : public BASE_CLASS
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                              //!< aliases
        typedef suffix_tree<CODE>             tree_type;  //!< alias
        typedef suffix_node<CODE>             tree_node;  //!< alias
        typedef suffix_knot<T>                data_node;  //!< alias
        typedef typename data_node::list_type data_list;  //!< alias
        typedef typename data_node::pool_type data_pool;  //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup
        inline explicit       suffix_graph() : dlist(), htree(), dpool() {}
        
        //! cleanup
        inline virtual       ~suffix_graph() throw() {}
        
        //______________________________________________________________________
        //
        // container interface if necessary
        //______________________________________________________________________

        //! number of objects
        inline virtual size_t size()     const throw() { return dlist.size; }
        
        //! capacity
        inline virtual size_t capacity() const throw() { return dlist.size + dpool.size; }
        
        //! preload cache
        inline virtual void   reserve(const size_t n) { dpool.cache(n); }
        
        //! remove objects, keep memory
        inline virtual void   free() throw()
        {
            dpool.store(dlist);
            htree.erase();
        }
        
        //! release all possible memory
        inline virtual void release() throw()
        {
            dpool.release();
            dlist.release();
            htree.ditch();
        }
        
    private:
        data_list dlist;
        tree_type htree;
        data_pool dpool;
        
        Y_DISABLE_COPY_AND_ASSIGN(suffix_graph);
    };
    
}

#endif
