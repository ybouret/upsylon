
#include "y/mkl/fitting/built-in/gaussians.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace {

    template <typename T> static inline
    void do_gauss()
    {

        const string &id = rtti::name_of<T>();

        // make data
        typename series<T>::type t  = new vector<T>();
        typename series<T>::type y  = new vector<T>();
        typename series<T>::type yf = new vector<T>();
        sample<T,T>              s("gaussians",t,y,yf);
        variables               &vars = *s;




        built_in::gaussians<T> G(2);
        G.make(vars);

        std::cerr << "vars=" << vars << std::endl;

        const size_t nvar = vars.sweep();
        vector<T>    aorg(nvar,0);
        vector<T>    aerr(nvar,0);
        vector<bool> used(nvar,true);

        vars(aorg,G.names[1]) = T(0.5) + 10*alea.to<T>();      // A
        vars(aorg,G.names[2]) = T(0.5) + T(0.5)*alea.to<T>();  // k
        vars(aorg,G.names[3]) = T(0.5) + 10*alea.to<T>();      // mu


        vars(aorg,G.names[4]) = T(0.5) + 10*alea.to<T>();      // A
        vars(aorg,G.names[5]) = T(0.5) + T(0.5)*alea.to<T>();  // k
        vars(aorg,G.names[6]) = T(10) + 10*alea.to<T>();      // mu

        display_variables::values(std::cerr, " (*) ", vars, aorg, NULL);

        for(T tt=-T(1); tt <= T(30); tt += T(0.1) )
        {
            s.add(tt, G.eval(tt,aorg,vars) + T(0.1) * alea.symm<T>());
        }

        for(size_t i=aorg.size();i>0;--i)
        {
            aorg[i] *= T(1) + T(0.05) * alea.symm<T>();
        }

        for(size_t i=1;i<=s.count();++i)
        {
            s.adjusted[i] = G.eval(s.abscissa[i],aorg,vars);
        }

        {
            const string  filename = "gauss-" + id + ".dat";
            ios::ocstream fp(filename);
            for(size_t i=1;i<=s.count();++i)
            {
                fp("%g %g %g\n", s.abscissa[i], s.ordinate[i], s.adjusted[i]);
            }
        }



        least_squares<T,T> ls(true);

        if(ls.fit(s,G,G,aorg,used,aerr))
        {
            display_variables::errors(std::cerr, NULL, vars, aorg, used, aerr);
            {
                const string  filename = "gauss-" + id + ".dat";
                ios::ocstream fp(filename);
                for(size_t i=1;i<=s.count();++i)
                {
                    fp("%g %g %g\n", s.abscissa[i], s.ordinate[i], s.adjusted[i]);
                }
            }
        }



    }


}

Y_UTEST(fitting_gaussians)
{
    for(size_t num=1;num<=3;++num)
    {
        built_in::bulk::gaussians g(num);
        std::cerr << g.names << std::endl;
    }

    do_gauss<float>();
    

}
Y_UTEST_DONE()

