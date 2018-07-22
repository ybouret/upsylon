//! \file
#ifndef Y_SORT_INDEX_INCLUDED
#define Y_SORT_INDEX_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    //! indexing functions
    struct indexing
    {
        //! make index, C++ style
        template <typename T,typename FUNC> static inline
        void make( array<size_t> &idx, FUNC &compare,  const array<T> &ra  ) throw()
        {
            const size_t n = idx.size(); assert( ra.size() == idx.size() );

            for( size_t i=1; i <= n; ++i )
                idx[i] = i;

            size_t   inc = 1;
            do
            {
                inc *=3;
                ++inc;
            }
            while( inc <= n );
            do
            {
                inc /= 3;
                for( size_t i=inc+1;i<=n;++i)
                {
                    const size_t   t = idx[i];
                    const T       &v = ra[t];
                    size_t         j = i;
                    size_t         d = j-inc;
                    assert( t      <= n );
                    assert( idx[d] <= n );
                    while( compare( v, ra[ idx[d] ] ) < 0 )
                    {
                        idx[j] = idx[d];
                        j = d;
                        if( j <= inc )
                            break;
                        d -= inc;
                    }
                    idx[ j ] = t;
                }

            }
            while( inc > 1 );
        }

        template <typename T> static inline
        void rank(array<T>            &rra,
                  const array<size_t> &idx,
                  array<T>            &tmp )
        {
            assert(rra.size()==idx.size());
            assert(idx.size()==rra.size());
            const size_t n = rra.size();
            for(size_t i=n;i>0;--i) tmp[i] = rra[i];
            for(size_t i=n;i>0;--i) rra[i] = tmp[ idx[i] ];
        }

    };

}

#endif
