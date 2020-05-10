#include "y/mpi++/mpi.hpp"
#include "y/os/rt-clock.hpp"

namespace upsylon {
    
    void mpi:: Barrier(const MPI_Comm comm ) const
    {
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Barrier(comm));
        const uint64_t delta = rt_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
}
