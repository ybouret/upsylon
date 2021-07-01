
//! \file
#ifndef Y_SORT_NODES_INCLUDED
#define Y_SORT_NODES_INCLUDED 1

#include "y/comparison.hpp"

namespace upsylon
{

    //! utilities to compare nodes
    template <typename NODE> struct compare_nodes
    {

        typedef int (*proc)(const NODE *lhs, const NODE *rhs, void *args); //!< node comparison function

        //! compare by increasing addresses
        static inline int by_increasing_addr(const NODE *lhs, const NODE *rhs, void *) throw()
        {
            return  comparison::increasing_addresses(lhs,rhs);
        }

        //! compare by decreasing addresses
        static inline int by_decreasing_addr(const NODE *lhs, const NODE *rhs, void *) throw()
        {
            return  comparison::decreasing_addresses(lhs,rhs);
        }
    };

}


#endif
