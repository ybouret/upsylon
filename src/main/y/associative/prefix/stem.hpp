//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/associative/prefix/node.hpp"

namespace upsylon {

    template <typename CODE, typename T>
    class prefix_stem
    {
    public:
        typedef prefix_node<CODE,T> node_type;

        inline explicit prefix_stem() :
        root( new node_type(0,0,NULL) )
        {
        }

        inline virtual ~prefix_stem() throw()
        {
            
        }

    private:
        node_type *root;

    };

}

#endif

