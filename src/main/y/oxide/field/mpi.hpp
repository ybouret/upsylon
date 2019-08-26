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
            void SendFixed(mpi             &MPI,
                           const IO::Block &block,
                           const int        target )
            {
                MPI.Send(block,target,Tag);
            }

            static inline
            void RecvFixed(mpi       &MPI,
                           IO::Block &block,
                           const int source)
            {
                MPI.Recv(block, source, Tag);
            }

        };
    }
}

#endif


