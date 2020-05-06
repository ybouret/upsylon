
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
        typedef typename FIELD::LayoutType   LayoutType;
        typedef typename FIELD::coord        coord;
        typedef typename FIELD::Loop         Loop;
        typedef typename FIELD::mutable_type type;
        
        const coord      rng = 100 * Coord::Ones<coord>();
        const LayoutType L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );
        FIELD F("hello",L);
        std::cerr << F << std::endl;
        std::cerr << F.name << " : " << F.objectType.name() << std::endl;
      
        Loop  loop(F.lower,F.upper);
        for(loop.boot();loop.good();loop.next())
        {
            F[ loop.value ] = support::get<type>();
        }
        
    }
    
    template <typename T>
    static inline void doAll()
    {
        doTest< Field1D<T> >();
        doTest< Field2D<T> >();
    }
    
}

Y_UTEST(field)
{
    
    doAll<int>();
    doAll<string>();
    doAll<mpn>();
}
Y_UTEST_DONE()
