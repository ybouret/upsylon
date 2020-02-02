#include "y/codec/nested.hpp"
#include "y/codec/delta.hpp"
#include "y/codec/mtf.hpp"

#include "y/codec/asf.hpp"
#include "y/codec/base64.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/string/convert.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(nested)
{

    nested_encoder<ASF::Encoder,echo_encoding>  E_ASF0;
    nested_encoder<ASF::Encoder,delta_encoding> E_ASF1;
    nested_encoder<ASF::Encoder,move_to_front>  E_ASF2;

    ios::codec    *enc[] = { &E_ASF0, &E_ASF1, &E_ASF2 };
    
    const unsigned num = sizeof(enc)/sizeof(enc[0]);

    if( argc > 1 )
    {
        const string           fileName = argv[1];
        auto_ptr<ios::istream> input = 0;

        if( "STDIN" == fileName )
        {
            input = new ios::icstream( ios::cstdin );
        }
        else
        {
            input = new ios::icstream( fileName );
        }

        char C;
        while( input->query(C) )
        {
            for(unsigned i=0;i<num;++i)
            {
                enc[i]->write(C);
            }
        }

        for(unsigned i=0;i<num;++i)
        {
            enc[i]->flush();
            ios::ocstream fp( vformat("enc%u.bin", i) );
            while( enc[i]->query(C) ) fp << C;
        }



    }



}
Y_UTEST_DONE()

