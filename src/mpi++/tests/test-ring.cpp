

#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {
    
    template <typename T>
    void doRing(const mpi &MPI, const size_t n)
    {
        MPI.Printf0(stderr, "-- doRing<%s>\n", typeid(T).name() );
        if(MPI.parallel)
        {
            vector<T> toSend(n,0);
            vector<T> toRecv(n,0);
            for(size_t i=1;i<=n;++i)
            {
                toSend[i] = support::get<T>();
            }
            const int dst = MPI.nextRank();
            const int src = MPI.prevRank();
            MPI.Sendrecv(&toSend[1], n, dst, &toRecv[1], n, src);
            MPI.Sendrecv(&toSend[1], n, src, &toRecv[1], n, dst);
            
            T       here  = support::get<T>();
            const T there = MPI.Sendrecv<T,T>(here,dst,src);
            here          = MPI.Sendrecv<T, T>(there,src,dst);
        }
    }
    
    
}

Y_UTEST(ring)
{
    Y_MPI(SINGLE);
    if(MPI.parallel)
    {
        size_t n = 1024;
        doRing<int>(MPI,n);
        doRing<char>(MPI,n);
        doRing<uint64_t>(MPI,n);
        const int dst = MPI.nextRank();
        const int src = MPI.prevRank();
        {
            MPI.Printf0(stderr, "-- doRing/Sizes\n" );
            static const size_t sz[] = { 0x00, 0xab, 0xab00, 0xab0000, 0xab000000 };
           
            for(size_t i=0;i<sizeof(sz)/sizeof(sz[0]);++i)
            {
                Y_ASSERT(MPI.SendrecvSizes(sz[i], dst, src)==sz[i]);
                Y_ASSERT(MPI.SendrecvSizes(sz[i], src, dst)==sz[i]);
            }
        }
        
        {
            MPI.Printf0(stderr, "-- doRing/strings\n" );
            string s;
            {
                const string r = MPI.Sendrecv<string,string>(s,dst,src);
                Y_ASSERT(r==s);
            }
            
            {
                const string r = MPI.Sendrecv<string,string>(s,src,dst);
                Y_ASSERT(r==s);
            }
            
            for(int iter=0;iter<10;++iter)
            {
                for(char i='a';i<='z';++i)
                {
                    s << i;
                    {
                        const string r = MPI.Sendrecv<string,string>(s,dst,src);
                        Y_ASSERT(r==s);
                    }
                    {
                        const string r = MPI.Sendrecv<string,string>(s,src,dst);
                        Y_ASSERT(r==s);
                    }
                }
            }
        }
        
        MPI.Printf(stderr, "send: %lu | recv: %lu\n", (unsigned long) MPI.commSend.data.full, (unsigned long) MPI.commRecv.data.full );
    }

}
Y_UTEST_DONE()
