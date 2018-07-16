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

        std::cerr << "\ttesting access" << std::endl;
        for(size_t iter=0;iter<16;++iter)
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

            {
                matrix<T> m2 = m;
                matrix<T> m3(1+alea.leq(10),1+alea.leq(10));

                m3.swap_with(m2);
            }

            matrix<T> mT(m,matrix_transpose);

        }

        std::cerr << "\ttesting swaps" << std::endl;
        matrix<T> m(2+alea.leq(30),2+alea.leq(30));
        for(size_t r=1;r<=m.rows;++r)
        {
            for(size_t c=1;c<=m.cols;++c)
            {
                T v = support::get<T>();
                m[r][c] = v;
            }
        }
        for(size_t count=0;count<16;++count)
        {
            m.swap_rows(alea.range<size_t>(1,m.rows), alea.range<size_t>(1,m.rows) );
            m.swap_cols(alea.range<size_t>(1,m.cols), alea.range<size_t>(1,m.cols) );
        }

        std::cerr << "\ttesting minor" << std::endl;
        matrix<T> mm(m.rows-1,m.cols-1);
        for(size_t i=1;i<=m.rows;++i)
        {
            for(size_t j=1;j<=m.cols;++j)
            {
                mm.minor_of(m,i,j);
            }
        }

    }
}
Y_UTEST(matrix)
{
    std::cerr << std::dec;
    do_test<float>();
    do_test<double>();
    do_test<int>();
    do_test<string>();
    std::cerr << std::hex;
    do_test<mpn>();
    do_test<mpz>();
    do_test<mpq>();

}
Y_UTEST_DONE()

