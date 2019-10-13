
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
    void copyTo( LHS &lhs, const RHS &rhs )
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            lhs[i] = rhs[i];
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
fill(a); copyTo(b,a); \
Y_ATOM_TICKS(fullTicks,atom::NAME(a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(b,loop));\
Y_ATOM_EQ(a,b,NAME);\
Y_ATOM_OUT(NAME,1);\
} while(false)
    
#define Y_ATOM_TEST2(NAME) do { \
fill(A); copyTo(B,A);\
fill(a); copyTo(b,a);\
Y_ATOM_TICKS(fullTicks,atom::NAME(A,a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(B,b,loop));\
Y_ATOM_EQ(a,b,NAME);\
Y_ATOM_OUT(NAME,2);\
} while(false)
    
#define Y_ATOM_TEST3(NAME) do { \
fill(A); copyTo(B,A);\
fill(a); copyTo(b,a);\
Y_ATOM_TICKS(fullTicks,atom::NAME(U,A,a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(V,B,b,loop));\
Y_ATOM_EQ(U,V,NAME);\
Y_ATOM_OUT(NAME,3);\
} while(false)
    
#define Y_ATOM_MULOP(NAME) do {\
const type value = support::get<type>();\
fill(a); copyTo(b,a);\
fill(A); copyTo(B,A);\
Y_ATOM_TICKS(fullTicks,atom::NAME(A,value,a));\
Y_ATOM_TICKS(loopTicks,atom::NAME(B,value,b,loop));\
Y_ATOM_EQ(A,B,NAME);\
Y_ATOM_OUT(NAME,1);\
} while(false)
    
    template <typename ARR, typename BRR>
    void Test1( ARR &a, BRR &b, concurrent::for_each &loop )
    {
        typedef typename ARR::mutable_type type;
        vector<type> A(a.size()), U(a.size());
        vector<type> B(b.size()), V(b.size());
        
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
        Y_ATOM_TEST3(add);
        
        Y_ATOM_TEST2(sub);
        Y_ATOM_TEST3(sub);
        Y_ATOM_TEST2(subp);
        
        {
            const type value = support::get<type>();
            fill(a); copyTo(b,a);
            Y_ATOM_TICKS(fullTicks,atom::mul_by(value,a));
            Y_ATOM_TICKS(loopTicks,atom::mul_by(value,b,loop));
            Y_ATOM_EQ(a,b,mul_by);
            Y_ATOM_OUT(mul_by,1);
        }
        
        
        {
            Y_ATOM_MULOP(muladd);
            Y_ATOM_MULOP(mulset);
            Y_ATOM_MULOP(mulsub);
        }
        
        {
            const type value = support::get<type>();
            fill(a); copyTo(b,a);
            fill(A); copyTo(B,A);
            Y_ATOM_TICKS(fullTicks,atom::setprobe(U,A,value,a));
            Y_ATOM_TICKS(loopTicks,atom::setprobe(V,B,value,b,loop));
            Y_ATOM_EQ(U,V,setprobe);
            Y_ATOM_OUT(setprobe,1);
        }
        
        
        {
            fill(a); copyTo(A,a);
            fill(b); copyTo(B,b);
            type ab=0, AB=0;
            Y_ATOM_TICKS(fullTicks,ab=atom::dot(a,b));
            Y_ATOM_TICKS(loopTicks,AB=atom::dot(A,B,loop));
            type dd = ab - AB;
            dd *= dd;
            std::cerr << "\tdelta.dot=" << dd << std::endl;
            Y_ATOM_OUT(dot,2);
        }
        
        {
            fill(a);
            copyTo(b,a);
            type a2=0, b2=0;
            Y_ATOM_TICKS(fullTicks,a2=atom::norm2(a));
            Y_ATOM_TICKS(loopTicks,b2=atom::norm2(b,loop));
            type dd = a2 - b2;
            dd *= dd;
            std::cerr << "\tdelta.norm2=" << dd << std::endl;
            Y_ATOM_OUT(norm2,1);
        }
        
        
    }
    
    
#define TEST1_PERM(A,B) Test1(A,B,loop); Test1(B,A,loop)
    
    template <typename T>
    static inline void doTest1(concurrent::for_each &loop)
    {
        
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
    
    template <
    typename LHS,
    typename MATRIX,
    typename RHS>
    void doTestMUL(LHS    &lhs,
                   MATRIX &M,
                   RHS    &rhs,
                   concurrent::for_each &loop)
    {
        typedef typename LHS::mutable_type type;
        vector<type> tmp( lhs.size() );
        
        {
            fillTableau(M);
            fill(lhs);
            fill(rhs);
            Y_ATOM_TICKS(fullTicks,atom::mul(lhs,M,rhs));
            copyTo(tmp,lhs);
            Y_ATOM_TICKS(loopTicks,atom::mul(lhs,M,rhs,loop));
            Y_ATOM_EQ(tmp,lhs,mul);
            Y_ATOM_OUT(mul,3);
        }
        
        
    }
    
    template <typename T>
    static inline void doTest2(concurrent::for_each &loop)
    {
        std::cerr << "Test2" << std::endl;
        typedef matrix<T>         Matrix;
        typedef Oxide::Field2D<T> Field;
        
        typedef point2d<T>        P2D;
        typedef point3d<T>        P3D;
        
        //----------------------------------------------------------------------
        //
        // MUL
        //
        //----------------------------------------------------------------------

#define DO_TEST2_MUL() do { doTestMUL(lhs,M,rhs,loop); doTestMUL(lhs,F,rhs,loop); } while(false)

#define DO_TEST2_MUL_LHS(N) do { \
{ vector<T,memory::global> lhs(N); DO_TEST2_MUL(); }\
{ vector<T,memory::pooled> lhs(N); DO_TEST2_MUL(); }\
{ list<T>                  lhs(N); DO_TEST2_MUL(); }\
} while(false)
        
        // 2D
        {
            const size_t n= 2;
            P2D   rhs;
            {
                Matrix M(1,n);
                Field  F("F",1,n);
                DO_TEST2_MUL_LHS(1);
            }
            
            {
                
                Matrix M(n,n);
                Field  F("F",n,n);
                { P2D    lhs; DO_TEST2_MUL(); }
                DO_TEST2_MUL_LHS(2);
            }
        }
        
        // 3D
        {
            const size_t n=3;
            P3D   rhs;
            {
                Matrix M(1,n);
                Field  F("F",1,n);
                DO_TEST2_MUL_LHS(1);
            }
            
            {
                Matrix M(2,n);
                Field  F("F",2,n);
                { P2D    lhs; DO_TEST2_MUL(); }
                DO_TEST2_MUL_LHS(2);
            }
            
            {
                Matrix M(n,n);
                Field  F("F",n,n);
                { P3D    lhs; DO_TEST2_MUL(); }
                DO_TEST2_MUL_LHS(n);
            }
            
            // GENERIC
            for(size_t nr=1;nr<=30;++nr)
            {
                for(size_t nc=1;nc<=30;++nc)
                {
                    Matrix M(nr,nc);
                    Field  F("F",nr,nc);
                    { vector<T,memory::global> rhs(nc);DO_TEST2_MUL_LHS(nr); }
                    { vector<T,memory::pooled> rhs(nc);DO_TEST2_MUL_LHS(nr); }
                    { list<T>                  rhs(nc);DO_TEST2_MUL_LHS(nr); }
                }
            }
            
        }
        
    }
    
}

#include "y/string/convert.hpp"

Y_UTEST(atom)
{
    size_t level = 0;
    if(argc>1)
    {
        level = string_convert::to<size_t>(argv[1],"level");
    }
    
    concurrent::simd loop;
    std::cerr << "loop.size=" << loop.engine().num_threads() << std::endl;
    
    if(level<=0||1==level)
    {
        doTest1<float>(loop);
        doTest1<short>(loop);
        doTest1<double>(loop);
        doTest1<mpz>(loop);
    }
    
    
    if(level<=0||2==level)
    {
        doTest2<float>(loop);
    }
    
}
Y_UTEST_DONE()


