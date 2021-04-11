#include "y/utest/run.hpp"
#include "y/yap/natural.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(yap_mul)
{
    size_t maxbits = 256;
    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }

    std::cerr << "Testing multiplication up to " << maxbits << "x" << maxbits << std::endl;

    for(size_t lbits=1;lbits<=maxbits;++lbits)
    {
        for(size_t rbits=lbits;rbits<=maxbits;++rbits)
        {
            (std::cerr << rbits+lbits << "...   \r").flush();
            const apn a(alea,lbits);
            const apn b(alea,rbits);


            const apn ab0 = apn::fmul(a,b);
            const apn ab1 = apn::lmul(a,b);
            const apn ba0 = apn::fmul(b,a);
            const apn ba1 = apn::lmul(b,a);
            Y_ASSERT(ab0==ab1);
            Y_ASSERT(ba0==ab0);
            Y_ASSERT(ba0==ba1);
        }
    }
    std::cerr << std::endl;

    

}
Y_UTEST_DONE()

#include "y/os/real-time-clock.hpp"
#include "y/ios/ocstream.hpp"
#include <cmath>
#include "y/code/hr-ints.hpp"
#include "y/concurrent/nucleus/thread.hpp"
#include "y/os/hw.hpp"
Y_UTEST(yap_mul_perf)
{
    concurrent::nucleus::thread::assign_current( hardware::nprocs()-1, program);
    
    size_t maxbits = 131072;
    double D       = 0.25;

    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }

    if(argc>2)
    {
        D = string_convert::to<double>(argv[2],"D");
    }


    //const string filename = "yap-perf.dat";
    //ios::ocstream::overwrite(filename);
    real_time_clock clk;
    for(size_t nbits=32;nbits<=maxbits; nbits <<= 1)
    {
        std::cerr << std::setw(8) << nbits << " bits/" << std::setw(7) << nbits/8 << " bytes: ";
        std::cerr.flush();
        uint64_t fticks = 0;
        uint64_t lticks = 0;
        unsigned cycles = 0;
        double   ftime  = 0;
        double   ltime  = 0;
        do
        {
            const apn a(alea,nbits);
            const apn b(alea,nbits);
            apn       ab0, ab1;
            Y_RTC_ADD(fticks,ab0=apn::fmul(a,b));
            Y_RTC_ADD(lticks,ab1=apn::lmul(a,b));
            Y_ASSERT(ab0==ab1);
            ++cycles;
            ftime = clk(fticks);
            ltime = clk(lticks);
        }
        while(ftime<D && ltime<D);
        
        const int64_t fspeed = int64_t(floor( cycles/ftime + 0.5 ));
        const int64_t lspeed = int64_t(floor( cycles/ltime + 0.5 ));
        
        std::cerr << human_readable(cycles);
        std::cerr << " | fft  : " <<  human_readable(fspeed) << "op/s";
        std::cerr << " | long : " <<  human_readable(lspeed) << "op/s";
        std::cerr << " | r = " << double(fspeed)/double(lspeed);
        std::cerr << std::endl;
        //ios::ocstream::echo(filename, "%u %g %g\n", unsigned(nbits), log10(fspeed), log10(lspeed));
    }
    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *yap::natural::instance() << std::endl;

}
Y_UTEST_DONE()


