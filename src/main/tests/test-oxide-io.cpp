#include "y/oxide/field3d.hpp"
#include "y/oxide/field/io.hpp"
#include "y/ios/imstream.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"

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
    static inline void run_with(FieldInfo::SaveProc save,
                                FieldInfo::LoadProc  load)
    {
        IO::Block io(1024*1024);


        for(size_t cycle=0;cycle<100;++cycle)
        {
            io.free();
            const Coord1D m = 20;
            Field1D<T> F1( "F1",  CoordOps::Integer<Coord1D>(m, alea),  CoordOps::Integer<Coord1D>(m, alea) );
            Field2D<T> F2( "F2",  CoordOps::Integer<Coord2D>(m, alea),  CoordOps::Integer<Coord2D>(m, alea) );
            Field3D<T> F3( "F3",  CoordOps::Integer<Coord3D>(m, alea),  CoordOps::Integer<Coord3D>(m, alea) );

            fill(F1);
            fill(F2);
            fill(F3);
            std::cerr << "F1=" << F1 << std::endl;
            std::cerr << "F2=" << F2 << std::endl;
            std::cerr << "F3=" << F3 << std::endl;

            F1.save(io,save);
            F2.save(io,save);
            F3.save(io,save);

            std::cerr << "#io=" << io.size() << "/" << io.capacity() << std::endl;
            
            
            if(load)
            {
                ios::imstream inp( io );
                F1.load(inp,load);
                F2.load(inp,load);
                F3.load(inp,load);
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

            F1.save_only(i1,io,save);
            F2.save_only(i2,io,save);
            F3.save_only(i3,io,save);
            if(load)
            {
                F1.load_only(i1,io,load);
                F2.load_only(i2,io,load);
                F3.load_only(i3,io,load);
                std::cerr << "\treloaded..." << std::endl;
            }



        }
    }
    
}

Y_UTEST(oxide_io)
{
    run_with<double>( IO::SaveBlock<double>       , IO::LoadBlock<double>       );
    run_with<float>(  IO::SaveIntegral<float>     , IO::LoadIntegral<float>     );
    run_with<string>( IO::SaveSerializable<string>, IO::LoadSerialized<string>  );

}
Y_UTEST_DONE()
