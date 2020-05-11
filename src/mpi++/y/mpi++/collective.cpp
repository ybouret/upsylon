#include "y/mpi++/mpi.hpp"
#include "y/os/rt-clock.hpp"

namespace upsylon {
    
    void mpi:: Barrier() const
    {
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
        const uint64_t delta = rt_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
}
