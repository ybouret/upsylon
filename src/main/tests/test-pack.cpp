#include "y/information/pack.hpp"
#include "y/utest/run.hpp"
#include "y/information/mtf.hpp"
#include "y/information/echo.hpp"
#include "y/information/bwt.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

Y_UTEST(pack)
{

    information::mtf_encoder     mtf;
    information::echo_modulation echo;

    if(argc>1)
    {
        const string   fileName = argv[1];
        string         source;
        {
            ios::icstream fp(fileName);
            char C = 0;
            while( fp.query(C) )
            {
                source << C;
            }
        }
        const size_t   length = source.length();
        string         level1( length, as_capacity, true );
        vector<size_t> indices(length,0);

        const size_t pidx = information::bwt::encode(*level1, *source, length, *indices, mtf);
        std::cerr << "pidx=" << pidx << std::endl;
        {
            ios::ocstream fp("pack.bin");
            fp << level1;
        }
    }


}
Y_UTEST_DONE()
