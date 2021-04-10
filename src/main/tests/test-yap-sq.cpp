
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
#include "y/code/hr-ints.hpp"

Y_UTEST(yap_sq_perf)
{
    size_t   maxbits = 1024;
    double   D       = 0.2;
    if(argc>1)
    {
        maxbits = string_convert::to<size_t>(argv[1],"maxbits");
    }
    
    if(argc>2)
    {
        D = string_convert::to<double>(argv[2],"D");
    }
    
    real_time_clock clk;
    
    for(size_t nbits=8;nbits<=maxbits;nbits<<=1)
    {
        uint64_t lticks=0;
        uint64_t fticks=0;
        std::cerr << std::setw(6) << nbits  << " : ";
        unsigned cycles = 0;
        do
        {
            const apn a(alea,nbits);
            apn       x;
            apn       y;
            
            {
                const uint64_t mark = clk.ticks();
                x = apn::fsquare(a);
                fticks += clk.ticks()-mark;
            }
            
            {
                const uint64_t mark = clk.ticks();
                y = apn::lsquare(a);
                lticks += clk.ticks()-mark;
            }
            
            Y_ASSERT(y==x);
            ++cycles;
        }
        while( clk(lticks) < D);
        std::cerr << human_readable(cycles);
        std::cerr << " | FFT  : " << clk(fticks);
        std::cerr << " | LONG : " << clk(lticks);
        //const double fspeed = cycles/clk(fticks);
        //const double lspeed = cycles/clk(lticks);
        std::cerr << std::endl;
    }
    
}
Y_UTEST_DONE()
