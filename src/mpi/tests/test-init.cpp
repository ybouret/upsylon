#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    MPI.print0(stderr,"THREAD_LEVEL=%s\n\n", MPI.threadLevelText());

    if(MPI.parallel)
    {
        if(MPI.isHead)
        {
            for(int r=1;r<MPI.size;++r)
            {
                const int    rank = MPI.Recv<int>(r, mpi::io_tag);
                MPI.print0(stderr, "recv from %d: %d\n", r, rank);
                const size_t sz   = MPI.RecvSize(r,mpi::io_tag); Y_ASSERT( sz == size_t(square_of(r)) );
                const string node = MPI.Recv<string>(r,mpi::io_tag);
                MPI.print0(stderr, "recv from %d: '%s'\n", r, *node);
            }
        }
        else
        {
            MPI.Send<int>(MPI.rank,    0,mpi::io_tag );
            MPI.SendSize(square_of(MPI.rank),0,mpi::io_tag);
            MPI.Send<string>(MPI.nodeName,0,mpi::io_tag);
        }
    }


    if(MPI.isHead)
    {
        int self = MPI.rank;
        for(int r=1;r<MPI.size;++r)
        {
            const int    peer = MPI.SendRecv(self, r, mpi::io_tag, r, mpi::io_tag);
            MPI.print0(stderr,"sendrecv: %d\n",peer);
            Y_ASSERT(r==peer);
            const int    sz = MPI.SendRecvSizes(0, r, mpi::io_tag, r, mpi::io_tag);
            MPI.print0(stderr, "sendrecv size: %u\n", unsigned(sz) );
            Y_ASSERT(r==sz);
        }
    }
    else
    {
        const int self = MPI.rank;
        const int host = MPI.SendRecv(self, 0, mpi::io_tag, 0, mpi::io_tag);
        Y_ASSERT(host==0);
        const size_t sz = MPI.SendRecvSizes(self, 0, mpi::io_tag, 0, mpi::io_tag);
        Y_ASSERT(sz==0);

    }

    int value = int(alea.leq(100));
    MPI.print(stderr, "initial value=%d\n", value);
    MPI.Bcast(value,0);
    MPI.print(stderr, "final   value=%d\n", value);

    {
        const double value = alea.to<double>();
        double       sum   = MPI.Reduce(value, MPI_SUM, 0);
        MPI.print(stderr, "    reduce sum: %g -> %g\n", value, sum);
        sum   = MPI.Allreduce(value, MPI_SUM);
        MPI.print(stderr, "all reduce sum: %g -> %g\n", value, sum);
    }
    MPI.print(stderr,"I/O time: %gms\n", MPI.getCommMilliseconds() );

    MPI.print(stderr,"Checking Data Type\n");
    (void) MPI.get_data_type_for<char>();
    (void) MPI.get_data_type_for<short>();
    (void) MPI.get_data_type_for<int>();
    (void) MPI.get_data_type_for<long>();
    (void) MPI.get_data_type_for<long long>();
    (void) MPI.get_data_type_for<ptrdiff_t>();
    (void) MPI.get_data_type_for<int8_t>();
    (void) MPI.get_data_type_for<int16_t>();
    (void) MPI.get_data_type_for<int32_t>();
    (void) MPI.get_data_type_for<int64_t>();

    (void) MPI.get_data_type_for<unsigned char>();
    (void) MPI.get_data_type_for<unsigned short>();
    (void) MPI.get_data_type_for<unsigned int>();
    (void) MPI.get_data_type_for<unsigned long>();
    (void) MPI.get_data_type_for<unsigned long long>();
    (void) MPI.get_data_type_for<size_t>();
    (void) MPI.get_data_type_for<uint8_t>();
    (void) MPI.get_data_type_for<uint16_t>();
    (void) MPI.get_data_type_for<uint32_t>();
    (void) MPI.get_data_type_for<uint64_t>();

    (void) MPI.get_data_type_for<float>();
    (void) MPI.get_data_type_for<double>();
    MPI.print(stderr,"Checking Data Type: OK\n");
}
Y_UTEST_DONE()

