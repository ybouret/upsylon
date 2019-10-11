
#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/container/matrix.hpp"
#include "y/oxide/field2d.hpp"

#include "y/memory/pooled.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename TABLEAU>
    void fillTableau( TABLEAU &tab )
    {
        for(size_t i=1;i<=tab.rows;++i)
        {
            for(size_t j=1;j<=tab.cols;++j)
            {
                tab[i][j] = support::get< typename TABLEAU::mutable_type >();
            }
        }
    }

    template <typename T>
    static inline void doTest()
    {

        typedef matrix<T>         Matrix;
        typedef Oxide::Field2D<T> Field;
        typedef point2d<T>        P2D;
        typedef point3d<T>        P3D;

        {
            P2D v;
        }

        {
            P3D v;
        }

        for(size_t iter=0;iter<8;++iter)
        {
            const size_t nr = 1 + alea.leq(30);
            const size_t nc = 1 + alea.leq(30);
            vector<T,memory::global> rhs(nr,as_capacity);
            vector<T,memory::pooled> lhs(nc,as_capacity);

            Matrix M(nr,nc);
            Field  F( "F", Oxide::Coord2D(1,1), Oxide::Coord2D(nc,nr) );
            Y_ASSERT(F.rows==nr);
            Y_ASSERT(F.cols==nc);
            fillTableau(M);
        }

    }

}

Y_UTEST(atom)
{
    doTest<float>();
}
Y_UTEST_DONE()


