#include "y/information/translator/shannon-fano/encoder.hpp"
#include "y/information/translator/shannon-fano/decoder.hpp"

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
    ShannonFano::Decoder dec;
    string           fileName = "fibonacci.bin";
    const string     compName = "shannon_fano.bin";
    const string     backName = "shannon_fano_org.bin";

    if( argc > 1 )
    {
        fileName = argv[1];
    }
    else
    {
        (void) Translator::Fibonacci(fileName, 'a', 'z' );
    }
    enc.testCODEC(fileName, compName, backName, &dec);
}
Y_UTEST_DONE()

