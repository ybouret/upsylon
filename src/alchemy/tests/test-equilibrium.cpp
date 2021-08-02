
#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/counting/comb.hpp"

using namespace upsylon;
using namespace Alchemy;


namespace
{
    static inline
    void drawC(addressable<double> &C)
    {
        for(size_t i=C.size();i>0;--i)
        {
            const double v = alea.to<double>();
            const double p = alea.range<double>(-14,0);
            C[i] = v * pow(10.0,p);
        }
    }


    static inline void trySolve(const Equilibrium   &eq,
                                addressable<double> &C)
    {
        const double K0 = eq.K(0);

        std::cerr << "  C0=" << C << "..";
        if(eq.solve(K0,C))
        {
            std::cerr << C << std::endl;
        }
        else
        {
            std::cerr << "blocked" << std::endl;
        }
    }

    static const size_t loop_max = 8;

    static inline
    void solveC(const Equilibrium   &eq,
                addressable<double> &C,
                const size_t         nz)
    {
        std::cerr << "Solving " << eq << std::endl;
        if(nz<=0)
        {
            for(size_t loop=0;loop<loop_max;++loop)
            {
                drawC(C);
                trySolve(eq,C);
            }
        }
        else
        {

            assert(nz<=C.size());
            combination    comb(C.size(),nz);
            for(comb.boot();comb.good();comb.next())
            {
                std::cerr << (counting &)comb << std::endl;
                for(size_t loop=0;loop<loop_max;++loop)
                {
                    drawC(C);
                    for(size_t i=comb.size();i>0;--i)
                    {
                        C[ comb[i] ] = 0;
                    }
                    trySolve(eq,C);
                }
            }
        }

    }

}



Y_UTEST(eq)
{
    Library       lib;
    const Species &h  = lib("H+",1);
    const Species &w  = lib("HO-",-1);
    const Species &ah = lib("AH",0);
    const Species &am = lib("Am",-1);

    std::cerr << "lib=" << lib << std::endl;

    Equilibrium::Pointer water = new ConstEquilibrium("water",1e-14);

    (*water)(1,h);
    (*water)(1,w);
    std::cerr << water << std::endl;

    Equilibrium::Pointer weak = new ConstEquilibrium("weak",1e-4);
    (*weak)(1,h);
    (*weak)(1,am);
    (*weak)(-1,ah);
    std::cerr << weak << std::endl;

    vector<long> Nu(lib->size(),0xff);
    water->fill(Nu); std::cerr << "Nu_" << water->name << " : " << Nu << std::endl;
    weak->fill(Nu);  std::cerr << "Nu_" << weak->name  << " : " << Nu << std::endl;

    vector<double> C(lib->size(),0);
    std::cerr << water->name << " : " << water->compute(water->K(0),C) << std::endl;
    std::cerr << weak->name  << " : " << weak->compute(weak->K(0),C)   << std::endl;

    const double xi = 1e-7;
    std::cerr << water->name << " +xi=" << xi << " : " << water->compute(water->K(0),C,xi) << std::endl;

    C[1] = 1e-5;
    C[2] = 1e-6;
    water->solve(water->K(0),C);


    std::cerr << std::endl;
    std::cerr << "Solving1..." << std::endl;
    for(size_t nz=0;nz<=C.size();++nz)
    {
        solveC(*water,C,nz);
    }
    std::cerr << std::endl;


    for(size_t nz=0;nz<=C.size();++nz)
    {
        solveC(*weak,C,nz);
    }
    std::cerr << std::endl;

    Equilibrium::Pointer rwater = new ConstEquilibrium("rwater",1e14);

    (*rwater)(-1,h);
    (*rwater)(-1,w);
    std::cerr << rwater << std::endl;
    for(size_t nz=0;nz<=C.size();++nz)
    {
        solveC(*rwater,C,nz);
    }


}
Y_UTEST_DONE()

