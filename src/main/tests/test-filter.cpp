
#include "y/information/filter/byte.hpp"

#include "y/information/shift/echo.hpp"
#include "y/information/shift/delta.hpp"
#include "y/information/shift/mtf.hpp"

#include "y/information/entropy.hpp"


#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {

    static inline void process( const string &src, information::filter &F )
    {
        information::entropy S;
        F.reset();
        const string out = F.to_string(src);
        out.display_visible(std::cerr << '[') << ']' << " $=" << S.of(out) << std::endl;

    }

}

Y_UTEST(filter)
{
    information::bytewise_filter echoF( new information::echo_modulation() );
    information::bytewise_filter deltaF( new information::delta_encoder()  );
    information::bytewise_filter mtfF( new information::mtf_encoder()  );

    for(int i=1;i<argc;++i)
    {
        const string src = argv[i];
        process( src, echoF  );
        process( src, deltaF );
        process( src, mtfF   );
    }

}
Y_UTEST_DONE()
