

#include "y/mkl/fitting/least-squares.hpp"
#include "y/mkl/fitting/sequential/explode.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/type/rtti.hpp"

#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/fitting/sample/display.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace {
    
    template <typename T>
    class Damped : public ODE::ExplicitAdjust<T>
    {
    public:
        mutable size_t calls;
        explicit Damped() throw() : calls(0)
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
            ++calls;
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
        const string &sfx = rtti::name_of<T>();
        
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
        
        
        const string filename = "adjode-" + sfx + ".dat";
        const string savename = "adjfcn-" + sfx + ".dat";
        {
            ios::ocstream fp(filename);
            s->save(fp);
        }
        ios::ocstream::overwrite(savename);
        
        Damped<T>                               *damped = new Damped<T>();
        typename ODE::ExplicitAdjust<T>::Pointer crunch = damped;
        typename ODE::ExplicitSolver<T>::Pointer solver = ODE::DriverCK<T>::New();
        correlation<T>                           corr;
        
        explode<T>         F(solver,crunch);
        least_squares<T,T> ls(true);
        variables         &vars = **s;
        
        
        
        vars << "mu" << "omega" << "y0";
        
        vector<T>    aorg( vars.sweep(),  0 );
        vector<bool> used( aorg.size(), true);
        vector<T>    aerr( aorg.size(), 0 );
        
        
        
        const char * pass[] =
        {
            "mu:omega", "y0", "mu:omega:y0"
        };

        ls.writing = true;
        try
        {
            vector<size_t> calls;
            //for(size_t q=0;q<sizeof(flags)/sizeof(flags[0]);++q)
            {
                vars(aorg,"mu")    = T(0.6);
                vars(aorg,"omega") = T(2.6);
                vars(aorg,"y0")    = T(1);
                damped->calls      = 0;
                for(size_t k=0;k<sizeof(pass)/sizeof(pass[0]);++k)
                {
                    vars.only_on(used,pass[k]);
                    if( ls.fit(*s,F,aorg,used,aerr) )
                    {
                        display_variables::errors(std::cerr,"\t",vars,aorg,used,aerr);
                        {
                            ios::ocstream fp(filename);
                            s->save(fp);
                        }
                        std::cerr<< "R2    = " << s->compute_R2() << std::endl;
                        std::cerr<< "corr  = " << s->compute_corr(corr) << std::endl;
                        {
                            ios::ocstream fp(savename);
                            T        x = 0;
                            const T dx = T(0.02);
                            
                            fp("%g %g\n",x,F.start(x,aorg,vars));
                            for(x+=dx;x<=s->abscissa->back();x+=dx)
                            {
                                fp("%g %g\n",x,F.reach(x,aorg,vars));
                            }
                            
                        }
                        ls.writing = false;
                    }
                    else
                    {
                        std::cerr << "couldn't fit '" << pass[k] << "'" << std::endl;
                        break;
                    }
                }
                
                display_variables::values(std::cerr << "aorg" << std::endl, " (*) ", vars, aorg, "//") << std::endl;
                display_variables::values(std::cerr << "used" << std::endl, NULL,    vars, used, NULL) << std::endl;
                
                display_sample::results(std::cerr,*s, aorg, used, aerr);
                std::cerr << "#calls=" << damped->calls << std::endl;
                calls.push_back(damped->calls);

                T lower = 0, upper=0;
                s->ordinate_range(lower,upper);
                std::cerr << "lower: " << lower << " | upper:" << upper << std::endl;

            }
            std::cerr << "calls=" << calls << std::endl;
        }
        catch(const exception &e)
        {
            std::cerr << "something went wrong: " << e.what() << std::endl;
            std::cerr << e.when() << std::endl;
        }
        
    }
    
}



Y_UTEST(fitting_ode)
{
    doAdjust<double>();
    doAdjust<float>();
    
}
Y_UTEST_DONE()

