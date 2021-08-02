
#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Alchemy;



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

    water->solve(water->K(0),C);
    


}
Y_UTEST_DONE()

