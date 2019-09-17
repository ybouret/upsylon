#include "y/mpi/mpi.hpp"

#include <cstdarg>

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    double mpi:: getCommMilliseconds()
    {
        rt_clock     clk;
        const double thisTime = clk( fullCommTicks );
        const double maxiTime = Allreduce<double>(thisTime,MPI_MAX);
        return maxiTime * 1000.0;
    }

    void mpi:: update(data_type_cache &cache,
                      MPI_Datatype     value)
    {
        if(cache.type != value )
        {
            cache.size = sizeOf(value);
            cache.type = value;
        }
    }

#define Y_MPI_TICKS() fullCommTicks += (lastCommTicks=rt_clock::ticks() - mark)

#define Y_MPI_COMMS(CACHE,COUNT)

    void mpi::Send(const void        *buffer,
                   const size_t       count,
                   const MPI_Datatype type,
                   const int          target,
                   const int          tag)
    {
        assert(!(0==buffer&&count>0));
        update(send,type);
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Send((void*)buffer, int(count), type, target, tag, MPI_COMM_WORLD) );
        Y_MPI_TICKS();
    }

    void mpi:: Recv(void              *buffer,
                    const size_t       count,
                    const MPI_Datatype type,
                    const int          source,
                    const int          tag)
    {
        assert(!(0==buffer&&count>0));
        update(recv,type);
        const uint64_t mark = rt_clock::ticks();
        MPI_Status     status;
        Y_MPI_CHECK(MPI_Recv(buffer, int(count), type, source, tag, MPI_COMM_WORLD, &status) );
        Y_MPI_TICKS();
    }

    void mpi:: Bcast(void              *buffer,
                     const size_t       count,
                     const MPI_Datatype type,
                     const int          root)
    {
        assert(!(0==buffer&&count>0));
        update(coll,type);
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Bcast(buffer,int(count),type,root,MPI_COMM_WORLD));
        Y_MPI_TICKS();
    }

    void mpi:: Reduce(const void  * send_data,
                      void        * recv_data,
                      const size_t  count,
                      MPI_Datatype  type,
                      MPI_Op       op,
                      const int    root )
    {
        assert(!(0==send_data&&count>0));
        const uint64_t mark = rt_clock::ticks();
        update(coll,type);
        Y_MPI_CHECK(MPI_Reduce(send_data, recv_data,count,type, op, root, MPI_COMM_WORLD));
        Y_MPI_TICKS();
    }

    void mpi:: Allreduce(const void  * send_data,
                         void        * recv_data,
                         const size_t  count,
                         MPI_Datatype  type,
                         MPI_Op        op)
    {
        assert(!(0==send_data&&count>0));
        const uint64_t mark = rt_clock::ticks();
        update(coll,type);
        Y_MPI_CHECK(MPI_Allreduce(send_data, recv_data,count,type, op, MPI_COMM_WORLD));
        Y_MPI_TICKS();
    }

    void mpi:: SendRecv(const void        *sendbuf,
                        const size_t       sendcount,
                        const MPI_Datatype sendtype,
                        const int          target,
                        const int          sendtag,
                        void              *recvbuf,
                        const size_t       recvcount,
                        const MPI_Datatype recvtype,
                        const int          source,
                        const int          recvtag)
    {
        const uint64_t mark = rt_clock::ticks();
        update(send,sendtype);
        update(recv,recvtype);
        MPI_Status status;
        Y_MPI_CHECK(MPI_Sendrecv((void*)sendbuf, int(sendcount), sendtype, target, sendtag,
                                 recvbuf,        int(recvcount), recvtype, source, recvtag,
                                 MPI_COMM_WORLD, &status) );
        Y_MPI_TICKS();
    }

    void mpi:: Barrier()
    {
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
        Y_MPI_TICKS();
        lastCommBytes = 0;
    }

}




