#include "y/mpi++/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/type/point3d.hpp"

using namespace upsylon;


Y_UTEST(coll)
{
    Y_MPI(SINGLE);
    static const size_t sz[] = { 0x00, 0xab, 0xab00, 0xab0000, 0xab000000 };
    
    MPI.Printf0(stderr, "-- Bcast\n");
    for(int r=0;r<MPI.size;++r)
    {
        int rk = r;
        MPI.Bcast(rk,r);
        MPI.Printf0(stderr, "rk=%d\n", rk);
        Y_ASSERT(r==rk);
    }
    
    MPI.Printf0(stderr, "-- BcastSize\n");
    for(int r=0;r<MPI.size;++r)
    {
        for(unsigned i=0;i<sizeof(sz)/sizeof(sz[0]);++i)
        {
            size_t s = (r==MPI.rank) ? sz[i] : 0xffffffff;
            MPI.BcastSize(s,r);
            Y_ASSERT(s==sz[i]);
        }
        
        if(true)
        {
            string s;
            for(int iter=0;iter<2;++iter)
            {
                for(char i='a';i<='z';++i)
                {
                    if(r==MPI.rank)
                    {
                        s += i;
                    }
                    else
                    {
                        s.clear();
                    }
                    MPI.Bcast(s,r);
                    MPI.Printf(stderr, "'%s'\n",*s);
                }
            }
        }
        
    }
    
    double value = MPI.rank;
    double res   = MPI.Reduce(value,MPI_SUM, 0);
    MPI.Printf(stderr, "reduce0 #1: %g\n", res);
    
    point2d<float> v2( float(MPI.rank), float(MPI.rank) );
    point2d<float> r2 = MPI.Reduce2(v2,MPI_SUM,0);
    MPI.Printf(stderr, "reduce0 #2: %g %g\n", r2.x, r2.y);

    
    
}
Y_UTEST_DONE()
