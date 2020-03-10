
//! \file

#ifndef Y_STRING_TREE_INCLUDED
#define Y_STRING_TREE_INCLUDED 1

#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    //! store anything indexed by strings/text
    template <typename T>
    class string_tree : public suffix_tree<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        inline explicit string_tree() : suffix_tree<T>() {}
        inline virtual ~string_tree() throw() {}

        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(string_tree);
    };

}

#endif
