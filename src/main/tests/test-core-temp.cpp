#include "y/utest/run.hpp"
#include "y/core/temporary-value.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-acquire.hpp"
#include "support.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace {


    template <typename T> static inline
    void temp_value()
    {
        T x = support::get<T>();
        std::cerr << x << std::endl;
        {
            const T tmp = support::get<T>();
            core::temporary_value<T> link(x,tmp);
            std::cerr << x << std::endl;
        }
        std::cerr << x << std::endl;
    }

    template <typename T> static inline
    void temp_link()
    {
        T *ptr = NULL;
        {
            Y_CHECK(NULL==ptr);
            T x = support::get<T>();
            core::temporary_link<T> link(x,&ptr);
            Y_CHECK(&x==ptr);
        }
        Y_CHECK(NULL==ptr);

    }

    static inline
    void temp_release()
    {
        matrix<double> m;
        vector<float>  v;
        {
            core::temporary_acquire<2> link;
            link.record(m);
            link.record(v);
            m.make(10,10);
            v.make(10,1.0f);
            std::cerr << "#recorded=" << link.size << std::endl;
        }
        Y_CHECK(0==m.rows);
        Y_CHECK(0==v.size());

    }

}

Y_UTEST(core_temp)
{
    std::cerr << "temporary values" << std::endl;
    temp_value<int>();
    temp_value<double>();
    std::cerr << std::endl;

    std::cerr << "temporary links" << std::endl;
    temp_link<int>();
    temp_link<string>();
    std::cerr << std::endl;

    std::cerr << "temporary acquires" << std::endl;
    temp_release();
    std::cerr << std::endl;

}
Y_UTEST_DONE()

