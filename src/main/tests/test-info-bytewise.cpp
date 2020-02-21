#include "y/information/translator/bytewise.hpp"
#include "y/information/modulation/echo.hpp"
#include "y/information/modulation/delta.hpp"
#include "y/information/modulation/mtf.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

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

    BytewiseTranslator mtf1( p );
    BytewiseTranslator enc1( new echo_modulation() );
    BytewiseTranslator enc2( NULL );


}
Y_UTEST_DONE()

