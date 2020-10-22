
#include "y/aqua/boot.hpp"
#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{

    namespace Aqua
    {

        using namespace mkl;

        void  Boot:: find(addressable<double> &C,
                          Engine              &engine)
        {
            assert(C.size()>=engine.M);
            // build Cstar
            const size_t   M  = engine.M;
            const size_t   Nc = size;
            vector<double> Cold(M,0);
            vector<double> Cnew(M,0);
            vector<double> Cprj(M,0);
            vector<double> Lambda(Nc,0);
            vector<double> RC(Nc,0);

            fill(Lambda);
            tao::mul(Cold,pL,Lambda);
            tao::divall(Cold,dL);
            std::cerr << "Lambda=" << Lambda << std::endl;
            std::cerr << "Cstar =" << Cold  << std::endl;
            std::cerr << "R     =" << R << std::endl;

            // initial balance
            if( !engine.balance_(Cold, pS, dS) )
            {
                throw exception("no possible initial balance");
            }
            // initial equilibrium
            std::cerr << "Cbal= " << Cold << std::endl;
            tao::set(Cnew,Cold);
            if(!engine.forward(Cnew))
            {
                throw exception("no possible initial forward");
            }

            std::cerr << "Cfwd= " << Cnew << std::endl;

            tao::mul(RC, R, Cnew);
            tao::subp(RC,Lambda);
            tao::mul(Cprj,pL,RC);
            tao::divall(Cprj,dL);
            tao::add(Cnew,Cprj);
            std::cerr << "dC  = " << Cprj << std::endl;
            std::cerr << "Cnew= " << Cnew << std::endl;
            if( !engine.balance_(Cnew, pS, dS) )
            {
                throw exception("no possible   balance");
            }
            std::cerr << "C   = " << Cnew << std::endl;
        }

    }

}

