
#include "y/math/kernel/atom.hpp"
#include "y/container/matrix.hpp"
#include "y/oxide/field2d.hpp"
#include "y/memory/pooled.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
#include <iomanip>

using namespace upsylon;
using namespace math;

namespace {

    template <typename TARGET, typename LHS, typename RHS>
    static inline void doMMUL(TARGET &M, LHS &lhs, RHS &rhs, concurrent::for_each &loop )
    {
        rt_clock clk;
        Y_ASSERT(lhs.rows==M.rows);
        Y_ASSERT(rhs.cols==M.cols);
        Y_ASSERT(lhs.cols==rhs.rows);
        support::fill2D(lhs);
        support::fill2D(rhs);
        
        matrix<typename TARGET::mutable_type> tmp(M.rows,M.cols);
        
        support::fill2D(M);
        Y_SUPPORT_TICKS(fullTicks,atom::mmul(M,lhs,rhs));
        atom::tool::copy2D(tmp,M);
        
        support::fill2D(M);
        Y_SUPPORT_TICKS(loopTicks,atom::mmul(M,lhs,rhs,loop));
        Y_ASSERT( atom::tool::deltaSquared2D(M,tmp) <= 0);
        std::cerr << clk.speedup(fullTicks,loopTicks) << '/';


    }


    template <typename T> static inline
    void doGram(concurrent::for_each &loop)
    {
        rt_clock clk;

        typedef matrix<T>                Matrix;
        std::cerr << "Testing Gram" << std::endl;

        for(size_t nr=1;nr<=16;nr<<=1)
        {
            Matrix Mseq(nr,nr);
            Matrix Mpar(nr,nr);
            std::cerr << '[' <<  std::setw(3) << nr << ']' << ':';
            for(size_t nc=1;nc<=256;nc<<=1)
            {
                Matrix   V(nr,nc);
                support::fill2D(V);
                support::fill2D(Mseq);
                support::fill2D(Mpar);

                Y_SUPPORT_TICKS(fullTicks,atom::Gram(Mseq,V));
                Y_SUPPORT_TICKS(loopTicks,atom::Gram(Mpar,V,loop));

                Y_ASSERT(atom::tool::deltaSquared2D(Mseq,Mpar)<=0);
                std::cerr << clk.speedup(fullTicks,loopTicks) << '/';

            } std::cerr << std::endl;
        }

    }

    template <typename T> static inline
    void doTest(concurrent::for_each &loop)
    {
        static const size_t nn[] = { 1,2,4,8,16,64 };

        std::cerr << "Testing MMUL" << std::endl;
        typedef matrix<T>                Matrix;
        typedef Oxide::Field2D<T>        Field;

        for(size_t i=0;i<sizeof(nn)/sizeof(nn[0]);++i)
        {
            const size_t nr = nn[i];

            for(size_t j=0;j<sizeof(nn)/sizeof(nn[0]);++j)
            {
                const size_t nc = nn[j];
                std::cerr << '[' <<  std::setw(3) << nr << 'x' <<  std::setw(3) << nc << ']';



                {
                    Matrix M(nr,nc);
                    Field  F("F",nr,nc,Oxide::AsMatrix);

                    for(size_t k=1;k<=1024;k<<=2)
                    {
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

        doGram<T>(loop);

    }



}


Y_UTEST(atom3)
{
    concurrent::simd loop;


    doTest<float>(loop);
    doTest<double>(loop);

}
Y_UTEST_DONE()

