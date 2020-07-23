//! \file
#ifndef Y_SORT_CLASSES_INCLUDED
#define Y_SORT_CLASSES_INCLUDED 1

#include "y/sort/sequence.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    //! finding classes from sequences
    struct find_classes
    {
        //! count classes on a SORTED sequence
        template <typename SEQUENCE> static inline
        size_t of_pre_sorted(const SEQUENCE &seq)
        {
            return  guess(seq.begin(),seq.end());
        }

        //! sort and count classes
        template <typename SEQUENCE> static inline
        size_t by_sorting_of( SEQUENCE &seq )
        {
            sort_sequence(seq,comparison::increasing<typename SEQUENCE::type>);
            return guess(seq.begin(),seq.end());
        }

        //! count classes on a SORTED range
        /**
         assume that the range is sorted and that
         the content of the iterator has a '!=' operator
         */
        template <typename ITERATOR> static inline
        size_t guess( ITERATOR curr, const ITERATOR last )
        {
            if( curr != last )
            {
                ITERATOR scan = curr;
                size_t   ncls  = 1;

                while( scan != last )
                {
                    if( *scan != *curr )
                    {
                        ++ncls;
                        curr = scan;
                    }
                    ++scan;
                }
                return ncls;
            }
            else
            {
                return 0;
            }
        }

    };




}

#endif

