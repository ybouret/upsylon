#include "y/mpi++/xmpi.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
using namespace  upsylon;

namespace
{
    template <typename T>
    static inline void doSequences(mpi &MPI)
    {
        vector<T> V;
        list<T>   L;
        if(MPI.head)
        {
            for(size_t i=alea.leq(10)+1;i>0;--i)
            {
                const T tmp = support::get<T>();
                V.push_back( tmp );
                L.push_back( V.back() );
            }
            for(int rank=1;rank<MPI.size;++rank)
            {
                XMPI::Send(MPI, rank, V);
                XMPI::Send(MPI, rank, L);
                
                V.free();
                L.free();
                XMPI::Recv(MPI, rank, L);
                XMPI::Recv(MPI, rank, V);
            }
        }
        else
        {
            XMPI::Recv(MPI, 0, V);
            XMPI::Recv(MPI, 0, L);
            Y_ASSERT(V.size()== L.size());
            
            XMPI::Send(MPI, 0, V);
            XMPI::Send(MPI, 0, L);
        }
    }
    
}

Y_UTEST(xmpi)
{
    Y_MPI(SINGLE);
    
    if(MPI.parallel)
    {
        doSequences<int>(MPI);
        doSequences<double>(MPI);
        doSequences<string>(MPI);

        
        MPI.Printf(stderr, "send: %lu | recv: %lu\n", (unsigned long) MPI.commSend.data.full, (unsigned long) MPI.commRecv.data.full );
    }
    
}
Y_UTEST_DONE();

