

#include "y/mkl/fitting/least-squares.hpp"
#include "y/mkl/fitting/sequential/explode.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/type/spec.hpp"

#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace {

    template <typename T>
    class Damped : public ODE::ExplicitAdjust<T>
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

        inline virtual T delta() const throw() { return T(0.01); }

        virtual void   setup(addressable<T>       &Y,
                             const accessible<T>  &aorg,
                             const variables      &vars) const
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
                             const variables      &vars) const
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
                        const variables     & ) const
        {
            return Y[1]; // position
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Damped);
    };

    template <typename T>
    void doAdjust()
    {
        const string &sfx = type_name_of<T>();

        const size_t                   n = 10 + alea.leq(10);
        typename sample<T,T>::pointer  s = sample<T,T>::create("sample",n);
        
        T curr = 0;
        for(size_t i=1;i<=n;++i)
        {
            curr += alea.to<T>();
            const T x  = curr;
            const T y  = exp_of( -T(0.3) * x ) * cos_of( T(2.7) * x );
            s->add(x,y);
        }

        {
            const string filename = "adjode-" + sfx + ".dat";
            ios::ocstream fp(filename);
            s->save(fp);
        }
#if 0
        typename Type<T>::Series X = new vector<T>(n);
        typename Type<T>::Series Y = new vector<T>(n);
        typename Type<T>::Series Z = new vector<T>(n);




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


            vars.display(std::cerr,used,"\t (*) use ");

            std::cerr << std::endl;

            std::cerr << "second pass" << std::endl;
            LS.verbose = true;
            tao::ld(used,false);
            vars(used,"y0")   = true;

            vars.display(std::cerr,used,"\t (*) use ");

            if(LS.fit(sample, explode, aorg, used, aerr))
            {
                vars.display(std::cerr, aorg, aerr);
                std::cerr << std::endl;

                std::cerr << "third pass" << std::endl;
                tao::ld(used,true);

                vars.display(std::cerr,used,"\t (*) use ");

                if(LS.fit(sample, explode, aorg, used, aerr))
                {
                    vars.display(std::cerr, aorg, aerr);
                    correlation<T> corr;
                    std::cerr << "corr=" << sample.computeCorrelation(corr) << std::endl;
                    std::cerr << "R2  =" << sample.computeR2() << std::endl;
ß
                    {
                        ios::ocstream fp("adjode.dat");
                        sample.save(fp);
                    }

                    {
                        ios::ocstream fp("adjfcn.dat");
                        T x = 0;
                        const T dx = T(0.02);

                        fp("%g %g\n",x,explode.start(x,aorg,vars));
                        for(x+=dx;x<=X->back();x+=dx)
                        {
                            fp("%g %g\n",x,explode.reach(x,aorg,vars));
                        }

                    }

                }
            }

        }
#endif

    }

}



Y_UTEST(fitting_ode)
{
    doAdjust<float>();

}
Y_UTEST_DONE()

