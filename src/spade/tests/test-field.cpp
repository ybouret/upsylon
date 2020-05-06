
#include "y/spade/field3d.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
 
    template <typename FIELD>
    static inline void doTest()
    {
        std::cerr << std::endl;
        
        typedef typename FIELD::LayoutType   LayoutType;
        typedef typename FIELD::coord        coord;
        typedef typename FIELD::Loop         Loop;
        typedef typename FIELD::mutable_type type;
        
        const coord      rng = 40 * Coord::Ones<coord>();
        const LayoutType L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );
        FIELD F(vformat("Field%uD<%s>",LayoutType::Dimensions, *type_name_of<type>()),L);
        std::cerr << F.name << " : " << F << std::endl;
        std::cerr << "\tlocalMemory=" << F.localMemory() << std::endl;
        std::cerr << "\tobjectBytes=" << F.objectBytes() << std::endl;
        
        std::cerr << "\tFill  Memory..." << std::endl;
        Loop         loop(F.lower,F.upper);
        vector<type> data(F.items,as_capacity);
        for(loop.boot();loop.good();loop.next())
        {
            F[ loop.value ] = support::get<type>();
            data.push_back( F[loop.value] );
        }
        
        std::cerr << "\tCheck Memory..." << std::endl;
        const FIELD &G = F;
        for(loop.boot();loop.good();loop.next())
        {
            Y_ASSERT( G[loop.value] == data[loop.index] );
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
