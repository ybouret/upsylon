#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include <cmath>

using namespace upsylon;

namespace  {

    class Fluo
    {
    public:
        const double K;
        const double a;
        const double Ia;
        const double b;
        const double Ib;
        const double Kpa;
        const double Kpb;
        const double Phi_alpha;
        const double Phi_beta;

        inline Fluo(const double pK,
                    const double pHa, const double _Ia,
                    const double pHb, const double _Ib) :
        K( pow(10.0,-pK)  ),
        a( pow(10.0,-pHa) ),
        Ia(_Ia),
        b( pow(10.0,-pHb) ),
        Ib(_Ib),
        Kpa(K+a),
        Kpb(K+b),
        Phi_alpha( (Ia*Kpa-Ib*Kpb)/(a-b) ),
        Phi_beta( (a*Ib*Kpb-b*Ia*Kpa)/(a-b)/K )
        {

        }

        ~Fluo() throw()
        {
        }

        double pH(const double I)
        {
            const double h = K * (Phi_beta-I)/(I-Phi_alpha);
            return -log10(h);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Fluo);
    };

}

Y_UTEST(fluo)
{
    //const double pK  = 4+4*alea.to<double>();
    const double pK = 6.98;
    //const double pHa = 2+2*alea.to<double>();
    const double pHa = 5;
    //const double pHb = 8+2*alea.to<double>();
    const double pHb = 8;
    const double Ia  = alea.to<double>();
    const double Ib  = 5+5*alea.to<double>();
    Fluo         fluo(pK,pHa,Ia,pHb,Ib);

    const double dI  = Ib-Ia;
    const double Ilo = Ia - 0.05 * dI;
    const double Iup = Ib + 0.05 * dI;

    {
        const size_t M = 100;
        ios::ocstream fp("fluo.dat");
        for(size_t i=0;i<=M;++i)
        {
            const double I = Ilo + ((Iup-Ilo)*i)/M;
            fp("%g %g %g %g %g\n",I,fluo.pH(I),pHa,pHb,pK);
        }
    }
}
Y_UTEST_DONE()

