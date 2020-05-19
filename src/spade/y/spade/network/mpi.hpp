//! \file

#ifndef Y_SPADE_MPI_INCLUDED
#define Y_SPADE_MPI_INCLUDED 1

#include "y/mpi++/xmpi.hpp"
#include "y/spade/network/dispatcher.hpp"

namespace upsylon {

    namespace Spade {

        class ParallelDispatcher : public Dispatcher
        {
        public:
            explicit ParallelDispatcher(const comms::topology);
            virtual ~ParallelDispatcher() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelDispatcher);
        };

    }

}

#endif

