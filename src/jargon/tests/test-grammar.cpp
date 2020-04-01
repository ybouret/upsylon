#include "y/jargon/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace  upsylon;
using namespace  Jargon;

Y_UTEST(grammar)
{
 
    Y_UTEST_SIZEOF(Axiom);
    Y_UTEST_SIZEOF(Axioms);
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(Inactive);
    Y_UTEST_SIZEOF(Terminal);
    Y_UTEST_SIZEOF(Internal);
    std::cerr << std::endl;

    Y_UTEST_SIZEOF(Member);
    Y_UTEST_SIZEOF(Manifest);
    Y_UTEST_SIZEOF(Compound);
    std::cerr << std::endl;

    Grammar G("G");
    std::cerr << "grammar " << G.title  << std::endl;
    
    Terminal &term = G.declare( new Terminal("word") );
    Compound &cmpd = G.declare( new Compound("hello",Y_FOURCC('T','R','Y','!')));
    
    cmpd << term;
    
    G.displayAxioms();
    std::cerr << "ground=<" << G.getGround().label << ">" << std::endl;
}
Y_UTEST_DONE()


