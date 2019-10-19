
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

    template <typename TARGET, typename LHS, typename RHS>
    static inline void doMMUL(TARGET &M, LHS &lhs, RHS &rhs, concurrent::for_each &loop )
    {
        Y_ASSERT(lhs.rows==M.rows);
        Y_ASSERT(rhs.cols==M.cols);
        Y_ASSERT(lhs.cols==rhs.rows);
        Y_SUPPORT_TICKS(fullTicks,atom::mmul(M,lhs,rhs));

        Y_SUPPORT_TICKS(loopTicks,atom::mmul(M,lhs,rhs,loop));


    }

    template <typename T> static inline
    void doTest(concurrent::for_each &loop)
    {
        static const size_t nn[] = { 1,2,4,8,16,256 };


        typedef matrix<T>                Matrix;
        typedef Oxide::Field2D<T>        Field;

        for(size_t i=0;i<sizeof(nn)/sizeof(nn[0]);++i)
        {
            const size_t nr = nn[i];

            for(size_t j=0;j<sizeof(nn)/sizeof(nn[0]);++j)
            {
                const size_t nc = nn[j];
                std::cerr << '[' <<  nr << 'x' << nc << ']';



                {
                    Matrix M(nr,nc);
                    Field  F("F",nr,nc);

                    for(size_t k=1;k<=1024;k<<=1)
                    {
                        std::cerr << '.';
                        Matrix LM(nr,k);
                        Matrix RM(k,nc);
                        Field  LF("LF",nr,k,Oxide::AsMatrix);
                        Field  RF("RF",k,nc,Oxide::AsMatrix);

                        doMMUL(M, LM, RM, loop);
                        doMMUL(F, LF, RF, loop);

                    }
                }


                std::cerr << std::endl;

            }
        }

    }

}

Y_UTEST(atom3)
{
    concurrent::simd loop;

    doTest<float>(loop);

}
Y_UTEST_DONE()

