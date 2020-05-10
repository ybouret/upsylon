#include "y/mpi++/mpi.hpp"
#include "y/os/rt-clock.hpp"

namespace upsylon {
    
    void mpi::Send(const void        *buffer,
                   const size_t       count,
                   const MPI_Datatype datatype,
                   const int          dest,
                   const int          tag,
                   const MPI_Comm     comm) const
    {
        assert(!(buffer==NULL && count>0) );
        if(count>0)
        {
            const int items = static_cast<int>(count);
            if(items<0) throw exception(MPI_ERR_COUNT, "mpi::Send(count overflow)");
            const uint64_t mark = rt_clock::ticks();
            Y_MPI_CHECK(MPI_Send(buffer,items,datatype,dest,tag,comm));
            commSend.ticks( rt_clock::ticks() - mark );
        }
        
    }
    
    void mpi:: Recv(void              *buffer,
                    const size_t       count,
                    const MPI_Datatype datatype,
                    const int          source,
                    const int          tag,
                    const MPI_Comm     comm,
                    MPI_Status        &status) const
    {
        assert(!(buffer==NULL && count>0) );
        if(count>0)
        {
            const int items = static_cast<int>(count);
            if(items<0) throw exception(MPI_ERR_COUNT, "mpi::Recv(count overflow)");
            const uint64_t mark = rt_clock::ticks();
            Y_MPI_CHECK(MPI_Recv(buffer,items,datatype,source,tag,comm,&status));
            commRecv.ticks( rt_clock::ticks() - mark );
        }
    }
}
