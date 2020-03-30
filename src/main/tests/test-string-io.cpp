#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/timings.hpp"

#include "support.hpp"

using namespace upsylon;

Y_UTEST(string_io)
{
    ios::ovstream  fp;
    vector<string> str;
    vector<size_t> len;
    
    std::cerr << "-- generating..." << std::endl;
    for(size_t i=1000+alea.leq(1000);i>0;--i)
    {
        const string tmp = support::get<string>();
        str << tmp;
        len << tmp.serialize(fp);
    }
    Y_CHECK(str.size()==len.size());
    std::cerr << "written " << str.size() << " strings in " << fp.size() << " bytes" << std::endl;
    
    std::cerr << "-- reloading..." << std::endl;
    const size_t nstr = str.size();
    {
        ios::imstream inp(fp);
        for(size_t i=1;i<=nstr;++i)
        {
            const string  id    = vformat("string[%u]", unsigned(i));
            size_t        shift = 0;
            const  string tmp   = string::read(inp, shift, *id);
            Y_ASSERT( tmp   == str[i] );
            Y_ASSERT( shift == len[i] );

        }
    }
    
    std::cerr << "-- timings..." << std::endl;
    {
        static const char *which = "temporary";
        double speed = 0;
        Y_TIMINGS(speed,1,
                  {
            ios::imstream inp(fp);
            for(size_t i=1;i<=str.size();++i)
            {
                size_t shift = 0;
                const volatile string tmp   = string::read(inp, shift,which);
            }
        }
                  );
        speed *= nstr;
        speed *= 1e-6;
        std::cerr << "speed=" << speed << " Mops" << std::endl;
    }
    
}
Y_UTEST_DONE()

