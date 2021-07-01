
//! \file
#ifndef Y_SORT_NODES_INCLUDED
#define Y_SORT_NODES_INCLUDED 1

#include "y/comparison.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! utilities to compare nodes
    //
    //__________________________________________________________________________
    template <typename NODE> struct compare_nodes
    {
        //______________________________________________________________________
        //
        //! node comparison function
        //______________________________________________________________________
        typedef int (*proc)(const NODE *lhs, const NODE *rhs, void *args);

        //______________________________________________________________________
        //
        //! compare by increasing addresses
        //______________________________________________________________________
        static inline int by_increasing_addr(const NODE *lhs, const NODE *rhs, void *) throw()
        {
            return  comparison::increasing_addresses(lhs,rhs);
        }

        //______________________________________________________________________
        //
        //! compare by decreasing addresses
        //______________________________________________________________________
        static inline int by_decreasing_addr(const NODE *lhs, const NODE *rhs, void *) throw()
        {
            return  comparison::decreasing_addresses(lhs,rhs);
        }
    };

}


#endif
