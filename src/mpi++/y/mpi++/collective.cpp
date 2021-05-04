#include "y/mpi++/mpi.hpp"
#include "y/os/real-time-clock.hpp"

namespace upsylon {
    
    void mpi:: Barrier() const
    {
        const uint64_t mark = real_time_clock::ticks();
        Y_MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
        const uint64_t delta = real_time_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
    void mpi:: Bcast(void *buffer, const size_t count, MPI_Datatype datatype, const int root, MPI_Comm comm) const
    {
        assert( !(NULL==buffer && count>0) );
        assert(root>=0);
        assert(root<size);
        const int nbcast = static_cast<int>(count);
        if(nbcast<0) throw upsylon::exception( "mpi::Bcast(size overflow)");
        const uint64_t mark = real_time_clock::ticks();
        Y_MPI_CHECK(MPI_Bcast(buffer,nbcast,datatype,root,comm));
        const uint64_t delta = real_time_clock::ticks() - mark;
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
    
    
    void mpi:: Reduce(const void*        send_data,
                      void *             recv_data,
                      const size_t       count,
                      const MPI_Datatype datatype,
                      const MPI_Op       op,
                      const int          root,
                      const MPI_Comm     comm) const
    {
        assert( !(send_data==NULL&&count>0) );
        assert( !(recv_data==NULL&&count>0) );
        assert(root>=0);
        assert(root<size);
        
        const int nred = static_cast<int>(count);
        if(nred<0) throw upsylon::exception( "mpi::Reduce(size overflow)");
        const uint64_t mark = real_time_clock::ticks();
        Y_MPI_CHECK(MPI_Reduce((void*)send_data, recv_data, nred, datatype, op, root, comm));
        const uint64_t delta = real_time_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
    void mpi:: Allreduce(const void*        send_data,
                         void *             recv_data,
                         const size_t       count,
                         const MPI_Datatype datatype,
                         const MPI_Op       op,
                         const MPI_Comm     comm) const
    {
        assert( !(send_data==NULL&&count>0) );
        assert( !(recv_data==NULL&&count>0) );
        const int nred = static_cast<int>(count);
        if(nred<0) throw upsylon::exception( "mpi::Allreduce(size overflow)");
        const uint64_t mark = real_time_clock::ticks();
        Y_MPI_CHECK(MPI_Allreduce((void*)send_data, recv_data, nred, datatype, op,comm));
        const uint64_t delta = real_time_clock::ticks() - mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
    
}
