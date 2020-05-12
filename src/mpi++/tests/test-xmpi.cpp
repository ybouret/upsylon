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
    
    static inline void doBuffer(mpi &MPI)
    {
        XMPI::Buffer s,r;
        
        // star
        if(MPI.head)
        {
            for(int rank=1;rank<MPI.size;++rank)
            {
                s.free();
                for(size_t i=alea.leq(100);i>0;--i)
                {
                    s.push_back( alea.full<uint8_t>() );
                }
                XMPI::vSend(MPI, s, rank, upsylon::comms::flexible_block_size);
                r.adjust(s.size(), 0);
                XMPI::vRecv(MPI, r, rank, comms::computed_block_size);
                for(size_t i=s.size();i>0;--i)
                {
                    Y_ASSERT(s[i]==r[i]);
                }
            }
        }
        else
        {
            XMPI::vRecv(MPI, r, 0, comms::flexible_block_size);
            XMPI::vSend(MPI, r, 0, comms::computed_block_size);
        }
        
        //ring
        s.free();
        r.free();
        const int next = MPI.nextRank();
        const int prev = MPI.prevRank();
        s.free();
        for(size_t i=alea.leq(100);i>0;--i)
        {
            s.push_back( alea.full<uint8_t>() );
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
        doBuffer(MPI);
        MPI.Printf(stderr, "send: %lu | recv: %lu\n", (unsigned long) MPI.commSend.data.full, (unsigned long) MPI.commRecv.data.full );
    }
    
}
Y_UTEST_DONE();

