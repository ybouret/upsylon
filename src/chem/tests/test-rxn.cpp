
#include "y/chem/reaction.hpp"
#include "y/chem/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(rxn)
{
    Library lib;
    Species &proton  = lib("H+",1);
    Species &hydroxy = lib("HO-",-1);
    (void)proton;
    (void)hydroxy;
    lib("AH",0);
    lib("A-",-1);

    {
        Reaction::Component c1( proton, 1 );
        Reaction::Component c2( hydroxy, 1 );
        Reaction::Component c3( lib["AH"], -1 );
        Reaction::Component c4( lib["A-"], -4 );
    }

    ConstReaction water("water",1e-14);

    water.add( proton, 1);
    water.add( hydroxy, 1);
    water.validate();

    ConstReaction acid("acid",pow(10,-4.8));
    acid.add(lib["AH"],-1);
    acid.add(lib["A-"],1);
    acid.add(proton,1);

    acid.validate();

    std::cerr << water << std::endl;
    std::cerr << acid  << std::endl;



}
Y_UTEST_DONE()

