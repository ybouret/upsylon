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

Y_UTEST(yap_perf)
{
    size_t maxbits = 16384;
    double D       = 0.25;

    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }

    if(argc>2)
    {
        D = string_convert::to<double>(argv[2],"D");
    }


    const string filename = "yap-perf.dat";
    ios::ocstream::overwrite(filename);
    real_time_clock clk;
    for(size_t nbits=32;nbits<=maxbits; nbits <<= 1)
    {
        std::cerr << std::setw(8) << nbits << " bits: "; std::cerr.flush();
        uint64_t fticks = 0;
        uint64_t lticks = 0;
        unsigned cycles = 0;
        do
        {
            const apn a(alea,nbits);
            const apn b(alea,nbits);
            apn       ab0, ab1;
            {
                const uint64_t mark = clk.ticks();
                ab0  = apn::fmul(a,b);
                fticks += clk.ticks() - mark;
            }

            {
                const uint64_t mark = clk.ticks();
                ab1   = apn::lmul(a,b);
                lticks += clk.ticks() - mark;
            }
            Y_ASSERT(ab0==ab1);
            ++cycles;
        }
        while( clk(lticks) < D);
        std::cerr << "#" << human_readable(cycles) << " | fft: " << clk(fticks) << " | long: " << clk(lticks) << std::endl;

        const double fspeed = double(cycles)/clk(fticks);
        const double lspeed = double(cycles)/clk(lticks);
        ios::ocstream::echo(filename, "%u %g %g\n", unsigned(nbits), log10(fspeed), log10(lspeed));
    }
    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *yap::natural::instance() << std::endl;

}
Y_UTEST_DONE()


