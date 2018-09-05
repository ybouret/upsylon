#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/math/kernel/tao.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(cs)
{

    Library   lib;
    Species  &proton  = lib("H+",  1);
    Species  &hydroxy = lib("HO-",-1);
    Species  &AH      = lib("AH",0);
    Species  &Am      = lib("Am",-1);
    lib("Na+",1);
    lib("Cl-",-1);

    Equilibria cs;

    {
        Equilibrium &water = cs("water",1e-14);
        water.add(proton);
        water.add(hydroxy);
    }

    {
        Equilibrium &acetic = cs("acetic",pow(10.0,-4.8));
        acetic.add(proton,2);
        acetic.add(Am,2);
        acetic.add(AH,-2);
    }

    std::cerr << "Raw Equilibria:" << std::endl;
    std::cerr << cs << std::endl;
    cs.compile_for(lib);
    std::cerr << "Compiled Equilibria:" << std::endl;
    std::cerr << cs << std::endl;


    vector<double> C(cs.M+2);

    for(size_t i=C.size();i>0;--i)
    {
        C[i] = alea.to<double>();
    }

    lib.display(std::cerr,C);
    std::cerr << "Nu =" << cs.Nu  << std::endl;
    std::cerr << "tNu=" << cs.tNu << std::endl;
    std::cerr << "Bal=" << cs.Bal << std::endl;

    std::cerr << "active=" << cs.active << std::endl;
    cs.computeK(0.0);
    cs.computeGamma(C);
    std::cerr << "K="     << cs.K     << std::endl;
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    cs.computeGammaAndPhi(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Phi  =" << cs.Phi   << std::endl;

    cs.computeW();
    std::cerr << "W=" << cs.W << std::endl;

    
    for(size_t i=1;i<=cs.N;++i)
    {
        std::cerr << "SolveSingle '" << cs.rxn[i]->name << "'" << std::endl;
        if(cs.solveSingle(i,C))
        {
            lib.display(std::cerr,C,"  (@) ");
        }
    }

    
    C.ld(0);
    for(size_t i=1;i<=cs.N;++i)
    {
        std::cerr << "SolveSingle0 '" << cs.rxn[i]->name << "'" << std::endl;
        if(cs.solveSingle(i,C))
        {
            lib.display(std::cerr,C,"  (@) ");
        }
    }


    for(size_t iter=0;iter<1000;++iter)
    {
        for(size_t i=C.size();i>0;--i)
        {
            C[i] = alea.to<double>();
        }
        cs.computeGammaAndPhi(C);
        for(size_t i=1;i<=cs.N;++i)
        {
            const array<double> &grad = cs.Phi[i];
            const array<int>    &nu   = cs.Nu[i];
            const double delta = math::tao::_dot<double>(grad,nu);
            Y_ASSERT(delta<=0);
        }
        bool success = true;
        for(size_t i=1;i<=cs.N;++i)
        {
            if(!cs.solveSingle(i,C))
            {
                success = false;
            }
        }
        if(success) std::cerr << '+'; else std::cerr << '-';
    }
    std::cerr << std::endl;

    std::cerr << "From 0..." << std::endl;
    C.ld(0);
    cs.computeGammaAndPhi(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Phi  =" << cs.Phi   << std::endl;

    cs.computeW();
    std::cerr << "W=" << cs.W << std::endl;

}
Y_UTEST_DONE()


