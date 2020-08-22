#include "y/string.hpp"
#include "y/yap/rational.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/timings.hpp"
#include "y/type/spec.hpp"

#include "support.hpp"

using namespace upsylon;

template <typename T>
static inline void doSRZ(double D, sequence<string> &tid, sequence<double> &spd)
{
    const string &id = type_name_of<T>();
    tid << id;
    std::cerr << "[testing <" << id << "> ]" << std::endl;
    ios::ovstream  fp;
    vector<T>      obj;
    vector<size_t> len;
    
    std::cerr << "|_generating..." << std::endl;
    for(size_t i=10000+alea.leq(10000);i>0;--i)
    {
        const T tmp = support::get<T>();
        obj << tmp;
        len << tmp.serialize(fp);
    }
    Y_CHECK(obj.size()==len.size());
    std::cerr << "|_written " << obj.size() << " " << id << " in " << fp.size() << " bytes" << std::endl;
    
    std::cerr << "|_reloading..." << std::endl;
    const size_t nobj = obj.size();
    {
        ios::imstream inp(fp);
        for(size_t i=1;i<=nobj;++i)
        {
            const string  lid   = vformat("%s[%u]", *id, unsigned(i));
            size_t        shift = 0;
            const  T      tmp   = T::read(inp, shift, *lid);
            Y_ASSERT( tmp   == obj[i] );
            Y_ASSERT( shift == len[i] );
            
        }
    }
    std::cerr << "|_examples:" << std::endl;
    for(size_t i=8+alea.leq(8);i>0;--i)
    {
        std::cerr << "\t" << alea.in(obj) << std::endl;
    }
    
    std::cerr << "|_timings..." << std::endl;
    {
        const char *which = *id;
        double      speed = 0;
        Y_TIMINGS(speed,D,
                  {
            ios::imstream inp(fp);
            for(size_t i=1;i<=nobj;++i)
            {
                size_t shift = 0;
                const volatile T tmp   = T::read(inp, shift,which);
            }
        }
                  );
        speed *= nobj;
        speed *= 1e-6;
        std::cerr << "|_speed = " << speed << " Mops" << std::endl;
        spd << speed;
    }
    
    std::cerr << std::endl;
}

#include "y/string/display.hpp"

Y_UTEST(srz_io)
{
    double D = 1;
    if( argc>1 )
    {
        D = atof(argv[1]);
    }
    vector<string> tid;
    vector<double> spd;
    doSRZ<string>(D,tid,spd);
    doSRZ<apn>(D,tid,spd);
    doSRZ<apz>(D,tid,spd);
    doSRZ<apq>(D,tid,spd);

    for(size_t i=1;i<=tid.size();++i)
    {
        string_display::align(std::cerr, tid[i], 16) << " : " << spd[i] << std::endl;
    }
    
    return 0;
    
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

