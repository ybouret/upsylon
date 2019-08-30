//! \file

#ifndef Y_OXIDE_FIELD_MPI_INCLUDED
#define Y_OXIDE_FIELD_MPI_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/field/io.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! MPI related routines
        struct Comm
        {
            static const int    Tag        = 0x07; //!< default tag

            //! send a block
            static inline
            void Send(mpi             &MPI,
                      const IO::Block &block,
                      const int        target,
                      const comm_mode  mode)
            {
                MPI.vSend(mode,block,target,Tag);

            }

            //! recv a block
            static inline
            void Recv(mpi            &MPI,
                      IO::Block      &block,
                      const int       source,
                      const comm_mode mode)
            {
                MPI.vRecv(mode,block,source,Tag);
            }


            //! sendrecv
            static inline
            void SendRecv(mpi             & MPI,
                          const IO::Block & sendBlock,
                          const int         target,
                          IO::Block       & recvBlock,
                          const int         source,
                          const comm_mode   mode)
            {
                MPI.vSendRecv(mode,sendBlock,target,Tag,recvBlock,source,Tag);
            }

        };
    }
}

#endif


