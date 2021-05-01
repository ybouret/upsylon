#include "y/code/base2.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;

#define SHOW(FIELD) std::cerr << "\t" << #FIELD << " = " << uint64_t(base2<T>::FIELD) << std::endl

template <typename T>
static inline void show2()
{
    std::cerr << rtti::name_of<T>() << std::endl;
    SHOW(signed_value);
    SHOW(max_shift);
    SHOW(max_power_of_two);
}


Y_UTEST(base2)
{
    show2<uint8_t>();
    show2<int8_t>();
    show2<uint16_t>();
    show2<int16_t>();
    show2<uint32_t>();
    show2<int32_t>();

    for(int i=1;i<=100000; i += 1+int(alea.leq(3000)) )
    {
        const int      p = prev_power_of_two(i);
        const int      n = next_power_of_two(i);
        const unsigned l = integer_log2(i);
        std::cerr << p << " : " << i << " : " << n << " =>" << l << std::endl;
        Y_ASSERT( (base2<size_t>::one << l) <= size_t(i) );
    }

    
}
Y_UTEST_DONE()

