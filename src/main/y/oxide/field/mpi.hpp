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

            static inline
            void SendPacked(mpi             &MPI,
                            const IO::Block &block,
                            const int        target )
            {
                const uint32_t sz = mpi::SizeToUint32(block.size());
                MPI.Send<uint32_t>( sz, target, Tag);
                SendFixed(MPI,block,target);

            }

            static inline
            void RecvPacked(mpi       &MPI,
                            IO::Block &block,
                            const int  source)
            {
                const uint32_t sz = MPI.Recv<uint32_t>(source,Tag);
                block.adjust(sz,0);
                RecvFixed(MPI,block,source);
            }



        };
    }
}

#endif


