
#include "y/utest/run.hpp"
#include "y/yap/natural.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(yap_sq)
{
    size_t maxbits = 1024;
    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }
    std::cerr << "Testing squares up to " << maxbits << "x" << maxbits << std::endl;
    
    for(size_t nbits=1;nbits<=maxbits;++nbits)
    {
        (std::cerr << "#bits = " << nbits << " ...\r").flush();
        for(size_t iter=0;iter<128;++iter)
        {
            const apn a(alea,nbits);
            const apn a2f = apn::fsquare(a);
            const apn a2l = apn::lsquare(a);
            Y_ASSERT(a2f==a2l);
        }
    }
    std::cerr << std::endl;
    
}
Y_UTEST_DONE()


#include "y/os/real-time-clock.hpp"
#include "y/code/human-readable.hpp"
#include "y/concurrent/assign-main.hpp"

Y_UTEST(yap_sq_perf)
{
    (void) assign_main::by_pid(program);

    size_t   maxbits = 131072;
    double   D       = 0.5;
    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }
    
    if(argc>2)
    {
        D = string_convert::to<double>(argv[2],"D");
    }
    
    real_time_clock clk;
    
    std::cerr.precision(3);
    
    for(size_t nbits=32;nbits<=maxbits;nbits<<=1)
    {
        uint64_t lticks=0;
        uint64_t fticks=0;
        std::cerr << std::setw(8) << nbits << " bits/" << std::setw(7) << nbits/apn::word_bits << " words: ";
        std::cerr.flush();
        unsigned cycles = 0;
        double   ftime  = 0;
        double   ltime  = 0;
        do
        {
            const apn a(alea,nbits);
            apn       x;
            apn       y;
            Y_RTC_ADD(fticks,x=apn::fsquare(a));
            Y_RTC_ADD(lticks,y=apn::lsquare(a));
            Y_ASSERT(y==x);
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
        const double r = double(fspeed)/double(lspeed);
        std::cerr << " | r = " << r;
        if(r>=1) { std::cerr << " (*)"; }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *yap::natural::instance() << std::endl;
}
Y_UTEST_DONE()
