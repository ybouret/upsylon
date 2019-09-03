#include "y/counting/mloop.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void do_mloop(const size_t dim, const array<T> &lo, const array<T> &hi)
    {
        assert(lo.size()==dim);
        assert(hi.size()==dim);
        mloop<T> loop(dim,*lo,*hi);

        vector< mloop<T> > loops(loop.count,as_capacity);

        for( loop.start(); loop.valid(); loop.next() )
        {
            loops.push_back_(loop);
            //std::cerr << "loop@" << loop.index << ": " << loop << std::endl;
        }
        std::cerr << "checking..." << std::endl;
        for(size_t i=1;i<=loops.size();++i)
        {
            Y_ASSERT(i==loops[i].index);
            //std::cerr << "@" << i << ": " << loops[i] << std::endl;
            
            for(size_t j=i;j<=loops.size();++j)
            {
                Y_ASSERT(j==loops[j].index);
                //std::cerr << "\t@" << j << ": " << loops[j] << std::endl;
                mloop<T>::memchk(loops[i],loops[j]);
            }
        }

    }

    template <typename T>
    void do_loops(const T a, const T b)
    {
        for(size_t dim=1;dim<=4;++dim)
        {
            vector<T> lo(dim,as_capacity);
            vector<T> hi(dim,as_capacity);
            for(size_t cycle=0;cycle<32;++cycle)
            {
                lo.free();
                hi.free();
                for(size_t i=1;i<=dim;++i)
                {
                    lo.push_back( alea.range<T>(a,b) );
                    hi.push_back( alea.range<T>(a,b) );
                }
                do_mloop<T>(dim,lo,hi);
            }

        }
    }

}


Y_UTEST(mloops)
{

    do_loops<uint8_t>(0,10);
    return 0;
    
    do_loops<int8_t>(-5,5);

    do_loops<uint16_t>(0,10);
    do_loops<int16_t>(-5,5);

    do_loops<size_t>(0,16);
    do_loops<unit_t>(-8,8);



}
Y_UTEST_DONE()
