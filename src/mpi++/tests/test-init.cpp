
#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
#if 0
    std::cerr << MPI.threadLevelText() << std::endl;
    std::cerr << MPI.processorName << std::endl;
    std::cerr << MPI.nodeName      << std::endl;
#endif
    
    if( MPI.parallel )
    {
        
        if(MPI.head)
        {
            for(int rank=1;rank<MPI.size;++rank)
            {
                const int r = MPI.Recv<int>(rank);
                std::cerr << "recv from " << rank << " : " << r << std::endl;
                MPI.Send<int64_t>(r,r);
            }
        }
        else
        {
            MPI.Send(MPI.rank,0);
            const int64_t r = MPI.Recv<int64_t>(0);
            Y_ASSERT(r==MPI.rank);
        }
    }
    else
    {
        
    }
    
    
}
Y_UTEST_DONE()

