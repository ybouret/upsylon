
//! \file

#ifndef Y_SUFFIX_NODE_TO_STRING_INCLUDED
#define Y_SUFFIX_NODE_TO_STRING_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/string.hpp"

namespace upsylon {

    //! convert node path to string
    string suffix_node_to_string(const suffix_node<char> *node)
    {
        assert(node);
        string s(node->depth,as_capacity,true); assert(s.size()==node->depth);
        node->encode(*s);
        return s;
    }
    
}

#endif
