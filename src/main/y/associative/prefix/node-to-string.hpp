
//! \file

#ifndef Y_PREFIX_NODE_TO_STRING_INCLUDED
#define Y_PREFIX_NODE_TO_STRING_INCLUDED 1

#include "y/associative/prefix/node.hpp"
#include "y/string.hpp"

namespace upsylon {

    //! convert node path to string
    template <typename T> inline
    string prefix_node_to_string(const prefix_node<char,T> *node)
    {
        assert(node);
        string s(node->depth,as_capacity,true); assert(s.size()==node->depth);
        node->encode(*s);
        return s;
    }
    
}

#endif
