#include "y/yap.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    void genFBC(const size_t N)
    {
        assert(N>2);
        const apq    h(1,2);
        matrix<apq>  W(N-1,N-2);
        const size_t r = W.rows;
        for(size_t i=1;i<=r;++i)
        {
            if(i<=1)
            {
                W[1][1] = h;
            }
            else
            {
                if(i>=r)
                {
                    W[r][r-1]= h;
                }
                else
                {
                    assert(i>1);
                    assert(i<r);
                    W[i][i]   = h;
                    W[i][i-1] = h;
                }
            }
        }

        matrix<apq> Wt(W,matrix_transpose);

        std::cerr << "W =" << W  << std::endl;

        matrix<apq> W2(N-2,N-2);
        matrix<apq> IW(N-2,N-2);
        tao::mmul(W2, Wt, W);
        std::cerr << "W2=" << W2 << std::endl;
        {
            matrix<apq> W2LU(W2);
            if(!LU::build(W2LU))
            {
                throw exception("Singular W'*W");
            }
            LU::inverse(W2LU,IW);
        }
        std::cerr << "IW=" << IW << std::endl;

        std::cerr << std::endl;

    }
}

Y_UTEST(fvm)
{
    genFBC(3);
    genFBC(4);
    genFBC(5);

}
Y_UTEST_DONE()

