
#include "y/chem/boot.hpp"
#include "y/chem/equilibria.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(boot)
{

    Library   lib;
    Species  &proton  = lib("H+",  1);
    Species  &hydroxy = lib("HO-",-1);
    Species  &AH      = lib("AH",0);
    Species  &Am      = lib("Am",-1);
    Species  &NH3     = lib("NH3",0);
    Species  &NH4     = lib("NH4+",1);
    Species  &Na      = lib("Na+",1);
    Species  &Cl      = lib("Cl-",-1);

    Equilibria cs;

    {
        Equilibrium &water = cs("water",1e-14);
        water.add(proton);
        water.add(hydroxy);
    }

    {
        Equilibrium &acetic = cs("acetic",square_of(pow(10.0,-4.8)));
        acetic.add(proton,2);
        acetic.add(Am,2);
        acetic.add(AH,-2);
    }

    {
        Equilibrium &ammoniac = cs("ammoniac",(pow(10.0,-9.2)));
        ammoniac.add(proton);
        ammoniac.add(NH3);
        ammoniac.sub(NH4);
    }

    cs.compile_for(lib);
    std::cerr << "<Equilibria>" << std::endl;
    std::cerr << cs << std::endl;
    std::cerr << "<Equilibria/>" << std::endl << std::endl;

    Boot::Loader loader;
    loader.electroneutrality(lib);
    loader.conserve(0,Na);
    loader.conserve(0.0001,Cl);
    loader.conserve(0.000001,Am,AH);
    loader.conserve(0.0001,NH4,NH3);

    vector<double> C( cs.M+2 );
    lib.display(std::cerr,C,"\t(0) " );
    std::cerr << loader << std::endl;

    cs.computeK(0.0);
    std::cerr << "Nu=" << cs.Nu << std::endl;
    if(cs.boot(C,loader))
    {
        lib.display(std::cerr,C);

        std::cerr << std::endl << "Damping..." << std::endl;
        vector<double> delta(cs.M);
        for(size_t iter=1;iter<=4;++iter)
        {
            for(size_t j=cs.M;j>0;--j)
            {
                delta[j] = alea.symm<double>();
            }
            std::cerr << "delta0=" << delta << std::endl;
            if(!cs.damp(delta,C))
            {
                throw exception("unexpected damp failure!!!");
            }
            std::cerr << "delta1=" << delta << std::endl;
        }
        std::cerr << "...done" << std::endl;
    }
    else
    {
        std::cerr << "couldn't boot" << std::endl;
    }



}
Y_UTEST_DONE()


Y_UTEST(bootAB)
{

    Library   lib;
    Species  &A  = lib("A", 0);
    Species  &B  = lib("B", 0);

    Equilibria cs;
    cs.compile_for(lib);
    vector<double> C(2);
    std::cerr << cs << std::endl;

    cs.computeK(0.0);
    if(true)
    {
        Boot::Loader loader;
        loader.conserve(0,A);
        loader.conserve(1,A,B);
        if(cs.boot(C,loader))
        {
            lib.display(std::cerr,C);
        }
        else std::cerr << "error loading" << std::endl;
    }

    std::cerr << "Adding AB" << std::endl;

    {
        Equilibrium &AB = cs("AB",1.2);
        AB.add(B);
        AB.sub(A);
    }
    cs.compile_for(lib);
    cs.computeK(0.0);
    std::cerr << cs << std::endl;

    {
        Boot::Loader loader;
        loader.conserve(1.0,A,B);
        if(cs.boot(C,loader))
        {
            lib.display(std::cerr,C);
        } else std::cerr << "error loading" << std::endl;

    }



}
Y_UTEST_DONE()

