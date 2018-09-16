#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    MPI.print0(stderr,"THREAD_LEVEL=%s\n\n", MPI.threadLevelText());

    if(MPI.isHead)
    {
        for(int r=1;r<MPI.size;++r)
        {
            const int    rank = MPI.Recv<int>(r, mpi::io_tag);
            MPI.print0(stderr, "recv from %d: %d\n", r, rank);
            const string node = MPI.Recv<string>(r,mpi::io_tag);
            MPI.print0(stderr, "recv from %d: %s\n", r, *node);
        }
    }
    else
    {
        MPI.Send(MPI.rank,    0,mpi::io_tag );
        MPI.Send(MPI.nodeName,0,mpi::io_tag);
    }

}
Y_UTEST_DONE()

