//! \file

#ifndef Y_SUFFIX_NODE_INCLUDED
#define Y_SUFFIX_NODE_INCLUDED 1


#include "y/core/list.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{

    struct suffix
    {
        static  const size_t flag_size = sizeof(void*);
        typedef unsigned_int<flag_size>::type flag_t;
        static  void *                        in_use() throw();
    };

    template <typename CODE>
    struct suffix_node
    {
        typedef suffix::flag_t flag_t;
        union
        {
            CODE         code;
            uint64_t     _u64;
        };
        suffix_node *next;
        suffix_node *prev;
        suffix_node *parent;
        union
        {
            flag_t used;
            void  *addr;
        };

    };

}

#endif
