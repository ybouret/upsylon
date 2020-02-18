#include "y/information/pack.hpp"
#include "y/information/filter/rle/encoder.hpp"
#include "y/utest/run.hpp"
#include "y/information/mtf.hpp"
#include "y/information/echo.hpp"
#include "y/information/bwt.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/fs/disk/file.hpp"

using namespace upsylon;

Y_UTEST(pack)
{

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


}
Y_UTEST_DONE()
