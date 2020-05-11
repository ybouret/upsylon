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
        assert(dest>=0);
        assert(dest!=rank);
        assert(dest<size);
        
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
                        const int      dest) const
    {
        const uint8_t n = bytes_for(args);
        Send<uint8_t>(n,dest);
        if(n>0)
        {
            const size_t swp = swap_le_as(args);
            Send<char>( (const char *)&swp,n,dest);
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
        assert(source>=0);
        assert(source!=rank);
        assert(source<size);
        if(count>0)
        {
            const int items = static_cast<int>(count);
            if(items<0) throw exception(MPI_ERR_COUNT, "mpi::Recv(count overflow)");
            const uint64_t mark = rt_clock::ticks();
            Y_MPI_CHECK(MPI_Recv(buffer,items,datatype,source,tag,comm,&status));
            commRecv.ticks( rt_clock::ticks() - mark );
        }
    }
    
    size_t mpi:: RecvSize(const int      source) const
    {
        const uint8_t n = Recv<uint8_t>(source);
        
        if(n>0)
        {
            if(n>sizeof(size_t)) throw upsylon::exception("RecvSize(too many bytes!)");
            size_t swp = 0;
            Recv<char>( (char *)&swp,n,source);
            return swap_le_as(swp);
        }
        else
        {
            return 0;
        }
    }
    
    void mpi::Sendrecv(const void        *sendbuf,
                       const size_t       sendcount,
                       const MPI_Datatype sendtype,
                       const int          dest,
                       int                sendtag,
                       void              *recvbuf,
                       const size_t       recvcount,
                       const MPI_Datatype recvtype,
                       const int          source,
                       const int          recvtag,
                       const MPI_Comm     comm,
                       MPI_Status         &status) const
    {
        assert( !( (0==sendbuf) && (sendcount>0)) );
        assert(dest>=0);
        assert(dest!=rank);
        assert(dest<size);
        assert( !( (0==recvbuf) && (recvcount>0)) );
        assert(source>=0);
        assert(source!=rank);
        assert(source<size);
        const uint64_t mark  = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, &status) );
        const uint64_t delta = rt_clock::ticks()-mark;
        commRecv.ticks(delta);
        commSend.ticks(delta);
    }
    
    template <>
    void mpi::Send<string>(const string  &str,
                           const int      dest) const
    {
        const size_t n = str.size();
        if(n>0)
        {
            SendSize(n,dest);
            Send(*str,n,dest);
        }
        else
        {
            static const uint8_t z=0;
            Send(z,dest); //!< will trigger 0 size and no content
        }
    }
    
    template <>
    string mpi::Recv<string>(const int      source) const
    {
        const size_t n = RecvSize(source);
        if(n>0)
        {
            string str(n,as_capacity,true);
            Recv(*str,n,source);
            return str;
        }
        else
        {
            return string();
        }
    }
    
}
