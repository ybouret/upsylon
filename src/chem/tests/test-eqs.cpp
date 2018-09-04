#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

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
        acetic.add(proton);
        acetic.add(Am);
        acetic.sub(AH);
    }

    cs.compile_for(lib);
    std::cerr << cs << std::endl;
    std::cerr << "Nu=" << cs.Nu << std::endl;

    vector<double> C(cs.M+2);
    for(size_t i=C.size();i>0;--i)
    {
        C[i] = alea.to<double>();
    }

    lib.display(std::cerr,C);

    cs.computeK(0.0);
    cs.computeGamma(C);
    std::cerr << "K="     << cs.K     << std::endl;
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    cs.computeGammaAndPhi(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "Phi  =" << cs.Phi   << std::endl;

}
Y_UTEST_DONE()


