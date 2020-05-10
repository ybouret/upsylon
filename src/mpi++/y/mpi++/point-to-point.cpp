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
    
    void mpi:: SendSize(const size_t   args,
                        const int      dest,
                        const int      tag,
                        const MPI_Comm comm ) const
    {
        const uint8_t n = bytes_for(args);
        Send<uint8_t>(n,dest,tag,comm);
        if(n>0)
        {
            const size_t swp = swap_le_as(args);
            Send<char>( (const char *)&swp, n, dest, tag, comm);
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
    
    size_t mpi:: RecvSize(const int      source,
                          const int      tag,
                          const MPI_Comm comm) const
    {
        const uint8_t n = Recv<uint8_t>(source,tag,comm);
        
        if(n>0)
        {
            if(n>sizeof(size_t)) throw upsylon::exception("RecvSize(too many bytes!)");
            size_t swp = 0;
            Recv<char>( (char *)&swp,n,source,tag,comm);
            return swap_le_as(swp);
        }
        else
        {
            return 0;
        }
    }
}
