//! \file
#ifndef Y_IPSO_FIELD_IO_INCLUDED
#define Y_IPSO_FIELD_IO_INCLUDED 1

#include "y/ipso/layout.hpp"
#include "y/container/sequence.hpp"
#include "y/sort/unique.hpp"

namespace upsylon
{
    namespace ipso
    {
        struct field_io
        {
            //! collect indices from a sub layout
            template <typename LAYOUT>
            static void collect( sequence<coord1D> &indices, const LAYOUT &full, const LAYOUT &sub );

            //! ensure unique set of increasing indices
            template <typename SEQUENCE>
            static void compile( SEQUENCE &indices ) throw() { unique<SEQUENCE>(indices); }

            

        };
    }
}

#endif

