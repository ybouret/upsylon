#include "y/code/base2.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

#define SHOW(FIELD) std::cerr << "\t" << #FIELD << " = " << uint64_t(base2<T>::FIELD) << std::endl

template <typename T>
static inline void show2()
{
    std::cerr << type_name_of<T>() << std::endl;
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

    for(int i=1;i<=1000; i += 1+alea.leq(30) )
    {
        std::cerr << prev_power_of_two(i) << " : " << i << " : " << next_power_of_two(i) << " =>" << integer_log2(i) << std::endl;
    }

    
}
Y_UTEST_DONE()

