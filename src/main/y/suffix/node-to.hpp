
//! \file

#ifndef Y_SUFFIX_NODE_TO_STRING_INCLUDED
#define Y_SUFFIX_NODE_TO_STRING_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/string.hpp"

namespace upsylon {

    struct suffix_node_
    {
        //! encode to a string with length=node->depth
        static string to_string(const suffix_node<char> *node);

        //! assuming a big-endian sizeof(void*) path
        static void  *to_address(const suffix_node<char> *node);
    };

    
    
}

#endif
