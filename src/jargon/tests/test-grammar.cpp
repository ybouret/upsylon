#include "y/jargon/grammar.hpp"
#include "y/utest/run.hpp"

using namespace  upsylon;
using namespace  Jargon;

Y_UTEST(grammar)
{
 
    Grammar G("G");
    std::cerr << "grammar " << G.title  << std::endl;
    
    G.enroll( new Terminal("word") );
    G.enroll( new Internal("hello",0));
    
    G.displayAxioms();
}
Y_UTEST_DONE()


