
#include "y/sibyl/types.hpp"
#include "y/utest/run.hpp"
#include "y/code/hr-ints.hpp"
#include "y/os/rt-clock.hpp"
#include <iomanip>

using namespace upsylon;
using namespace sibyl;

namespace {
    
    
    template <typename CORE>
    static inline uint64_t add8()
    {
        uint64_t tmx = 0;
        for(uint64_t A=0;A<256;++A)
        {
            for(uint64_t B=0;B<256;++B)
            {
                const uint8_t a = uint8_t(A);
                const uint8_t b = uint8_t(B);
                CORE          s = 0;
                CORE          c = 0;
                const uint64_t mark = rt_clock::ticks();
                slick::add_with_carry<CORE,uint8_t>(c,s,a,b);
                tmx += rt_clock::ticks()-mark;
                volatile const uint64_t S = s;
                volatile const uint64_t C = c;
                Y_ASSERT(A+B==S+(C<<8));
            }
        }
        return tmx;
    }
    
    template <typename CORE>
    static inline uint64_t add16()
    {
        uint64_t tmx = 0;
        for(size_t iter=0;iter<65536;++iter)
        {
            const uint64_t A = alea.full<uint16_t>();
            const uint64_t B = alea.full<uint16_t>();
            const uint16_t a = uint16_t(A);
            const uint16_t b = uint16_t(B);
            CORE           s = 0;
            CORE           c = 0;
            const uint64_t mark = rt_clock::ticks();
            slick::add_with_carry<CORE,uint16_t>(c,s,a,b);
            tmx += rt_clock::ticks()-mark;
            const uint64_t S = s;
            const uint64_t C = c;
            Y_ASSERT(A+B==S+(C<<16));
        }
        return tmx;
    }
    
    template <typename CORE>
    static inline uint64_t add32()
    {
        uint64_t tmx = 0;
        for(size_t iter=0;iter<65536;++iter)
        {
            const uint64_t A = alea.full<uint32_t>();
            const uint64_t B = alea.full<uint32_t>();
            const uint32_t a = uint32_t(A);
            const uint32_t b = uint32_t(B);
            CORE           s = 0;
            CORE           c = 0;
            const uint64_t mark = rt_clock::ticks();
            slick::add_with_carry<CORE,uint32_t>(c,s,a,b);
            tmx += rt_clock::ticks()-mark;
            const uint64_t S = s;
            const uint64_t C = c;
            Y_ASSERT(A+B==S+(C<<32));
        }
        return tmx;
    }
    
    template <typename CORE>
    static inline uint64_t add64()
    {
        uint64_t tmx = 0;
        for(size_t iter=0;iter<65536;++iter)
        {
            uint64_t A = alea.full<uint64_t>();
            uint64_t B = alea.full<uint64_t>();
            uint64_t S = 0;
            uint64_t C = 0;
            const uint64_t mark = rt_clock::ticks();
            slick::add_with_carry<CORE,uint64_t>(C,S,A,B);
            tmx += rt_clock::ticks()-mark;
        }
        return tmx;
    }
    
}

#define TEST(NAME,SOURCE,TARGET) do {\
std::cerr << #NAME << " " << std::setw(9) << "uint" #SOURCE  "_t" << " with " << std::setw(9) << "uint" #TARGET  "_t" << " : ";\
rt_clock clk;     \
uint64_t tmx = 0; \
unsigned cyc = 0; \
double   ell = 0; \
double   ops = (SOURCE>>3);\
do { ++cyc; tmx += NAME##SOURCE<uint##TARGET##_t>(); } while( (ell=clk(tmx)) < duration );\
std::cerr << (ops*double(cyc)/ell) << std::endl;\
} while(false)

#include "y/string/convert.hpp"

Y_UTEST(sibyl_t)
{
    
    double duration = 1.0;
    if(argc>1)
    {
        duration = string_convert::to<double>(argv[1],"duration");
    }
    
    TEST(add,8,8);
    TEST(add,8,16);
    TEST(add,8,32);
    TEST(add,8,64);
    std::cerr << std::endl;

    TEST(add,16,16);
    TEST(add,16,32);
    TEST(add,16,64);
    std::cerr << std::endl;
    
    TEST(add,32,32);
    TEST(add,32,64);
    std::cerr << std::endl;

    TEST(add,64,64);
}
Y_UTEST_DONE()

