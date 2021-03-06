

#include "y/aqua/engine.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"
#include "y/ios/align.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Aqua;
using namespace mkl;

Y_UTEST(engine)
{
    size_t iterMax = 10;
    if(argc>1)
    {
        iterMax = string_convert::to<size_t>(argv[1],"iterMax");
    }

    std::cerr << "-- building library" << std::endl;
    Library lib;

    Species &h   = lib("H+",1);
    Species &w   = lib("HO-",-1);
    Species &ah  = lib("AH",0);
    Species &am  = lib("Am",-1);


    Species &Na = lib("Na+",1);  (void)Na;
    Species &Cl = lib("Cl-",-1); (void)Cl;

    Species &NH4 = lib("NH4+",1);
    Species &NH3 = lib("NH3",0);

    lib.init();
    std::cerr << lib << std::endl;

    std::cerr << "-- building equilibria" << std::endl;
    Equilibria eqs;
    {
        Equilibrium &water = eqs.constant("water",1e-14);
        water(h,1);
        water(w,1);
    }

    {
        Equilibrium &acid = eqs.constant("acid",pow(10.0,-4.8));
        acid(ah,-1);
        acid(am,1);
        acid(h,1);
    }

    {
        Equilibrium &ammoniac = eqs.constant("ammoniac",pow(10.0,-9.2));
        ammoniac(NH4,-1);
        ammoniac(NH3,1);
        ammoniac(h,1);
    }


    std::cerr << eqs << std::endl;

    std::cerr << "-- start engine" << std::endl;
    Engine cs;
    cs.init(lib,eqs);
    cs.computeK(0);
    cs.updateKs();


    std::cerr << "Nu      = " << cs.Nu      << std::endl;
    std::cerr << "Nu2     = " << cs.Nu2     << std::endl;
    std::cerr << "dNu     = " << cs.dNu     << std::endl;
    std::cerr << "pNu     = " << cs.pNu     << std::endl;
    std::cerr << "active  = " << cs.active  << std::endl;
    std::cerr << "Ma      = " << cs.Ma      << " #/ " << cs.M << std::endl;
    std::cerr << "Nc      = " << cs.Nc      << std::endl;
    std::cerr << "K       ="  << cs.K << std::endl;
    std::cerr << "Ks      ="  << cs.Ks << std::endl;

    vector<double> C(cs.M+2,0);

    for(size_t iter=0;iter<iterMax;++iter)
    {
    GENERATE:
        for(size_t j=C.size();j>0;--j)
        {
            if(alea.to<double>()>0.2)
            {
                C[j] = alea.symm<double>() * pow(10.0,-8*alea.to<double>());
            }
            else
            {
                C[j] = 0;
            }
        }

        //lib.show(std::cerr << "ini=",C);

        cs.balanceVerbose=false;
        if( cs.balance(C) )
        {
            //lib.show(std::cerr << "end=",C);
        }
        else
            goto GENERATE;



        if(cs.sweep(C))
        {
            //lib.show(std::cerr << "swp=",C);
        }
        else
        {
            std::cerr << "couldn't sweep!!!" << std::endl;
            goto GENERATE;
        }

        //cs.forwardVerbose=true;
        //cs.balanceVerbose=true;
        if( cs.forward(C) )
        {
            lib.show(std::cerr << "res=",C);
        }

    }

    cs.forwardVerbose=true;
    tao::ld(C,0);
    if(cs.forward(C))
    {
        lib.show(std::cerr << "nil=",C);
    }
    else
    {
        throw exception("Coudln't forward to zero");
    }
    cs.forwardVerbose=false;

    vector<double> dC(cs.M,0);
    dC[ ah.indx ] = 0.001;
    lib.show(std::cerr << "dC=",dC);

    for(size_t j=1;j<=10;++j)
    {
        if(!cs.feed(C,dC))
        {
            throw exception("coudln't feed!");
        }
        lib.show(std::cerr << "C=",C);
    }


}
Y_UTEST_DONE()

