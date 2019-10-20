#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(minfit)
{

    {
        const size_t nr = 1+alea.leq(10);
        const size_t nc = 1+alea.lt(nr);

        matrix<double> A(nr,nc);
        matrix<double> W(nc,nc);
        vector<double> y(nr);
        vector<double> x(nc);
        vector<double> R(nr);

        for(size_t i=1;i<=nr;++i)
        {
            for(size_t j=1;j<=nc;++j)
            {
                A[i][j] = 5.0 * alea.symm<double>();
            }
            y[i] = 5.0 * alea.symm<double>();
        }

        for(size_t j=1;j<=nc;++j)
        {
            W[j][j] = 0.1 + alea.to<double>();
        }

        std::cerr << "A=" << A << std::endl;
        std::cerr << "W=" << W << std::endl;
        std::cerr << "y=" << y << std::endl;
        matrix<double> AA(nc,nc);
        matrix<double> WW(nc,nc);
        atom::mmul_ltrn(AA, A, A);
        atom::mmul_ltrn(WW, W, W);



        vector<double> tAy(nc);
        atom::mul_trn(tAy,A,y);

        std::cerr << "AA="  << AA  << std::endl;
        std::cerr << "WW="  << WW  << std::endl;
        std::cerr << "tAy=" << tAy << std::endl;

        matrix<double> HH(nc,nc);
        vector<double> WWx(nc);

        {
            ios::ocstream fp("minfit.dat");

            for(double mu2=0.0;mu2<10;mu2+=0.1)
            {
                for(size_t i=1;i<=nc;++i)
                {
                    for(size_t j=1;j<=nc;++j)
                    {
                        HH[i][j] = AA[i][j] + mu2 * WW[i][j];
                    }
                }

                if( !LU::build(HH) )
                {
                    throw exception("Singular Matrix");
                }

                atom::set(x,tAy);
                LU::solve(HH,x);
                std::cerr << "x=" << x << std::endl;
                atom::mul(WWx,WW,x);
                const double Wx2 = atom::dot(x,WWx);
                std::cerr << "Wx2=" << Wx2 << std::endl;

                atom::mul(R,A,x);
                atom::sub(R,y);
                const double R2 = atom::norm2(R);
                std::cerr << "R2=" << R2 << std::endl;

                fp("%g %g %g\n", mu2, Wx2, R2);

            }
        }








    }


}
Y_UTEST_DONE()

