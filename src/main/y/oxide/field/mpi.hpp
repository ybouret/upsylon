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

            //! send encode size of block
            static inline
            void SendHeader(mpi             &MPI,
                            const IO::Block &block,
                            const int        target)
            {
                const IO::Header & header = block.encodeHeader();
                MPI.Send(header.ro(),HeaderSize, MPI_BYTE, target,Tag);
            }

            //! recv encoded size of block
            static inline
            void RecvHeader(mpi       &MPI,
                            IO::Block &block,
                            const int  source)
            {
                IO::Header & header = block.header.get();
                MPI.Recv(header.rw(),HeaderSize,MPI_BYTE,source,Tag);
                block.decodeHeader();
            }

            //! send a block
            static inline
            void Send(mpi             &MPI,
                      const IO::Block &block,
                      const int        target,
                      const Mode       mode)
            {
                switch(mode)
                {
                    case Packed: SendHeader(MPI,block,target);                              /* FALLTHRU */
                    case Static: MPI.Send( block._(),target,Tag); break;
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
                    case Packed: RecvHeader(MPI,block,source);                          /* FALLTHRU */
                    case Static: MPI.Recv( block._(), source, Tag); break;
                }
            }

            //! exchange and adjust blocks
            static inline
            void SendRecvHeaders(mpi             & MPI,
                                 const IO::Block & sendBlock,
                                 const int         target,
                                 IO::Block       & recvBlock,
                                 const int         source)
            {
                const IO::Header & sendHeader = sendBlock.encodeHeader();
                IO::Header       & recvHeader = recvBlock.header.get();
                MPI.Sendrecv(sendHeader.ro(),HeaderSize, MPI_BYTE, target, Tag,
                             recvHeader.rw(), HeaderSize, MPI_BYTE, source, Tag);
                recvBlock.decodeHeader();
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
                    case Packed: SendRecvHeaders(MPI,sendBlock,target,recvBlock,source);      /* FALLTHRU */
                    case Static: MPI.SendRecv(sendBlock, target, Tag, recvBlock, source, Tag); break;
                }
            }

        };
    }
}

#endif


