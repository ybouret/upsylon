
#include "y/math/kernel/atom.hpp"
#include "y/container/matrix.hpp"
#include "y/oxide/field2d.hpp"
#include "y/memory/pooled.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {


    template <typename LHS,
    typename MATRIX,
    typename RHS>
    static inline void testNrm(LHS    &lhs,
                               MATRIX &M,
                               RHS    &rhs,
                               concurrent::for_each &loop,
                               const bool            exact)
    {
        rt_clock clk;
        typedef typename LHS::mutable_type type;

        support::fill1D(rhs);
        support::fill2D(M);


        const size_t nr = M.rows;
        const size_t nc = M.cols;

        vector<type> tmp( nr );
        {
            Y_SUPPORT_TICKS(fullTicks,atom::mul(lhs,M,rhs));
            atom::tool::copy1D(tmp,lhs);
            Y_SUPPORT_TICKS(loopTicks,atom::mul(lhs,M,rhs,loop));
            const type d2 = atom::tool::deltaSquared1D(lhs,tmp);
            std::cerr << '<' << d2 << "@*" << '>' << clk.speedup(fullTicks,loopTicks) << '/';
            if(exact) Y_ASSERT(d2<=0);
        }

        {
            vector<type> org(nr);
            support::fill1D(org);

            {
                atom::tool::copy1D(lhs,org);
                Y_SUPPORT_TICKS(fullTicks,atom::mul_add(lhs,M,rhs));
                atom::tool::copy1D(tmp,lhs);

                atom::tool::copy1D(lhs,org);
                Y_SUPPORT_TICKS(loopTicks,atom::mul_add(lhs,M,rhs,loop));
                const type d2 = atom::tool::deltaSquared1D(lhs,tmp);
                std::cerr << '<' << d2 << "@+" << '>' << clk.speedup(fullTicks,loopTicks) << '/';
                if(exact) Y_ASSERT(d2<=0);
            }

            support::fill1D(org);

            {
                atom::tool::copy1D(lhs,org);
                Y_SUPPORT_TICKS(fullTicks,atom::mul_sub(lhs,M,rhs));
                atom::tool::copy1D(tmp,lhs);

                atom::tool::copy1D(lhs,org);
                Y_SUPPORT_TICKS(loopTicks,atom::mul_sub(lhs,M,rhs,loop));
                const type d2 = atom::tool::deltaSquared1D(lhs,tmp);
                std::cerr << '<' << d2 << "@-" << '>' << clk.speedup(fullTicks,loopTicks) << '/';
                if(exact) Y_ASSERT(d2<=0);
            }

        }

        {
            for(size_t i=nr;i>0;--i)
            {
                for(size_t j=nc;j>0;--j)
                {
                    M[i][j] = 0;
                }
            }
            support::fill1D(rhs);
            atom::mul(lhs,M,rhs);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';
            atom::mul(lhs,M,rhs,loop);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';

            support::fill1D(rhs);
            atom::mul_add(lhs,M,rhs);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';
            atom::mul_add(lhs,M,rhs,loop);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';

            support::fill1D(rhs);
            atom::mul_sub(lhs,M,rhs);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';
            atom::mul_sub(lhs,M,rhs,loop);
            Y_ASSERT( atom::norm2(lhs) <= 0 ); std::cerr << '.';

            if(nr==nc)
            {

                for(size_t i=nr;i>0;--i)
                {
                    M[i][i] = 1;
                }

                support::fill1D(rhs);
                atom::mul(lhs,M,rhs);
                Y_ASSERT( atom::tool::deltaSquared1D(lhs,rhs) <= 0 ); std::cerr << '.';
                atom::mul(lhs,M,rhs,loop);
                Y_ASSERT( atom::tool::deltaSquared1D(lhs,rhs) <= 0 ); std::cerr << '.';


            }
        }


    }

    template <typename T>
    void doTest2( concurrent::simd &loop, const bool exact)
    {

        const char         *name = typeid(T).name();
        std::cerr << "[[ Enter Atom2<" << name << "> ]]" << std::endl;

        static const size_t nn[] = { 1,2,3,4,8,16,256 };

        typedef matrix<T>                Matrix;
        typedef Oxide::Field2D<T>        Field;
        typedef vector<T,memory::global> GVector;
        typedef vector<T,memory::pooled> PVector;
        typedef list<T>                  List;


        for(size_t i=0;i<sizeof(nn)/sizeof(nn[0]);++i)
        {
            const size_t nr = nn[i];

            GVector    gvr(nr);
            PVector    pvr(nr);
            List       glr(nr);
            point2d<T> p2r;
            point3d<T> p3r;

            for(size_t j=0;j<sizeof(nn)/sizeof(nn[0]);++j)
            {
                const size_t nc = nn[j];
                std::cerr << '\t' << '[' <<  nr << 'x' << nc << ']' << ':';
                Matrix M(nr,nc);
                Field  F("F",nr,nc,Oxide::AsMatrix);

                GVector    gvc(nc);
                PVector    pvc(nc);
                List       glc(nc);
                point2d<T> p2c;
                point3d<T> p3c;

#define TEST2_NRM_(MM,RHS) do {               \
testNrm(gvr, MM, RHS, loop, exact);           \
testNrm(gvr, MM, RHS, loop, exact);           \
testNrm(gvr, MM, RHS, loop, exact);           \
if(2==nr) testNrm(p2r, MM, RHS, loop, exact); \
if(3==nr) testNrm(p3r, MM, RHS, loop, exact); \
} while(false)

#define TEST2_NRM(RHS) do { TEST2_NRM_(M,RHS); TEST2_NRM_(F,RHS); } while(false)

                TEST2_NRM(gvc);
                TEST2_NRM(pvc);
                TEST2_NRM(glc);
                if(2==nc) TEST2_NRM(p2c);
                if(3==nc) TEST2_NRM(p3c);
                std::cerr << std::endl;
            }

        }

        std::cerr << "[[ Leave Atom2<" << name << "> ]]" << std::endl;

    }

}


Y_UTEST(atom2)
{
    concurrent::simd loop;

    //doTest2<float>(loop,false);
    doTest2<double>(loop,false);
    doTest2<short>(loop,true);
    //doTest2<mpz>(loop);
    // doTest2<mpq>(loop);

}
Y_UTEST_DONE()

