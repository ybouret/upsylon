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
        matrix<apq>  F(N-1,2);
        const size_t r = W.rows;
        for(size_t i=1;i<=r;++i)
        {
            if(i<=1)
            {
                W[1][1] = h;
                F[1][1] = h;
            }
            else
            {
                if(i>=r)
                {
                    W[r][r-1]= h;
                    F[r][2]  = h;
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
        std::cerr << "F =" << F  << std::endl;

        matrix<apq> IW(N-2,N-2);
        {
            matrix<apq> W2(N-2,N-2);
            tao::mmul(W2, Wt, W);
            if(!LU::build(W2))
            {
                throw exception("Singular W'*W");
            }
            LU::inverse(W2,IW);
        }
        std::cerr << "IW=" << IW << std::endl;

#if 0
        matrix<apq> WtF(Wt.rows,F.cols);
        tao::mmul(WtF,Wt,F);
        std::cerr << "WtF=" << WtF << std::endl;
#endif
        matrix<apq> P(N-2,N);
        matrix<apq> Q(2,N);
        for(size_t i=1;i<=N-2;++i)
        {
            P[i][i+1] = 1;
        }
        std::cerr << "P=" << P << std::endl;


        Q[1][1] = 1;
        Q[2][N] = 1;
        std::cerr << "Q=" << Q << std::endl;



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

