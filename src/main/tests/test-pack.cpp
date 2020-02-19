#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(pack)
{
#if 0
    information::Pack::Encoder   packEnc(1024*512);

    if(argc>1)
    {
        const string   fileName = argv[1];
        ios::icstream  source( fileName );
        ios::ocstream  target( "pack.bin" );

        packEnc.reset();
        size_t       nr = 0;
        const size_t nc = packEnc.process(target, source, &nr);
        std::cerr << "packEnc: " << nr << " -> " << nc << std::endl;

    }
#endif

}
Y_UTEST_DONE()
