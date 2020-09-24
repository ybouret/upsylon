#include "y/mkl/kernel/preconditioning.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{

    template <typename T> static inline
    void do_precond(const matrix<T>        &curv,
                    addressable<T>         &omega,
                    const accessible<bool> &used)
    {
        preconditioning<T> precond;
        T                  quality = 0;

        std::cerr << "curv=" << curv << std::endl;
        std::cerr << "used=" << used << std::endl;
        if(precond(omega,curv,&used,quality))
        {
            std::cerr << "omega  =" << omega   << std::endl;
            std::cerr << "quality=" << quality << std::endl;
        }
        else
        {
            std::cerr << "singular matrix" << std::endl;
        }
        std::cerr << std::endl;
    }

    template <typename T>
    void do_precond()
    {
        size_t n = 4;

        matrix<T>    curv(n,n);
        vector<T>    omega(n,0);
        vector<bool> used(n,true);

        const T amplitude = T(10);

        for(size_t i=1;i<=n;++i)
        {
            curv[i][i] = amplitude * alea.to<double>();
        }
        do_precond(curv,omega,used);

        for(size_t i=1;i<=n;++i)
        {
            curv[i][i] = amplitude*( T(1) - T(1)/(T(1000)+T(i)));
        }
        do_precond(curv,omega,used);

        for(size_t i=1;i<=n;++i)
        {
            curv[i][i] = amplitude;
        }
        do_precond(curv,omega,used);


    }

}

Y_UTEST(precond)
{

    do_precond<float>();
    do_precond<double>();

}
Y_UTEST_DONE()
