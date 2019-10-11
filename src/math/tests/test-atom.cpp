
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

#define Y_ATOM_TICKS(OUTPUT,CODE) \
uint64_t OUTPUT=0;  do { const uint64_t ini = rt_clock::ticks(); CODE; OUTPUT=rt_clock::ticks()-ini; } while(false)
    
#define Y_ATOM_EQ(X,Y,NAME) do { \
static const bool NAME##_sanity = areEqual(X,Y); \
std::cerr << "\t" #NAME << "."; Y_CHECK(  NAME##_sanity ); } while(false)
    
#define Y_ATOM_OUT(NAME,ARITY) do {\
rt_clock clk; std::cerr << "\t"#NAME << "." << ARITY << ".speedup=" << clk(fullTicks)/clk(loopTicks) << std::endl;\
} while(false)
    
#define Y_ATOM_TEST1(NAME) do { \
fill(a); for(size_t i=a.size();i>0;--i) b[i] = a[i]; \
Y_ATOM_TICKS(fullTicks,atom::NAME(a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(b,loop));\
Y_ATOM_EQ(a,b,NAME);\
Y_ATOM_OUT(NAME,1);\
} while(false)
   
#define Y_ATOM_TEST2(NAME) do { \
fill(A); fill(a);\
for(size_t i=a.size();i>0;--i) { B[i]=A[i]; b[i]=a[i]; }\
Y_ATOM_TICKS(fullTicks,atom::NAME(A,a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(B,b,loop));\
Y_ATOM_EQ(a,b,NAME);\
Y_ATOM_OUT(NAME,2);\
} while(false)

    
    template <typename ARR, typename BRR>
    void Test1( ARR &a, BRR &b, concurrent::for_each &loop )
    {
        typedef typename ARR::mutable_type type;
        vector<type> A(a.size());
        vector<type> B(b.size());
        
        Y_ASSERT(a.size()==b.size());
        {
            const type value = support::get<type>();
            fill(a); Y_ATOM_TICKS(fullTicks,atom::ld(a,value));
            fill(b); Y_ATOM_TICKS(loopTicks,atom::ld(b,value,loop));
            Y_ATOM_EQ(a,b,ld);
            Y_ATOM_OUT(ld,1);
        }
        
        Y_ATOM_TEST1(neg);
        Y_ATOM_TEST2(neg);
        Y_ATOM_TEST2(set);
        Y_ATOM_TEST2(add);
        Y_ATOM_TEST2(sub);

        
        
    }


#define TEST1_PERM(A,B) Test1(A,B,loop); Test1(B,A,loop)
    
    template <typename T>
    static inline void doTest(concurrent::for_each &loop)
    {

        //typedef matrix<T>         Matrix;
        //typedef Oxide::Field2D<T> Field;
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
            TEST1_PERM(u,v);
            TEST1_PERM(u,gv);
            TEST1_PERM(u,pv);
            TEST1_PERM(u,gl);
        }

        {
            std::cerr << "3D" << std::endl;
            const size_t             n = 3;
            vector<T,memory::global> gv(n); Y_ASSERT( n == gv.size() );
            vector<T,memory::pooled> pv(n); Y_ASSERT( n == pv.size() );
            list<T>                  gl(n); Y_ASSERT( n == gl.size() );
            P3D u;
            P3D v;
            TEST1_PERM(u,v);
            TEST1_PERM(u,gv);
            TEST1_PERM(u,pv);
            TEST1_PERM(u,gl);
        }

        {
            std::cerr << "Big" << std::endl;
            const size_t n = 1000 + alea.leq(1000);
            vector<T,memory::global> gv(n); Y_ASSERT( n == gv.size() );
            vector<T,memory::pooled> pv(n); Y_ASSERT( n == pv.size() );
            list<T>                  gl(n); Y_ASSERT( n == gl.size() );
            TEST1_PERM(gv,pv);
            TEST1_PERM(gv,gl);
            TEST1_PERM(pv,gl);
        }

        
        
        
    }

}

Y_UTEST(atom)
{
    concurrent::simd loop;
    std::cerr << "loop.size=" << loop.engine().num_threads() << std::endl;

    doTest<float>(loop);
    doTest<short>(loop);
}
Y_UTEST_DONE()


