//! \file

#ifndef Y_SUFFIX_GRAPH_INCLUDED
#define Y_SUFFIX_GRAPH_INCLUDED 1

#include "y/suffix/knot.hpp"
#include "y/suffix/tree.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    
    template <
    typename CODE,
    typename T,
    typename BASE_CLASS>
    class suffix_graph : public BASE_CLASS
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef suffix_tree<CODE> tree_type;
        typedef suffix_node<CODE> tree_node;
        typedef suffix_knot<T>    data_node;
        typedef typename data_node::list_type data_list;
        typedef typename data_node::pool_type data_pool;
        
        
        inline explicit       suffix_graph() : dlist(), htree(), dpool() {}
        inline virtual       ~suffix_graph() throw() {}
        
        inline virtual size_t size()     const throw() { return dlist.size; }
        inline virtual size_t capacity() const throw() { return dlist.size + dpool.size; }
        inline virtual void   reserve(const size_t n) { dpool.cache(n); }
        inline virtual void   free() throw()
        {
            dpool.store(dlist);
            htree.erase();
        }
        
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
