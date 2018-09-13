
#include "y/lang/syntax/node.hpp"

#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

#define SHOW(CLASS) std::cerr << "sizeof(" #CLASS << ")=" << sizeof(CLASS) << std::endl

Y_UTEST(grammar)
{
    SHOW(Syntax::Node);
    SHOW(Syntax::Node::List);
    
}
Y_UTEST_DONE()

