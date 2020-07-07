#include "y/randomized/marsaglia.hpp"
#include "y/randomized/isaac.hpp"
#include "y/randomized/mersenne-twister.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/utils.hpp"
#include "y/mkl/triplet.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    static inline
    void check( const array<T> &data )
    {
        const size_t N = data.size(); Y_ASSERT(N>1);
        double ave    = 0;
        for(size_t i=N;i>0;--i)
        {
            const double r = data[i];
            ave    += r;
        }
        ave /= N;
        double sig = 0;
        for(size_t i=N;i>0;--i)
        {
            sig += square_of(data[i]-ave);
        }
        sig= sqrt(sig/(N-1));
        std::cerr << "\t|_average=" << ave << ", stddev=" << sig << std::endl;
    }

    template <typename T> static inline
    void check_range( randomized::bits &ran )
    {
        for(size_t iter=0;iter<10000;++iter)
        {
            mkl::triplet<T> t = { ran.full<T>(), 0, ran.full<T>() };
            cswap_increasing(t.a,t.c); assert(t.a<=t.c);
            t.b = ran.range(t.a, t.c);
            //std::cerr << "t=" << t << std::endl;
            Y_ASSERT(t.is_increasing());
        }
    }


    static inline void check_bits( randomized::bits &ran )
    {
        std::cerr << "Checking Bits..." << std::endl;
        const size_t ITER=16000;
        vector<double> rd(ITER,as_capacity);
        vector<float>  rf(ITER,as_capacity);
        vector<double> sd(ITER,as_capacity);
        vector<float>  sf(ITER,as_capacity);
        for(size_t i=0;i<ITER;++i)
        {
            rd.push_back(ran.to<double>());
            rf.push_back(ran.to<float>());
            sd.push_back(ran.symm<double>());
            sf.push_back(ran.symm<float>());
        }
        check(rd);
        check(rf);
        check(sd);
        check(sf);
        vector<uint16_t> arr(5);
        ran.fill(*arr, arr.size() * sizeof(uint16_t) );
        std::cerr << "\tarr=" << arr << std::endl;
        std::cerr << "\tranges..." << std::endl;
        check_range<int>(ran);
        check_range<uint32_t>(ran);
    }
}

Y_UTEST(bits)
{
    const bool Marsaglia_is_OK =randomized::Marsaglia::test();
    Y_CHECK(Marsaglia_is_OK);

    randomized::cstdbits         ran;
    randomized::ISAAC<4>         isaac4;
    randomized::ISAAC<8>         isaac8;
    randomized::mersenne_twister mt;

    check_bits(ran);
    check_bits(alea);
    check_bits(isaac4);
    check_bits(isaac8);
    check_bits(mt);
    check_bits(randomized::bits::simple());
    check_bits(randomized::bits::crypto());

}
Y_UTEST_DONE()


