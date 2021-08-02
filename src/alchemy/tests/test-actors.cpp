

#include "y/alchemy/actors.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(actors)
{
    Library        lib;
    const Species &h = lib("H+",1);
    const Species &w = lib("HO-",-1);
    
    Actors A;
    A(h,10);
    A(w,1);
    
    Actors B;
    B(w,3);
    B(h,12);
    
    std::cerr << "A=" << A << std::endl;
    std::cerr << "B=" << B << std::endl;

    vector<double> C(lib->size(),0.0);

    for(size_t i=lib->size();i>0;--i)
    {
        C[i] = alea.to<double>();
    }

    std::cerr << "C=" << C << std::endl;
    std::cerr << "A -> " << A.findExtent(C) << std::endl;
    std::cerr << "B -> " << B.findExtent(C) << std::endl;



}
Y_UTEST_DONE()

