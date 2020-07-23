//! \file

#ifndef Y_SORT_GROUPS_INCLUDED
#define Y_SORT_GROUPS_INCLUDED 1

#include "y/sort/sequence.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    //! finding groups of similar objects within a sequence
    struct find_groups
    {

        //!  build groups of an INCREASING sequence
        template <typename SEQUENCE> static inline
        void of_increasing(const SEQUENCE &seq, sequence<size_t> &g)
        {
            return guess(g,seq.begin(),seq.end());
        }

        //!  build groups of a sequence
        template <typename SEQUENCE> static inline
        void by_sorting_of( SEQUENCE &seq, sequence<size_t> &g)
        {
            sort_sequence(seq, comparison::increasing<typename SEQUENCE::type> );
            return guess(g,seq.begin(),seq.end());
        }
        
        //! build groups from a set of INCREASING values
        template <typename ITERATOR> static inline
        void guess(sequence<size_t> &g, ITERATOR curr, const ITERATOR last)
        {
            //------------------------------------------------------------------
            // initialize first group
            //------------------------------------------------------------------
            g.free();
            if(curr==last) return;
            g << 1;

            //------------------------------------------------------------------
            // iterate with comparison
            //------------------------------------------------------------------
            while(true)
            {
                ITERATOR prev = curr++;
                if(curr==last) return;
                if( *curr <= *prev )
                {
                    ++g.back();
                }
                else
                {
                    g << 1;
                }
            }
        }

    };

}

#endif

