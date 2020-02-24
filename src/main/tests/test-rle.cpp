#include "y/information/translator/rle/encoder.hpp"
#include "y/information/translator/rle/decoder.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/string/convert.hpp"
#include "y/fs/disk/file.hpp"
#include "y/hashing/md5.hpp"

using namespace upsylon;

namespace {


}

Y_UTEST(rle)
{

    if(argc>1)
    {
        const string              fileName = argv[1];
        const string              compName = "rle.bin";
        const string              backName = "unrle.bin";
        information::RLE::Encoder rle;

        rle.testCODEC(fileName, compName, backName, NULL);
    }

}
Y_UTEST_DONE()


