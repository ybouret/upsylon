//! \file

#ifndef Y_OXIDE_FIELD_MPI_INCLUDED
#define Y_OXIDE_FIELD_MPI_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/field/io.hpp"

namespace upsylon
{
    namespace Oxide
    {
        struct Comm
        {
            static const int Tag = 0x07;

            static inline
            void Send( const mpi &MPI, const IO::Block &block, const int target )
            {

            }

        };
    }
}

#endif


