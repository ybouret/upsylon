
#include "y/spade/fields.hpp"
#include "y/spade/field/ops.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    static const Coord1D nmax[4] = { 0, 1000, 100, 22};

    template <typename FIELD>
    static inline void doTest()
    {
        std::cerr << std::endl;
        
        typedef typename FIELD::LayoutType   LayoutType;
        typedef typename FIELD::coord        coord;
        typedef typename FIELD::Loop         Loop;
        typedef typename FIELD::mutable_type type;

        static const size_t dims = FIELD::Dimensions;

        for(size_t iter=0;iter<16;++iter)
        {
            const coord      rng = nmax[dims] * Coord::Ones<coord>();
            const LayoutType L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );
            FIELD F(vformat("Field%uD<%s>",LayoutType::Dimensions, *type_name_of<type>()),L);
            std::cerr << F.name << " : " << F << std::endl;
            std::cerr << "\tlocalMemory=" << F.localMemory() << std::endl;
            std::cerr << "\tobjectBytes=" << F.objectBytes() << std::endl;
            std::cerr << "\tvirtualBits=" << F.virtualBits() << std::endl;
            std::cerr << "\tFill  Memory..." << std::endl;
            Loop         loop(F.lower,F.upper);
            vector<type> data(F.items,as_capacity);
            for(loop.boot();loop.good();loop.next())
            {
                F[ loop.value ] = support::get<type>();
                data.push_back( F[loop.value] );
                Y_ASSERT( F(loop.index-1) == data[loop.index] );
            }

            std::cerr << "\tCheck Memory..." << std::endl;
            const FIELD &G = F;
            for(loop.boot();loop.good();loop.next())
            {
                Y_ASSERT( G[loop.value]   == data[loop.index] );
                Y_ASSERT( G(loop.index-1) == data[loop.index] );
                Y_ASSERT( G( G.indexOf(loop.value) ) == G(loop.index-1) );
            }

            type vmin = Ops::Min(G,G);
            type vmax = Ops::Max(G,G);
            std::cerr << "\tMin... {" << vmin << "}" << std::endl;
            std::cerr << "\tMax... {" << vmax << "}" << std::endl;
            type vmin2 = vmax;
            type vmax2 = vmin;
            Ops::MinMax(vmin2,vmax2,G,G);
            std::cerr << "\tMin... {" << vmin2 << "}" << std::endl;
            std::cerr << "\tMax... {" << vmax2 << "}" << std::endl;

            std::cerr << "\tDone..." << std::endl;
        }
        
    }
    
    template <typename T>
    static inline void doAll()
    {
        doTest< Field1D<T> >();
        doTest< Field2D<T> >();
        doTest< Field3D<T> >();
    }
    
}

Y_UTEST(field)
{
    doAll<char>();
    doAll<int>();
    doAll<string>();
    doAll<mpn>();
}
Y_UTEST_DONE()
