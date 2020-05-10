
#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    
    
    MPI.Printf0(stderr, "ThreadLevel=%s\n",MPI.threadLevelText());
    
    if( MPI.parallel )
    {
        static const size_t sz[] = { 0x00, 0xab, 0xab00, 0xab0000, 0xab000000 };
        
        if(MPI.head)
        {
            for(int rank=1;rank<MPI.size;++rank)
            {
                const int r = MPI.Recv<int>(rank);
                std::cerr << "recv from " << rank << " : " << r << std::endl;
                MPI.Send<int64_t>(r,r);
                for(size_t i=0;i<sizeof(sz)/sizeof(sz[0]);++i)
                {
                    MPI.SendSize(sz[i],rank);
                }
                string s;
                for(size_t l=0;l<16;++l)
                {
                    MPI.Send(s,rank);
                    s << alea.range<char>('a','b');
                }
            }
        }
        else
        {
            MPI.Send(MPI.rank,0);
            const int64_t r = MPI.Recv<int64_t>(0);
            Y_ASSERT(r==MPI.rank);
            for(size_t i=0;i<sizeof(sz)/sizeof(sz[0]);++i)
            {
                const size_t rs = MPI.RecvSize(0);
                Y_ASSERT(rs==sz[i]);
            }
            for(size_t l=0;l<16;++l)
            {
                const string s = MPI.Recv<string>(0);
                Y_ASSERT(s.size()==l);
            }
        }
    }
    else
    {
        
    }
    MPI.Printf(stderr,"ThreadLevel=%s\n",MPI.threadLevelText());
    MPI.Printf(stderr, "send: %lu | recv: %lu\n",
               (unsigned long)MPI.commSend.data.full,
               (unsigned long)MPI.commRecv.data.full);
    
}
Y_UTEST_DONE()

