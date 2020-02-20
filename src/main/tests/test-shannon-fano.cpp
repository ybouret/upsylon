#include "y/information/translator/shannon-fano/encoder.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(shannon_fano)
{
    ShannonFano::Encoder enc;
    if(argc>1)
    {
        const string  fileName = argv[1];
        const string  compName = "sf.bin";
        size_t nr = 0;
        size_t nc = 0;
        {
            ios::icstream source( fileName );
            ios::ocstream target( compName );
            enc.reset();
            nc = enc.process(target, source, &nr );
        }
        std::cerr << "sf : " << nr << " -> " << nc << std::endl;
    }
    enc.displayChars();
    //enc.getRoot().graphViz("sf.dot");
}
Y_UTEST_DONE()

