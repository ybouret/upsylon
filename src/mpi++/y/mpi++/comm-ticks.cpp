
#include "y/mpi++/mpi.hpp"


namespace upsylon
{
    mpi::comm_ticks:: comm_ticks() throw() :
    last(0),
    full(0)
    {
    }
    
    mpi:: comm_ticks:: ~comm_ticks() throw()
    {
        last = full = 0;
    }
    
    void mpi:: comm_ticks:: operator()(const uint64_t delta) throw()
    {
        full += (last=delta);
    }
}
