#include "y/math/adjust/sequential/explode.hpp"
#include "y/math/adjust/least-squares.hpp"
#include "y/math/adjust/samples.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;
using namespace Adjust;

namespace {

    template <typename T>
    class Damped : public ODE::ExplicitScheme<T>
    {
    public:
        explicit Damped() throw()
        {
        }

        virtual ~Damped() throw()
        {
        }

        inline virtual size_t dimensions() const throw() { return 2; }

        inline virtual T start() const throw() { return 0; }

        inline virtual T delta() const throw() { return 0.01; }

        virtual void   setup(addressable<T>       &Y,
                             const accessible<T>  &aorg,
                             const Variables      &vars) const
        {
            Y_ASSERT( dimensions() == Y.size() );
            //std::cerr << "-- setup" << std::endl;
            Y[1] =  vars(aorg,"y0");
            Y[2] = 0;
        }

        virtual void   rates(addressable<T>       &dYdt,
                             const T               ,
                             const accessible<T>  &Y,
                             const accessible<T>  &aorg,
                             const Variables      &vars)
        {
            Y_ASSERT( dimensions() == dYdt.size() );
            Y_ASSERT( dimensions() ==    Y.size() );
            const T mu = vars(aorg,"mu");
            const T om = vars(aorg,"omega");
            const T position = Y[1];
            const T speed    = Y[2];
            dYdt[1] = speed;
            dYdt[2] = -mu * speed - (om*om)*position;
        }

        virtual T query(const T              ,
                        const accessible<T> &Y,
                        const accessible<T> &,
                        const Variables     & ) const
        {
            return Y[1]; // position
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Damped);
    };

    template <typename T>
    void doAdjust()
    {
        const size_t n = 10 + alea.leq(10);

        typename Type<T>::Series X = new vector<T>(n);
        typename Type<T>::Series Y = new vector<T>(n);
        typename Type<T>::Series Z = new vector<T>(n);

        T curr = 0;
        for(size_t i=1;i<=n;++i)
        {
            curr += alea.to<T>();
            X[i]  = curr;
            Y[i]  = exp_of( -T(0.3) * X[i] ) * cos_of( 2.7 * X[i] );
        }


        Sample<T> sample( X,Y,Z );

        {
            ios::ocstream fp("adjode.dat");
            sample.save(fp);
        }


        typename ODE::ExplicitScheme<T>::Pointer crunch = new Damped<T>();
        typename ODE::ExplicitSolver<T>::Pointer solver = ODE::DriverCK<T>::New();

        ExplODE<T>      explode(solver,crunch);
        LeastSquares<T> LS(true);

        Variables &vars = sample.variables;
        vars << "mu" << "omega" << "y0";

        vector<T>    aorg( vars.sweep(),  0 );
        vector<bool> used( aorg.size(), true);
        vector<T>    aerr( aorg.size() );

        vars(aorg,"mu")    = 0.5;
        vars(aorg,"omega") = 2.5;
        vars(aorg,"y0")    = 1;
        vars(used,"y0")   = false;

        if(LS.fit(sample, explode, aorg, used, aerr))
        {
            vars.display(std::cerr, aorg, aerr);

            {
                ios::ocstream fp("adjode.dat");
                sample.save(fp);
            }

            {
                ios::ocstream fp("adjfcn.dat");
                T x = 0;
                const T dx = 0.02;

                fp("%g %g\n",x,explode.start(x,aorg,vars));
                for(x+=dx;x<=X->back();x+=dx)
                {
                    fp("%g %g\n",x,explode.reach(x,aorg,vars));
                }

            }
            vars.display(std::cerr,used,"\t (*) use ");
            correlation<T> corr;
            std::cerr << "corr=" << sample.computeCorrelation(corr) << std::endl;
            std::cerr << "R2  =" << sample.computeR2() << std::endl;
            std::cerr << std::endl;

            std::cerr << "second pass" << std::endl;
            LS.verbose = true;
            atom::ld(used,false);
            vars(used,"y0")   = true;

            vars.display(std::cerr,used,"\t (*) use ");

            if(LS.fit(sample, explode, aorg, used, aerr))
            {
                vars.display(std::cerr, aorg, aerr);
                std::cerr << std::endl;

                std::cerr << "thirds pass" << std::endl;
                atom::ld(used,true);

                vars.display(std::cerr,used,"\t (*) use ");

                if(LS.fit(sample, explode, aorg, used, aerr))
                {
                    vars.display(std::cerr, aorg, aerr);
                }
            }

        }
    }

}


Y_UTEST(adjust_ode)
{


    doAdjust<float>();
    //doAdjust<double>();




}
Y_UTEST_DONE()

