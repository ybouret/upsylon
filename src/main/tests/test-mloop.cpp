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
        std::cerr << "Create MLoop..." << std::endl;
        core::mloop<T> loop(dim,*lo,*hi);

        vector< core::mloop<T> > loops(loop.count,as_capacity);

        for( loop.start(); loop.valid(); loop.next() )
        {
            loops.push_back_(loop);
            //std::cerr << "loop@" << loop.index << ": " << loop << std::endl;
        }
        std::cerr << "checking..." << std::endl;
        for(size_t i=1;i<=loops.size();++i)
        {
            Y_ASSERT(i==loops[i].index);
            for(size_t j=i;j<=loops.size();++j)
            {
                Y_ASSERT(j==loops[j].index);
                core::mloop<T>::memchk(loops[i],loops[j]);
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

    template <typename LOOP>
    static inline void doLoop( LOOP &loop )
    {
        vector< core::mloop<typename LOOP::type> > loops(loop.count,as_capacity);

        std::cerr << "<" << loop.lower << "->" << loop.upper << ">" << std::endl;
        for( loop.start(); loop.valid(); loop.next() )
        {
            std::cerr << "\t@" << loop.index << " : " << loop.value << " <= " << loop << std::endl;
            loops.push_back_(loop);
        }
        std::cerr << "<" << loop.lower << "->" << loop.upper << "/>" << std::endl;
        std::cerr << "checking..." << std::endl;
        for(size_t i=1;i<=loops.size();++i)
        {
            Y_ASSERT(i==loops[i].index);
            for(size_t j=i;j<=loops.size();++j)
            {
                Y_ASSERT(j==loops[j].index);
                core::mloop<typename LOOP::type>::memchk(loops[i],loops[j]);
            }
        }

    }


}

#include "y/type/point3d.hpp"

Y_UTEST(mloops)
{

    do_loops<uint8_t>(0,10);
    do_loops<int8_t>(-5,5);

    do_loops<uint16_t>(0,10);
    do_loops<int16_t>(-5,5);

    do_loops<size_t>(0,10);
    do_loops<unit_t>(-5,5);

    std::cerr << "named loops" << std::endl;
    {
        mloop<unit_t,unit_t> loop(-4,4);
        doLoop(loop);
        loop.reset(4,-4);
        doLoop(loop);
    }

    {
        typedef point2d<int> p2d;
        const p2d            ini(-2,-3);
        const p2d            end(1,4);
        mloop< int,p2d >     loop(ini,end);
        doLoop(loop);
        loop.reset(end,ini);
        doLoop(loop);
    }

    {
        typedef point3d<int16_t> p3d;
        const p3d                ini(-1,-2,-3);
        const p3d                end(4,5,6);
        mloop< int16_t,p3d >     loop(ini,end);
        doLoop(loop);
        loop.reset(end,ini);
        doLoop(loop);
    }

}
Y_UTEST_DONE()
