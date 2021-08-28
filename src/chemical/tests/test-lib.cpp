

#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"
#include "y/ios/osstream.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lib)
{
    Library lib;
    
    lib("H+");
    lib("HO-");
    
    std::cerr << lib << std::endl;
    
    string out;
    {
        ios::osstream fp(out);
        static_cast<ios::ostream &>(fp) << lib << '\n';
    }
    std::cerr << "'" << out << "'" << std::endl;
    
    
}
Y_UTEST_DONE()

