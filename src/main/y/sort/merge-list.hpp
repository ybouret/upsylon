//! \file
#ifndef Y_SORT_MERGE_LIST_INCLUDED
#define Y_SORT_MERGE_LIST_INCLUDED 1

#include "y/core/list.hpp"
#include "y/sort/nodes.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! assuming a doubly linked node
    //
    //__________________________________________________________________________
    template <typename NODE>
    class merge_list_of
    {
    public:
        typedef core::list_of<NODE>                sub_list;           //!< internal sub list to hold nodes
        typedef typename compare_nodes<NODE>::proc compare_nodes_proc; //!< alias

        //! sort a given list with same members than sub_list
        template <typename LIST> static inline
        void sort( LIST &source, compare_nodes_proc proc, void *args)
        {
            assert(proc);
            switch(source.size)
            {
                case 0: return;
                case 1: return;
                default:
                {
                    assert(source.size>1);
                    sub_list L;
                    sub_list R;
                    const size_t        mid = source.size >> 1;
                    for( size_t i=source.size-mid;i>0;--i) L.push_front( source.pop_back() );
                    for( size_t i=mid;i>0;--i)             R.push_front( source.pop_back() );
                    assert(0==source.size);

                    merge_list_of<NODE>::template sort<sub_list>(L,proc,args);
                    merge_list_of<NODE>::template sort<sub_list>(R,proc,args);
                    merge_list_of<NODE>::template fusion<LIST>(source,L,R,proc,args);
                }
            }
        }

        //! sorting nodes by increasing address
        template <typename LIST> static inline
        void by_increasing_address( LIST &source )
        {
            sort<LIST>(source,compare_nodes<NODE>::by_increasing_addr,NULL);
        }

        //! sorting nodes by increasing address
        template <typename LIST> static inline
        void by_decreasing_address( LIST &source )
        {
            sort<LIST>(source,compare_nodes<NODE>::by_decreasing_addr,NULL);
        }
        
    private:
        //! merge two sorted list
        template <typename LIST>
        static inline
        void fusion( LIST &target, sub_list &L, sub_list &R, compare_nodes_proc proc, void *args )
        {
            //! while we can compare two nodes
            assert(0==target.size);
            while( (L.size>0) && (R.size>0) )
            {
                if( proc(L.head,R.head,args) < 0 )
                {
                    target.push_back( L.pop_front() );
                }
                else
                {
                    target.push_back( R.pop_front() );
                }
            }

            //! append remaining lists
            assert(0==L.size||0==R.size);
            while( L.size > 0 ) target.push_back( L.pop_front() );
            while( R.size > 0 ) target.push_back( R.pop_front() );
        }

    };
}

#endif

