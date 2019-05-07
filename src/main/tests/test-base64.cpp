#include "y/codec/base64.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/utils.hpp"

using namespace upsylon;



Y_UTEST(base64)
{
#if 0
    for(size_t i=0;i<=40;++i)
    {
        std::cerr << i << " bits ->" << Y_BASE64_BITS_FOR(i)  << std::endl;
    }

    for(size_t i=1;i<=16;++i)
    {
        std::cerr << "#bytes=" << i << "->" << Y_BASE64_BYTES_FOR(i) << std::endl;
    }
#endif
    if( (argc>1) && (0==strcmp(argv[1],"NULL")) )
    {

        // build inverse table
        std::cerr << "Look Up: " << std::endl;
        for(size_t i=0;i<256;++i)
        {
            const char C = char(i);
            const int  std_code = ios::base64::find_in_std(C);
            const int  url_code = ios::base64::find_in_url(C);
            if(url_code!=std_code)
            {
                //std::cerr << "#" << i << " -> " << std_code << "|" << url_code << std::endl;
                Y_ASSERT(url_code==-1||std_code==-1);
            }
            const int value = max_of(std_code,url_code);
            fprintf(stdout," %2d",value);
            if(i<255) fprintf(stdout,",");
            if( 0==((1+i)&15) ) fprintf(stdout,"\n");
        }
        return 0;
    }

    std::cerr << "Encoding..." << std::endl;
    ios::icstream        fp( ios::cstdin  );
    ios::ocstream        op( ios::cstdout );
    ios::base64::encoder b64;

    b64.filter(op,fp);

}
Y_UTEST_DONE()

Y_UTEST(u64)
{
    ios::icstream        fp( ios::cstdin  );
    ios::ocstream        op( ios::cstdout );
    ios::base64::decoder u64;
    std::cerr << "Decoding..." << std::endl;
    u64.filter(op,fp);
}
Y_UTEST_DONE()

