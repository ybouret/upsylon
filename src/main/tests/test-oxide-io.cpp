#include "y/oxide/field3d.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include "y/ios/ovstream.hpp"

#include <typeinfo>

using namespace upsylon;
using namespace Oxide;

namespace
{
    
    template <typename FIELD>
    static inline void fill( FIELD &F )
    {
        for(size_t i=F.items/2;i>0;--i)
        {
            typename FIELD::const_coord c    = F.rand(alea);
            typename FIELD::const_type  tmp  = support::get< typename FIELD::mutable_type >();
            F(c) = tmp;
        }
    }

    template <typename T>
    static inline void run_with( ios::plugin &plg )
    {
        ios::ovstream io(1024*1024);


        for(size_t cycle=0;cycle<100;++cycle)
        {
            io.free();
            const Coord1D m = 20;
            Field1D<T> F1( "F1",  Coord::Integer<Coord1D>(m, alea),  Coord::Integer<Coord1D>(m, alea) );
            Field2D<T> F2( "F2",  Coord::Integer<Coord2D>(m, alea),  Coord::Integer<Coord2D>(m, alea) );
            Field3D<T> F3( "F3",  Coord::Integer<Coord3D>(m, alea),  Coord::Integer<Coord3D>(m, alea) );

            fill(F1);
            fill(F2);
            fill(F3);
            std::cerr << "F1=" << F1 << std::endl;
            std::cerr << "F2=" << F2 << std::endl;
            std::cerr << "F3=" << F3 << std::endl;

            const size_t n1 = F1.save(io,F1,F1,plg); Y_ASSERT(n1==io.size());
            const size_t n2 = F2.save(io,F2,F2,plg); Y_ASSERT(n1+n2==io.size());
            const size_t n3 = F3.save(io,F3,F3,plg); Y_ASSERT(n1+n2+n3==io.size());

            std::cerr << "#io=" << io.size() << "/" << io.capacity() << std::endl;
            
            
            {
                ios::imstream inp( io );
                const size_t r1 = F1.load(inp,F1,F1,plg); Y_ASSERT(r1==n1);
                const size_t r2 = F2.load(inp,F2,F2,plg); Y_ASSERT(r2==n2);
                const size_t r3 = F3.load(inp,F3,F3,plg); Y_ASSERT(r3==n3);
                std::cerr << "\treloaded..." << std::endl;
            }

            io.free();
            vector<Coord1D>                i1;
            vector<Coord1D,memory::pooled> i2;
            list<Coord1D>                  i3;
            {
                const Layout1D sub( F1.rand(alea), F1.rand(alea) );
                F1.collect(i1,sub);
                std::cerr << "#i1=" << i1.size() << std::endl;
            }
            {
                const Layout2D sub( F2.rand(alea), F2.rand(alea) );
                F2.collect(i2,sub);
                std::cerr << "#i2=" << i2.size() << std::endl;
            }
            {
                const Layout3D sub( F3.rand(alea), F3.rand(alea) );
                F3.collect(i3,sub);
                std::cerr << "#i3=" << i3.size() << std::endl;
            }


            F1.save(i1,io,plg);
            F2.save(i2,io,plg);
            F3.save(i3,io,plg);
            {
                ios::imstream fp(io);
                F1.load(i1,fp,plg);
                F2.load(i2,fp,plg);
                F3.load(i3,fp,plg);
                std::cerr << "\treloaded..." << std::endl;
            }



        }
    }
    
}

Y_UTEST(oxide_io)
{
    {
        ios::raw_plugin<double> plg;
        run_with<double>(plg);
    }
    {
        ios::nbo_plugin<float> plg;
        run_with<float>(plg);
    }

    {
        ios::srz_plugin<string> plg;
        run_with<string>(plg);
    }

}
Y_UTEST_DONE()
