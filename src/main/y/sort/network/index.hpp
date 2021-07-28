//! \file
#ifndef Y_NETWORK_SORT_INDEX_INCLUDED
#define Y_NETWORK_SORT_INDEX_INCLUDED 1

#include "y/sort/network.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! C-style indexation based on network_sort
    //
    //__________________________________________________________________________
    template <const size_t N>
    struct newtork_index
    {
        //______________________________________________________________________
        //
        // types and defintions
        //______________________________________________________________________
        typedef network_sort<N> algo;                                      //!< base algorithm
        static  const size_t    nops = sizeof(algo::I)/sizeof(algo::I[0]); //!< number of tests

        //______________________________________________________________________
        //
        //! build a [0:N-1] index of data[0:N-1]
        //______________________________________________________________________
        template <typename T, typename COMPARE> static inline
        void make( size_t *indx, const T *data, COMPARE &compare)
        {
            for(size_t k=0;k<N;++k) indx[k] = k;
            for(size_t k=0;k<nops;++k)
            {
                size_t &i = indx[algo::I[k]];
                size_t &j = indx[algo::J[k]];
                if( compare(data[j],data[i]) < 0 )
                    bswap(i,j);
            }
        }

    };
}

#endif

