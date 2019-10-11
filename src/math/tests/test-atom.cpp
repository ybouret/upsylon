
#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/container/matrix.hpp"
#include "y/oxide/field2d.hpp"

#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename TABLEAU> static inline
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

    template <typename ARR, typename BRR> static inline
    void doTest1(ARR &u,
                 BRR &v,
                 concurrent::for_each &loop)
    {
        typedef typename ARR::mutable_type type;
        const type value = support::get<type>();

        std::cerr << "-- Test1 [";
        {
            std::cerr << ".";
            atom::ld(u,value);
            atom::ld(v,value);
            atom::ld(u,value,loop);
            atom::ld(v,value,loop);
        }

        {
            std::cerr << ".";
            atom::set(u,v);
            atom::set(v,u);
            atom::set(u,u);
            atom::set(v,v);

            atom::set(u,v,loop);
            atom::set(v,u,loop);
            atom::set(u,u,loop);
            atom::set(v,v,loop);
        }

        {
            std::cerr << ".";
            atom::neg(u,u);
            atom::neg(u,v);
            atom::neg(v,v);
            atom::neg(v,u);

            atom::neg(u,u,loop);
            atom::neg(u,v,loop);
            atom::neg(v,v,loop);
            atom::neg(v,u,loop);
        }

        std::cerr << "]" << std::endl;

    }


#define DO_TEST1(U,V) do { doTest1(U,V,seq); doTest1(U,V,par);} while(false)

    template <typename T>
    static inline void doTest(concurrent::for_each &seq,
                              concurrent::for_each &par)
    {

        typedef matrix<T>         Matrix;
        typedef Oxide::Field2D<T> Field;
        typedef point2d<T>        P2D;
        typedef point3d<T>        P3D;

        {
            std::cerr << "2D: " << std::endl;
            const size_t n = 2;
            vector<T,memory::global> gv(n); Y_ASSERT( n == gv.size() );
            vector<T,memory::pooled> pv(n); Y_ASSERT( n == pv.size() );
            list<T>                  gl(n); Y_ASSERT( n == gl.size() );
            P2D u;
            P2D v;

            DO_TEST1(u,v);
            DO_TEST1(u,gv);
            DO_TEST1(u,pv);
            DO_TEST1(u,gl);

            std::cerr << u << std::endl;
            std::cerr << v << std::endl;

        }

        {
            std::cerr << "3D" << std::endl;
            const size_t             n = 3;
            vector<T,memory::global> gv(n); Y_ASSERT( n == gv.size() );
            vector<T,memory::pooled> pv(n); Y_ASSERT( n == pv.size() );
            list<T>                  gl(n); Y_ASSERT( n == gl.size() );
            P3D u;
            P3D v;
            DO_TEST1(u,v);
            DO_TEST1(u,gv);
            DO_TEST1(u,pv);
            DO_TEST1(u,gl);

            std::cerr << u << std::endl;
            std::cerr << v << std::endl;
        }

        {
            std::cerr << "Big" << std::endl;
            const size_t n = 1000 + alea.leq(10000);
            vector<T,memory::global> gv(n); Y_ASSERT( n == gv.size() );
            vector<T,memory::pooled> pv(n); Y_ASSERT( n == pv.size() );
            list<T>                  gl(n); Y_ASSERT( n == gl.size() );
            DO_TEST1(gv,pv);
            DO_TEST1(gv,gl);
            DO_TEST1(pv,gl);

        }


        std::cerr << "generic" << std::endl;
        for(size_t iter=0;iter<8;++iter)
        {
            const size_t nr = 1 + alea.leq(30);
            const size_t nc = 1 + alea.leq(30);
            vector<T,memory::global> rhs(nr);
            vector<T,memory::pooled> rh2(nr);



            vector<T,memory::pooled> lhs(nc);
            vector<T,memory::global> lh2(nc);




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
    concurrent::simd           par;
    concurrent::sequential_for seq;

    std::cerr << "par.size=" << par.engine().num_threads() << std::endl;
    std::cerr << "seq.size=" << seq.engine().num_threads() << std::endl;

    doTest<short>(seq,par);
    doTest<float>(seq,par);
}
Y_UTEST_DONE()


