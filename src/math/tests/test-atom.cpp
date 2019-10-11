
#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/container/matrix.hpp"
#include "y/oxide/field2d.hpp"

#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"
#include <iomanip>

#include "support.hpp"
#include "y/os/rt-clock.hpp"
#include "y/sort/sorted-sum.hpp"

#include <typeinfo>

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

    template <typename ARRAY> static inline
    void fill( ARRAY &tab )
    {
        for(size_t i=1;i<=tab.size();++i)
        {
           tab[i] = support::get< typename ARRAY::mutable_type >();
        }
    }

    template <typename LHS, typename RHS> static inline
    bool areEqual( const LHS &lhs, const RHS &rhs)
    {
        Y_ASSERT(lhs.size()==rhs.size());
        vector< typename LHS::mutable_type > delta(lhs.size(),as_capacity);
        for(size_t i=1;i<=lhs.size();++i)
        {
            typename LHS::const_type d  = lhs[i] - rhs[i];
            typename LHS::const_type d2 = d*d;

            delta.push_back_( d2 );
        }
        typename LHS::const_type s2 = sorted_sum(delta);
        return  (s2 <= 0);
    }


#define TEST_TICKS(OUTPUT,EXPR)   uint64_t OUTPUT = 0; \
do {\
const uint64_t ini=rt_clock::ticks(); \
EXPR;\
OUTPUT=rt_clock::ticks()-ini;\
} while(false)

#define SHOW_TICKS(NAME,TEXT) do { \
rt_clock clk;\
std::cerr << "\t@"#NAME << TEXT << ": " << clk(fullTicks)/clk(loopTicks) << std::endl;\
} while(false)

#define TEST1_V1(NAME,U) do { \
TEST_TICKS(fullTicks,(atom::NAME(U)));\
TEST_TICKS(loopTicks,(atom::NAME(U,loop)));\
SHOW_TICKS(NAME,"/1");\
} while(false)

#define TEST1_V2(NAME,U,V) do { \
TEST_TICKS(fullTicks,(atom::NAME(U,V)));\
TEST_TICKS(loopTicks,(atom::NAME(U,V,loop)));\
SHOW_TICKS(NAME,"/2");\
} while(false)

#define TEST1_V3(NAME,U,V,W) do { \
TEST_TICKS(fullTicks,(atom::NAME(U,V,W)));\
TEST_TICKS(loopTicks,(atom::NAME(U,V,W,loop)));\
SHOW_TICKS(NAME,"/3");\
} while(false)

#define TEST_LD(PFX) do { atom::ld( PFX##0, 0 ); atom::ld( PFX##1, 0 ); } while(false)
#define TEST1_EQ2(NAME,PFX,RHS)     do { TEST_LD(PFX); atom::NAME(PFX##0,RHS);     atom::NAME(PFX##1,RHS,loop);     TEST_EQ(PFX##0,PFX##1,NAME); } while(false)
#define TEST1_EQ3(NAME,PFX,LHS,RHS) do { TEST_LD(PFX); atom::NAME(PFX##0,LHS,RHS); atom::NAME(PFX##1,LHS,RHS,loop); TEST_EQ(PFX##0,PFX##1,NAME); } while(false)

#define TEST_EQ(A,B,OP) std::cerr << "\t" #OP " : "; Y_CHECK(areEqual(A,B));

    template <typename ARR, typename BRR> static inline
    void doTest1(ARR &u,
                 BRR &v,
                 concurrent::for_each &loop)
    {
        std::cerr << "<Testing with " << typeid(ARR).name() << " & " << typeid(BRR).name() << ">" << std::endl;
        typedef typename ARR::mutable_type type;
        const type   value = support::get<type>();

        vector< typename ARR::mutable_type> U0(u.size());
        vector< typename ARR::mutable_type> U1(u.size());
        vector< typename BRR::mutable_type> V0(v.size());
        vector< typename BRR::mutable_type> V1(v.size());


        {
            TEST1_V2(ld,u,value);
            TEST1_V2(ld,v,value);
            TEST_EQ(u,v,ld);
        }

        {
            fill(u); TEST1_V2(set,u,u);
            fill(v); TEST1_V2(set,u,v);
            fill(u); TEST1_EQ2(set,U,u);
            fill(v); TEST1_EQ2(set,V,v);
        }

        {
            fill(u); TEST1_V1(neg,u);
            fill(v); TEST1_V1(neg,v);
            fill(v); TEST1_V2(neg,u,v);
            fill(u); TEST1_V2(neg,v,u);
            fill(u); TEST1_EQ2(neg,U,u);
            fill(v); TEST1_EQ2(neg,V,v);
        }

        {
            fill(u); fill(v);  TEST1_V2(add,u,v); TEST1_V3(add,U0,u,v);
            fill(v); fill(u);  TEST1_V2(add,v,u); TEST1_V3(add,U0,v,u);

            fill(u); fill(v);  TEST1_EQ2(add,U,u); TEST1_EQ3(add,U,u,v);
            fill(u); fill(v);  TEST1_EQ2(add,V,v); TEST1_EQ3(add,V,v,u);
        }


        {
            fill(u); fill(v);  TEST1_V2(sub,u,v); TEST1_V3(sub,U0,u,v);
            fill(v); fill(u);  TEST1_V2(sub,v,u); TEST1_V3(sub,U0,v,u);

            fill(u); fill(v);  TEST1_EQ2(sub,U,u); TEST1_EQ3(sub,U,u,v);
            fill(u); fill(v);  TEST1_EQ2(sub,V,v); TEST1_EQ3(sub,V,v,u);
        }





        std::cerr << "<Testing>" << std::endl << std::endl;
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
            const size_t n = 1000 + alea.leq(1000);
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

    doTest<float>(seq,par);
    doTest<short>(seq,par);
}
Y_UTEST_DONE()


