#include "y/information/translator/bytewise.hpp"
#include "y/information/modulation/echo.hpp"
#include "y/information/modulation/delta.hpp"
#include "y/information/modulation/mtf.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace Information;

Y_UTEST(bytewise)
{

    Y_UTEST_SIZEOF(counted);
    Y_UTEST_SIZEOF(object);
    Y_UTEST_SIZEOF(counted_object);
    Y_UTEST_SIZEOF(Modulation);
    Y_UTEST_SIZEOF(EchoModulation);
    Y_UTEST_SIZEOF(DeltaEncoder);
    Y_UTEST_SIZEOF(DeltaDecoder);

    const Modulation::Pointer p = new MoveToFrontEncoder();

    BytewiseTranslator mtf( p );
    BytewiseTranslator unmtf( new MoveToFrontDecoder() );
    BytewiseTranslator ec1( new EchoModulation() );
    BytewiseTranslator ec2( NULL );
    BytewiseTranslator del(   new DeltaEncoder() );
    BytewiseTranslator undel( new DeltaDecoder() );

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

