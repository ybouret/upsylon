
#include "y/chem/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lib)
{
    Library lib;
    Species &proton  = lib("H+",1);
    Species &hydroxy = lib("HO-",-1);
    (void)proton;
    (void)hydroxy;
    lib("AH",0);
    lib("A-",-1);

    vector<double> C(lib.size()+2);
    for(size_t i=1;i<=C.size();++i) C[i] = alea.to<double>();
    lib.display(std::cerr,C,"\t");

}
Y_UTEST_DONE()

