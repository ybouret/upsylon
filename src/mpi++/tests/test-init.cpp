
#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define ITEMS 1000

Y_UTEST(init)
{
    Y_MPI(SINGLE);
    
    
    MPI.Printf0(stderr, "ThreadLevel=%s\n",MPI.threadLevelText());
    
    if( MPI.parallel )
    {
        static const size_t sz[] = { 0x00, 0xab, 0xab00, 0xab0000, 0xab000000 };
        long         data[ITEMS];
        if(MPI.head)
        {
            for(size_t i=0;i<ITEMS;++i) data[i] = alea.full<long>();
            
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
                MPI.Send(data,ITEMS,rank);
                long wksp[ITEMS];
                MPI.Recv(wksp,ITEMS,rank);
                for(size_t i=0;i<ITEMS;++i)
                {
                    Y_ASSERT(wksp[i]==data[i]);
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
            long wksp[ITEMS] = { 0 };
            MPI.Recv(wksp,ITEMS,0);
            MPI.Send(wksp,ITEMS,0);
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

