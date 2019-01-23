#include "y/randomized/marsaglia.hpp"
#include "y/randomized/isaac.hpp"
#include "y/randomized/mersenne-twister.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/utils.hpp"

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

    static inline void check_bits( randomized::bits &ran )
    {
        std::cerr << "Checking Bits..." << std::endl;
        const size_t ITER=10000;
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
        std::cerr << "arr=" << arr << std::endl;
    }
}

Y_UTEST(bits)
{
    Y_CHECK(randomized::Marsaglia::test());

    randomized::cstdbits ran;
    randomized::ISAAC<4> isaac4;
    randomized::ISAAC<8> isaac8;
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


