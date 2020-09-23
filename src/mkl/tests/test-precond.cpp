#include "y/mkl/kernel/preconditioning.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    template <typename T>
    void do_precond()
    {
        size_t n = 3;

        matrix<T>    curv(n,n);
        vector<T>    omega(n,0);
        vector<bool> used(n,true);

        for(size_t i=1;i<=n;++i)
        {
            curv[i][i] = 10 * alea.to<double>();
            curv[i][i] = 1 - 1.0/(1000.0+i);
            //curv[i][i] = 1;
        }
        preconditioning<T> precond;
        //used[2] = false;
        
        precond.run(omega,curv,used);



    }

}

Y_UTEST(precond)
{

    do_precond<float>();
    
    

}
Y_UTEST_DONE()
