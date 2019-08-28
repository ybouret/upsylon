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
            static const int Tag = 0x07; //!< default tag
            
            //! transfert mode
            enum Mode
            {
                Static, //!< assume the size is fixed
                Packed  //!< the size is send before
            };

            //! send encode size of block
            static inline
            void SendPrologue(mpi             &MPI,
                              const IO::Block &block,
                              const int        target)
            {
                block.encodePrologue();
                MPI.Send(block.prologue,IO::Block::Prologue, MPI_BYTE, target,Tag);
            }

            //! recv encoded size of block
            static inline
            void RecvPrologue(mpi       &MPI,
                              IO::Block &block,
                              const int  source)
            {
                MPI.Recv(block.prologue,IO::Block::Prologue,MPI_BYTE,source,Tag);
                block.setFast( block.decodePrologue() );
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
                    case Packed: SendPrologue(MPI,block,target);                              /* FALLTHRU */
                    case Static: MPI.Send( block.interface(),target,Tag); break;
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
                    case Packed: RecvPrologue(MPI,block,source);                          /* FALLTHRU */
                    case Static: MPI.Recv( block.interface(), source, Tag); break;
                }
            }

            //! exchange and adjust blocks
            static inline
            void SendrecvPrologue(mpi             & MPI,
                                  const IO::Block & sendBlock,
                                  const int         target,
                                  IO::Block       & recvBlock,
                                  const int         source)
            {
                sendBlock.encodePrologue();
                MPI.Sendrecv(sendBlock.prologue, IO::Block::Prologue, MPI_BYTE, target, Tag,
                             recvBlock.prologue, IO::Block::Prologue, MPI_BYTE, source, Tag);
                recvBlock.setFast( recvBlock.decodePrologue() );
            }

            //! sendrecv
            static inline
            void Sendrecv(mpi             & MPI,
                          const IO::Block & sendBlock,
                          const int         target,
                          IO::Block       & recvBlock,
                          const int         source,
                          const Mode        mode)
            {
                switch(mode)
                {
                    case Packed: SendrecvPrologue(MPI,sendBlock,target,recvBlock,source);      /* FALLTHRU */
                    case Static: MPI.SendRecv(sendBlock, target, Tag, recvBlock, source, Tag); break;
                }
            }

        };
    }
}

#endif


