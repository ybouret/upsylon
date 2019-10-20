//! \file
#ifndef Y_SORT_INDEX_INCLUDED
#define Y_SORT_INDEX_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    //! indexing functions
    struct indexing
    {
        //! make index, C++ style
        template <typename T,typename FUNC> static inline
        void make( addressable<size_t> &idx, FUNC &compare,  const accessible<T> &ra  ) throw()
        {

            // initialize indices
            const size_t n = idx.size(); assert( ra.size() == idx.size() );
            for(size_t i=n;i>0;--i) idx[i] = i;

            // sort routine
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

        //! rank source into target according to indices
        template <typename T> static inline
        void rank( addressable<T> &target, const accessible<T> &source, const accessible<size_t> &indices )
        {
            assert(target.size()==source.size());
            assert(target.size()==indices.size());
            for(size_t i=target.size();i>0;--i)
            {
                assert(indices[i]>0);
                assert(indices[i]<=target.size());
                target[i] = source[indices[i]];
            }
        }

        //! with the help of local memory
        template <typename T> static inline
        void rank( addressable<T> &arr, const accessible<size_t> &idx )
        {
            assert(arr.size()==idx.size());
            const size_t num = arr.size();
            vector<T>    tmp(num);
            rank(tmp,arr,idx);
            for(size_t i=num;i>0;--i)
            {
                bswap(tmp[i],arr[i]);
            }
        }




    };

}

#endif
