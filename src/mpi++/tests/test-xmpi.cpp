#include "y/mpi++/xmpi.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
using namespace  upsylon;

Y_UTEST(xmpi)
{
    Y_MPI(SINGLE);
    
    if(MPI.parallel)
    {
        if(MPI.head)
        {
            vector<int> iv;
            list<int>   il;
            for(size_t i=alea.leq(10)+1;i>0;--i)
            {
                iv.push_back( alea.full<int>() );
                il.push_back( iv.back() );
            }
            
            vector<int> riv;
            list<int>   ril;
            
            for(int rank=1;rank<MPI.size;++rank)
            {
                XMPI::Send(MPI, rank, iv);
                XMPI::Send(MPI, rank, il);
                
                riv.free();
                ril.free();
                XMPI::Recv(MPI, rank, ril);
                XMPI::Recv(MPI, rank, riv);
            }
            
                     
            Y_ASSERT(iv.size()== il.size());
            Y_ASSERT(iv.size()==riv.size());
            Y_ASSERT(iv.size()==ril.size());

        }
        else
        {
            vector<int> iv;
            list<int>   il;
            XMPI::Recv(MPI, 0, iv);
            XMPI::Recv(MPI, 0, il);
            Y_ASSERT(iv.size()== il.size());

            XMPI::Send(MPI, 0, iv);
            XMPI::Send(MPI, 0, il);
        }
        
        MPI.Printf(stderr, "send: %lu | recv: %lu\n", (unsigned long) MPI.commSend.data.full, (unsigned long) MPI.commRecv.data.full );
    }
    
}
Y_UTEST_DONE();

