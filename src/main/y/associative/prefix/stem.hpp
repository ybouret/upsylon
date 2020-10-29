//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/associative/prefix/node.hpp"

namespace upsylon {

    template <typename CODE, typename T>
    class prefix_stem
    {
    public:
        typedef          prefix_node<CODE,T>         node_type;
        typedef typename prefix_node<CODE,T>::pool_t pool_type;

        inline explicit prefix_stem() :
        root( new node_type(0,0,NULL) ),
        pool()
        {
        }

        inline virtual ~prefix_stem() throw()
        {
            
        }

        const ios::vizible & get_root() const throw() { assert(root); return *root; }

    private:
        node_type *root;
        pool_type  pool;

    };

}

#endif

