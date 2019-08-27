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
            enum Mode
            {
                Static,
                Packed
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
                    case Packed: {
                        const uint32_t sz = mpi::size_to_uint32(block.size());
                        MPI.Send<uint32_t>( sz, target, Tag);
                    }
                    case Static:
                        MPI.Send( static_cast<const IO::Array &>(block),target,Tag);
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
                    case Packed:
                    {
                        const uint32_t sz = MPI.Recv<uint32_t>(source,Tag);
                        block.adjust(sz,0);
                    }
                    case Static:
                        MPI.Recv( static_cast<IO::Array &>(block), source, Tag);
                }
            }

        };
    }
}

#endif


