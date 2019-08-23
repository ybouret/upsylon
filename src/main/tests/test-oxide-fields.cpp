

#include "y/oxide/field2d.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;
using namespace Oxide;

namespace {
#if 1
    template <typename FIELD>
    static inline
    void testAccess( FIELD &F )
    {
        for(size_t i=F.items*8;i>0;--i)
        {
            const typename FIELD::coord C = F.rand(alea);
            const typename FIELD::type  tmp = support::get<typename FIELD::mutable_type>();
            F(C) = tmp;
        }
        
    }

    template <typename T>
    static inline void testField1D()
    {
        const Layout1D L( CoordOps::Integer<Coord1D>(20, alea),  CoordOps::Integer<Coord1D>(20, alea) );
        const string   id = typeid(T).name();
        Field1D<T>     F(id,L);
        std::cerr << "Field1D <" << F.name << ">: " << F << std::endl;
        std::cerr << "sizeof=" << sizeof(Field1D<T>) << std::endl;
        testAccess(F);
    }

    template <typename T>
    static inline void testField2D()
    {
        const Layout2D L( CoordOps::Integer<Coord2D>(20, alea),  CoordOps::Integer<Coord2D>(20, alea) );
        const string   id = typeid(T).name();
        Field2D<T>     F(id,L);
        std::cerr << "Field2D <" << F.name << ">: " << F << std::endl;
        std::cerr << "sizeof=" << sizeof(Field2D<T>) << std::endl;
        testAccess(F);
    }
#endif

}


Y_UTEST(oxide_fields)
{
#if 1
    testField1D<double>();
    testField1D<string>();
    testField1D<mpq>();

    testField2D<double>();
    testField2D<string>();
    testField2D<mpq>();
#endif
}
Y_UTEST_DONE()

