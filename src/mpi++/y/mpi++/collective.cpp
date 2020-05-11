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
    
    void mpi:: Bcast(void *buffer, const size_t count, MPI_Datatype datatype, const int root, MPI_Comm comm) const
    {
        assert( !(NULL==buffer && count>0) );
        const int nbcast = static_cast<int>(count);
        if(nbcast<0) throw upsylon::exception( "mpi::Bcast(size overflow)");
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Bcast(buffer,count,datatype,root,comm));
        const uint64_t delta = rt_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
    void mpi:: BcastSize(size_t &args, const int root) const
    {
        uint8_t n    = 0;
        size_t  temp = 0;
        if(rank==root)
        {
            temp = swap_le_as(args);
            n    = bytes_for(args);
        }
        Bcast(n,root);
        if(n>0)
        {
            if(n>sizeof(size_t)) throw upsylon::exception( "mpi::BcastSize(too many bytes!)");
            Bcast((char *)&temp,n,root);
            args = swap_le_as(temp);
        }
        else
        {
            args = 0;
        }

    }
    
    template <> void mpi::Bcast<string>(string &str, const int root) const
    {
        size_t len = str.size();
        BcastSize(len,root);
        str.adjust(len, 0);
        assert(str.size()==len);
        Bcast<char>(*str,len,root);
    }


    
}
