
#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Alchemy;



Y_UTEST(eq)
{
#if 0
    library        lib;
    const species &h  = lib("H+",1);
    const species &w  = lib("HO-",-1);
    const species &ah = lib("AH",0);
    const species &am = lib("Am",-1);
    
    equilibrium::pointer water = new constant_equilibrium("water",1e-14);
    
    (*water)(1,h);
    (*water)(1,w);
    
    equilibrium::pointer toto = new constant_equilibrium("toto",1e-1);
    (*toto)(2,h);
    (*toto)(-1,w);
    

    equilibrium::pointer weak = new constant_equilibrium("weak",1e-4);
    (*weak)(1,h);
    (*weak)(1,am);
    (*weak)(-1,ah);
    
    std::cerr << water << std::endl;
    std::cerr << toto  << std::endl;
    std::cerr << weak  << std::endl;

    
    lib.compile();
    std::cerr << water << std::endl;
    std::cerr << toto  << std::endl;
    std::cerr << weak  << std::endl;
    
    water->compile();
    toto->compile();
    weak->compile();
    std::cerr << water << std::endl;
    std::cerr << toto  << std::endl;
    std::cerr << weak  << std::endl;
    
    
    water->display_code(std::cerr) << std::endl;
    toto->display_code(std::cerr)  << std::endl;
    weak->display_code(std::cerr)  << std::endl;
    
    
    vector<long> Nu(lib->size(),0);
    water->fill(Nu); std::cerr << "Nu_" << water->name << " : " << Nu << std::endl;
    weak->fill(Nu); std::cerr << "Nu_" << weak->name << " : " << Nu << std::endl;
#endif
    
}
Y_UTEST_DONE()

