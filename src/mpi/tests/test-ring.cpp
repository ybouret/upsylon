#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/os/rt-clock.hpp"
#include "y/code/hr-ints.hpp"

using namespace upsylon;

namespace
{




    static inline
    size_t Ring(mpi            &MPI,
              const uint8_t   ln2blockSize,
              const size_t    cycles,
              array<int64_t> &speed)
    {
        MPI.Barrier();
        assert(speed.size()>=2);

        const size_t blockSize = (1<<ln2blockSize);
        MPI.print0(stderr, "Ring[%u]/%u\n", unsigned(blockSize), unsigned(cycles) );

        mpi::vBlock sndblk( blockSize, true);
        mpi::vBlock rcvblk( blockSize, true);

        const int rank = MPI.rank;
        const int prev = (rank<=0)        ? MPI.last : rank-1;
        const int next = (rank>=MPI.last) ? 0        : rank+1;

        uint64_t fwd = 0, rev=0;
        for(size_t cycle=0;cycle<cycles;++cycle)
        {

            // forward
            alea.fill(*sndblk,sndblk.size());
            MPI.SendRecv(*sndblk, blockSize, MPI_BYTE, next, mpi::io_tag,
                         *rcvblk, blockSize, MPI_BYTE, prev, mpi::io_tag);
            fwd += MPI.lastCommTicks;

            // reverse
            alea.fill(*sndblk,sndblk.size());
            MPI.SendRecv(*sndblk, blockSize, MPI_BYTE, prev, mpi::io_tag,
                         *rcvblk, blockSize, MPI_BYTE, next, mpi::io_tag);
            rev += MPI.lastCommTicks;
        }
        const double bytes = blockSize * cycles;
        rt_clock clk;
        speed[1] = int64_t(bytes/clk(fwd));
        speed[2] = int64_t(bytes/clk(rev));
        return blockSize;
    }

}

#include "y/ios/ocstream.hpp"

Y_UTEST(ring)
{

    Y_MPI(SINGLE);

    matrix<int64_t> speed(MPI.size,2);

    const string logfile = "ring.dat";
    ios::ocstream::overwrite(logfile);

    for(size_t ln2bs=8; ln2bs<=20; ++ln2bs)
    {
        const size_t bs = Ring(MPI,ln2bs,128,speed[MPI.rank+1]);

        if(MPI.parallel)
        {
            if(MPI.isHead)
            {
                for(int rank=1;rank<MPI.size;++rank)
                {
                    MPI.RecvAll(speed[rank+1], rank, mpi::io_tag);
                }
            }
            else
            {
                MPI.SendAll(speed[MPI.rank+1], 0, mpi::io_tag);
            }
        }

        if(MPI.isHead)
        {
            //std::cerr << speed << std::endl;
            ios::ocstream fp("ring.dat",true);
            fp("[blockSize=%u]\n", unsigned(bs) );
            for(int rank=0;rank<MPI.size;++rank)
            {
                const array<int64_t> &spd = speed[rank+1];
                const human_readable f( spd[1] );
                const human_readable r( spd[2] );
                const string data = vformat("\tnode%d | fwd: %7.2f%c/s | rev: %7.2f%c/s\n", rank, f.value, f.radix, r.value, r.radix);
                fp << data;
                fputs(*data, stderr);
            }
        }
        MPI.print0(stderr, "\n");

    }


}
Y_UTEST_DONE()
