#include "y/mkl/fitting/gls.hpp"
#include "y/mkl/fitting/sequential/explode.hpp"
#include "y/mkl/fitting/variable/display.hpp"
#include "y/mkl/fitting/sample/display.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{
    class Damped : public ODE::ExplicitAdjust<double>
    {
    public:
        mutable size_t calls;

        inline Damped() : ODE::ExplicitAdjust<double>(), calls(0)
        {
        }

        inline virtual ~Damped() throw()
        {
        }


        inline virtual size_t dimensions() const throw() { return 2; }

        inline virtual double start() const throw() { return 0; }

        inline virtual double delta() const throw() { return 0.01; }

        virtual void   setup(addressable<double>       &Y,
                             const accessible<double>  &aorg,
                             const variables           &vars) const
        {
            Y_ASSERT( dimensions() == Y.size() );
            Y[1] =  vars(aorg,"y0");
            Y[2] = 0;
        }

        virtual void   rates(addressable<double>       &dYdt,
                             const double               ,
                             const accessible<double>  &Y,
                             const accessible<double>  &aorg,
                             const variables           &vars) const
        {
            Y_ASSERT( dimensions() == dYdt.size() );
            Y_ASSERT( dimensions() ==    Y.size() );
            const double mu = vars(aorg,"mu");
            const double om = vars(aorg,"omega");
            const double position = Y[1];
            const double speed    = Y[2];
            dYdt[1] = speed;
            dYdt[2] = -mu * speed - (om*om)*position;
            ++calls;
        }

        virtual double query(const double              ,
                             const accessible<double> &Y,
                             const accessible<double> &,
                             const variables          & ) const
        {
            return Y[1]; // position
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Damped);
    };
}

Y_UTEST(fitting_gls)
{
    gls          ls;
    gls::shared  s = gls::single::create("trial",1024);

    const size_t n = 10 + alea.leq(10);

    {
        double curr = 0;
        for(size_t i=1;i<=n;++i)
        {
            curr += alea.to<double>();
            const double x  = curr;
            const double y  = exp( -0.35 * x ) * cos( 2.3 * x );
            s->add(x,y);
        }
    }

    Damped                              *damped = new Damped();
    ODE::ExplicitAdjust<double>::Pointer crunch = damped;
    correlation<double>                           corr;

    explode<double>    F(crunch);
    variables         &vars = **s;



    vars << "mu" << "omega" << "y0";

    vector<double>    aorg( vars.sweep(), 0 );
    vector<bool>      used( aorg.size(), true);
    vector<double>    aerr( aorg.size(),  0 );

    const char * pass[] =
    {
        "mu:omega", "y0", "mu:omega:y0"
    };

    const string filename = "glsfit.dat";
    const string savename = "glsfcn.dat";

    try
    {

        vars(aorg,"mu")    = 0.6;
        vars(aorg,"omega") = 2.4;
        vars(aorg,"y0")    = 1;
        damped->calls      = 0;
        for(size_t k=0;k<sizeof(pass)/sizeof(pass[0]);++k)
        {
            vars.only_on(used,pass[k]);
            if( ls(*s,F,aorg,used,aerr) )
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
                    double        x  = 0;
                    const double  dx = 0.02;

                    fp("%g %g\n",x,F.start(x,aorg,vars));
                    for(x+=dx;x<=s->abscissa->back();x+=dx)
                    {
                        fp("%g %g\n",x,F.reach(x,aorg,vars));
                    }

                }
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

        double lower = 0, upper=0;
        s->ordinate_range(lower,upper);
        std::cerr << "lower: " << lower << " | upper:" << upper << std::endl;

    }
    catch(const exception &e)
    {
        std::cerr << "something went wrong: " << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }



}
Y_UTEST_DONE()

