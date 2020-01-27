#include "y/oxide/field3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"


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
        const Layout1D L( Coord::Integer<Coord1D>(20, alea),  Coord::Integer<Coord1D>(20, alea) );
        const string   id = type_name_of<T>();
        Field1D<T>     F(id,L);
        std::cerr << "Field1D <" << F.name << ">: " << F << std::endl;
        std::cerr << "\tsizeof=" << sizeof(Field1D<T>) << std::endl;
        testAccess(F);
    }

    template <typename T>
    static inline void testField2D()
    {
        const Layout2D L( Coord::Integer<Coord2D>(20, alea),  Coord::Integer<Coord2D>(20, alea) );
        const string   id = type_name_of<T>();
        Field2D<T>     F(id,L);
        std::cerr << "Field2D <" << F.name << ">: " << F << std::endl;
        std::cerr << "\tsizeof=" << sizeof(Field2D<T>) << std::endl;
        testAccess(F);
    }


    template <typename T>
    static inline void testField3D()
    {
        const Layout3D L( Coord::Integer<Coord3D>(20, alea),  Coord::Integer<Coord3D>(20, alea) );
        const string   id = type_name_of<T>();
        Field3D<T>     F(id,L);
        std::cerr << "Field3D <" << F.name << ">: " << F << std::endl;
        std::cerr << "\tsizeof=" << sizeof(Field3D<T>) << std::endl;
        testAccess(F);
    }

}


Y_UTEST(oxide_fields)
{

    for(size_t iter=0;iter<16;++iter)
    {
        testField1D<unsigned>();
        testField1D<double>();
        testField1D<string>();
        testField1D<mpq>();
    }
    std::cerr << std::endl;

    for(size_t iter=0;iter<4;++iter)
    {
        testField2D<double>();
        testField2D<string>();
        testField2D<mpq>();
    }
    std::cerr << std::endl;

    for(size_t iter=1;iter<2;++iter)
    {
        testField3D<double>();
        testField3D<string>();
        testField3D<mpq>();
    }

}
Y_UTEST_DONE()

