
//! \file
#ifndef Y_SORT_MERGE_POOL_INCLUDED
#define Y_SORT_MERGE_POOL_INCLUDED 1

#include "y/core/pool.hpp"
#include "y/sort/nodes.hpp"

#include <iostream>

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! assuming a doubly linked node
    //
    //__________________________________________________________________________
    template <typename NODE>
    class merge_pool_of
    {
    public:
        typedef core::pool_of<NODE>                sub_pool;           //!< internal sub list to hold nodes
        typedef typename compare_nodes<NODE>::proc compare_nodes_proc; //!< alias

        //! sort a given list with same members than sub_list
        template <typename POOL> static inline
        void sort( POOL &source, compare_nodes_proc proc, void *args)
        {
            assert(proc);
            switch(source.size)
            {
                case 0: return;
                case 1: return;
                default:
                {
                    assert(source.size>1);
                    sub_pool L;
                    sub_pool R;
                    const size_t        mid = source.size >> 1;
                    for( size_t i=source.size-mid;i>0;--i) L.store( source.query() );
                    for( size_t i=mid;i>0;--i)             R.store( source.query() );
                    assert(0==source.size);

                    merge_pool_of<NODE>::template sort<sub_pool>(L,proc,args);
                    merge_pool_of<NODE>::template sort<sub_pool>(R,proc,args);
                    merge_pool_of<NODE>::template fusion<POOL>(source,L,R,proc,args);
                }
            }
        }

        //! sorting nodes by increasing address
        template <typename POOL> static inline
        void by_increasing_address( POOL &source )
        {
            sort<POOL>(source,compare_nodes<NODE>::by_increasing_addr,NULL);
        }

        //! sorting nodes by increasing address
        template <typename POOL> static inline
        void by_decreasing_address( POOL &source )
        {
            sort<POOL>(source,compare_nodes<NODE>::by_decreasing_addr,NULL);
        }

    private:
        //! merge two sorted list
        template <typename POOL>
        static inline
        void fusion( POOL &target, sub_pool &L, sub_pool &R, compare_nodes_proc proc, void *args )
        {
            //! while we can compare two nodes
            assert(0==target.size);
            while( (L.size>0) && (R.size>0) )
            {
                if( proc(L.head,R.head,args) < 0 )
                {
                    target.stash( L.query() );
                }
                else
                {
                    target.stash( R.query() );
                }
            }

            //! append remaining pools
            assert(0==L.size||0==R.size);
            while( L.size > 0 ) target.stash( L.query() );
            while( R.size > 0 ) target.stash( R.query() );
        }

    };
}

#endif

