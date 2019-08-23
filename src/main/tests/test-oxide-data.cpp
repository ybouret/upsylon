
#include "y/oxide/field/data.hpp"
#include "y/memory/cblock.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;

namespace {

    template <typename T>
    static inline void testData()
    {
        const size_t n = 1 + alea.leq(1000);
        memory::cblock_of<T> blk(n);
        std::cerr << "Field Data for " << n << "  <" << typeid(T).name() << ">" << std::endl;
        Oxide::FieldData<T> d( blk.data, blk.size );
    }


}



Y_UTEST(oxide_data)
{
    testData<double>();
    testData<float>();
    testData<char>();
    testData<string>();
    testData<mpn>();
    testData<mpz>();
    testData<mpq>();

}
Y_UTEST_DONE()

