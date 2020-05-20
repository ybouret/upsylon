//! \file

#ifndef Y_SPADE_MPI_INCLUDED
#define Y_SPADE_MPI_INCLUDED 1

#include "y/mpi++/xmpi.hpp"
#include "y/spade/network/transfer.hpp"

namespace upsylon {

    namespace Spade {

        //! handle multiple transfers
        class ParallelTransfer : public Transfer
        {
        public:
            explicit ParallelTransfer(const comms::topology); //!< setup
            virtual ~ParallelTransfer() throw();              //!< cleanup

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelTransfer);
        };

    }

}

#endif

