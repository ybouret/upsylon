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
            static const size_t HeaderSize = IO::Header::requested; //!< byte for Headers I/O

            //! transfert mode
            enum Mode
            {
                Static, //!< assume the size is fixed
                Packed  //!< the size is send before
            };

            //! send a block
            static inline
            void Send(mpi             &MPI,
                      const IO::Block &block,
                      const int        target,
                      const Mode       mode)
            {
                switch(mode)
                {
                    case Packed: MPI.SendSize(block.size(),target,Tag); /* FALLTHRU */
                    case Static: MPI.SendAll(block,target,Tag); break;
                }
            }

            //! recv a block
            static inline
            void Recv(mpi       &MPI,
                      IO::Block &block,
                      const int  source,
                      const Mode mode)
            {
                switch(mode)
                {
                    case Packed: block.setFast( MPI.RecvSize(source,Tag) ); /* FALLTHRU */
                    case Static: MPI.RecvAll( block, source, Tag); break;
                }
            }


            //! sendrecv
            static inline
            void SendRecv(mpi             & MPI,
                          const IO::Block & sendBlock,
                          const int         target,
                          IO::Block       & recvBlock,
                          const int         source,
                          const Mode        mode)
            {
                switch(mode)
                {
                    case Packed: recvBlock.setFast( MPI.SendRecvSizes( sendBlock.size(), target, Tag, source, Tag) ); /* FALLTHRU */
                    case Static: MPI.SendRecvAll(sendBlock, target, Tag, recvBlock, source, Tag); break;
                }
            }

        };
    }
}

#endif


