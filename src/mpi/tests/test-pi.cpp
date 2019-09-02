#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/parops.hpp"
#include "y/os/rt-clock.hpp"

using namespace upsylon;

Y_UTEST(PI)
{
    Y_MPI(SINGLE);


    const uint64_t mark  = rt_clock::ticks();
    unsigned long length = 0x10000000; //!< count per proc
    unsigned long offset = 1;

    parops::split_any(length, offset, MPI.size, MPI.rank);

    MPI.print(stderr, "%lu->%lu\n", offset, offset+length-1);

    double sum = 0;
    while(length>0)
    {
        sum += 1.0/square_of(offset++);
        --length;
    }

    sum = sqrt(6.0 * MPI.Reduce(sum, MPI_SUM,0));
    const uint64_t dtt = rt_clock::ticks() - mark;
    rt_clock clk;
    const double   dt  = clk(dtt);

    MPI.print0(stderr,"PI=%.12g in %.3fms\n",sum, dt*1000.0);



}
Y_UTEST_DONE()


