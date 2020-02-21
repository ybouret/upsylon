#include "y/information/translator/bytewise.hpp"
#include "y/information/modulation/echo.hpp"
#include "y/information/modulation/delta.hpp"
#include "y/information/modulation/mtf.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

Y_UTEST(bytewise)
{

    Y_UTEST_SIZEOF(counted);
    Y_UTEST_SIZEOF(object);
    Y_UTEST_SIZEOF(counted_object);
    Y_UTEST_SIZEOF(modulation);
    Y_UTEST_SIZEOF(echo_modulation);
    Y_UTEST_SIZEOF(delta_encoder);
    Y_UTEST_SIZEOF(delta_decoder);

    const modulation::pointer p = new mtf_encoder();

    BytewiseTranslator mtf( p );
    BytewiseTranslator unmtf( new mtf_decoder() );
    BytewiseTranslator ec1( new echo_modulation() );
    BytewiseTranslator ec2( NULL );
    BytewiseTranslator del( new delta_encoder() );
    BytewiseTranslator undel( new delta_decoder() );

    if(argc>1)
    {
        const string fileName = argv[1];
        {
            const string       compName = "mtf.bin";
            const string       backName = "unmtf.bin";
            mtf.testCODEC(fileName, compName, backName, &unmtf);
        }

        {
            const string       compName = "del.bin";
            const string       backName = "undel.bin";
            del.testCODEC(fileName, compName, backName, &undel);
        }

        {
            const string       compName = "echo.bin";
            const string       backName = "unecho.bin";
            ec1.testCODEC(fileName, compName, backName, &ec2);
        }

    }


}
Y_UTEST_DONE()

