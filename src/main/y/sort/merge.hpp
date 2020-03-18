//! \file
#ifndef Y_SORT_MERGE_INCLUDED
#define Y_SORT_MERGE_INCLUDED 1

#include "y/core/list.hpp"

namespace upsylon
{
    //! assuming a doubly linked node
    template <typename NODE>
    class merging
    {
    public:
        typedef core::list_of<NODE> sub_list; //!< internal sub list to hold nodes
        typedef int (*compare_node_proc)(const NODE *lhs, const NODE *rhs, void *args); //!< node comparison function

        //! sort a given list with same members than sub_list
        template <typename LIST> static inline
        void sort( LIST &source, compare_node_proc proc, void *args)
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

                    merging<NODE>::template sort<sub_list>(L,proc,args);
                    merging<NODE>::template sort<sub_list>(R,proc,args);
                    merging<NODE>::template fusion<LIST>(source,L,R,proc,args);
                }
            }
        }

        //! sorting nodes by address
        template <typename LIST> static inline
        void sort_by_addr( LIST &source )
        {
            sort<LIST>(source,compare_nodes_addr,NULL);
        }
        
    private:
        //! merge two sorted list
        template <typename LIST>
        static inline
        void fusion( LIST &target, sub_list &L, sub_list &R, compare_node_proc proc, void *args )
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
        
        static inline int compare_nodes_addr( const NODE *lhs, const NODE *rhs, void *) throw()
        {
            return int( static_cast<ptrdiff_t>(rhs-lhs) );
        }
    };
}

#endif

