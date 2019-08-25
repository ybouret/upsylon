

#include "y/oxide/field3d.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;
using namespace Oxide;

namespace {

    template <typename FIELD>
    static inline
    void testAccess( FIELD &F )
    {
        Y_ASSERT(F.entry);
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
        std::cerr << "\tsizeof=" << sizeof(Field1D<T>) << std::endl;
        testAccess(F);
    }

    template <typename T>
    static inline void testField2D()
    {
        const Layout2D L( CoordOps::Integer<Coord2D>(20, alea),  CoordOps::Integer<Coord2D>(20, alea) );
        const string   id = typeid(T).name();
        Field2D<T>     F(id,L);
        std::cerr << "Field2D <" << F.name << ">: " << F << std::endl;
        std::cerr << "\tsizeof=" << sizeof(Field2D<T>) << std::endl;
        testAccess(F);
    }


    template <typename T>
    static inline void testField3D()
    {
        const Layout3D L( CoordOps::Integer<Coord3D>(20, alea),  CoordOps::Integer<Coord3D>(20, alea) );
        const string   id = typeid(T).name();
        Field3D<T>     F(id,L);
        std::cerr << "Field3D <" << F.name << ">: " << F << std::endl;
        std::cerr << "\tsizeof=" << sizeof(Field3D<T>) << std::endl;
        testAccess(F);
    }

}


Y_UTEST(oxide_fields)
{
    testField1D<double>();
    testField1D<string>();
    testField1D<mpq>();
    std::cerr << std::endl;

    testField2D<double>();
    testField2D<string>();
    testField2D<mpq>();
    std::cerr << std::endl;

    testField3D<double>();
    testField3D<string>();
    testField3D<mpq>();

}
Y_UTEST_DONE()

