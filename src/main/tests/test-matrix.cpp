#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
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

        for(size_t iter=0;iter<10;++iter)
        {
            matrix<T> m(1+alea.leq(10),1+alea.leq(10));

            for(size_t r=1;r<=m.rows;++r)
            {
                for(size_t c=1;c<=m.cols;++c)
                {
                    T v = support::get<T>();
                    m[r][c] = v;
                }
            }
            std::cerr << "m" << m.rows << "x" << m.cols << "=" << m << std::endl;
        }

    }
}
Y_UTEST(matrix)
{
    matrix_data m(3,5,4);

    std::cerr << "m.allocated=" << m.allocated << std::endl;
    std::cerr << "r_indx=" << m.r_indx << std::endl;
    std::cerr << "c_indx=" << m.c_indx << std::endl;

    do_test<float>();
    do_test<double>();
    do_test<int>();
    do_test<string>();
}
Y_UTEST_DONE()

