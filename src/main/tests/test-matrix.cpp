#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/mpl/natural.hpp"
#include <typeinfo>
#include "support.hpp"

using namespace upsylon;

namespace  {

    template <typename T>
    static inline
    void do_test()
    {
        std::cerr << "sizeof(matrix<" << typeid(T).name() << ">)=" << sizeof(matrix<T>) << std::endl;
        { matrix<T> m0; }

        for(size_t iter=0;iter<16;++iter)
        {
            matrix<T> m(1+alea.leq(20),1+alea.leq(20));

            for(size_t r=1;r<=m.rows;++r)
            {
                for(size_t c=1;c<=m.cols;++c)
                {
                    T v = support::get<T>();
                    m[r][c] = v;
                }
            }
            std::cerr << "m" << m.rows << "x" << m.cols << "=" << m << std::endl;

            matrix<T> m2 = m;
            matrix<T> m3(1+alea.leq(10),1+alea.leq(10));

            m3.swap_with(m2);

        }

    }
}
Y_UTEST(matrix)
{
    
    do_test<float>();
    do_test<double>();
    do_test<int>();
    do_test<string>();
    do_test<mpn>();

    if(false)
    {
        matrix<mpn> M(3,5);
        std::cerr << "M=" << M << std::endl;
    }
}
Y_UTEST_DONE()

