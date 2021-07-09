#include "y/utest/run.hpp"
#include "y/mkl/fitting/cycle.hpp"
#include "y/mkl/fitting/gls.hpp"
#include "y/mkl/fitting/sample/display.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/samples.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

#define SCALING 1.0e-7

namespace {

    static const double H[] =
    {
        0,
        5.01187E-08,
        1.41254E-07,
        3.0903E-07,
        5.01187E-07,
        5.7544E-07,
        7.4131E-07
    };

    static const double Li[] =
    {
        0,
        1.1,
        2.79,
        17.66,
        26.25,
        34.52,
        35.19
    };
    
#if 0
    static const double d7[] =
    {
        16.7,
        7.6,
        2.7,
        2.1,
        2,
        2.5
    };
#endif
    static const size_t N = sizeof(H)/sizeof(H[0]);
    
    const double Kh0 = 0.17;
    const double Kl0 = 36;
    const double L00 = 5900;

    class mwc
    {
    public:
        size_t calls;
        inline  mwc() throw() : calls(0) {}
        inline ~mwc() throw() {}
        
        
        inline double compute(const double h, const accessible<double> &aorg, const variables &vars)
        {
            const double Kh    = vars(aorg,"Kh") * SCALING;
            // const double c     = vars(aorg, "c");
            const double Kl    = vars(aorg,"Kl") * SCALING;
            const double c     = Kh/Kl;
            const double L0    = vars(aorg, "L0");
            const double Vmax  = vars(aorg, "Vmax");
            
            const double alpha = h/Kh;
            const double opa   = 1.0+alpha;
            const double opca  = 1.0+c*alpha;
            

            const double num  = alpha * opa + L0 * c * alpha * opca;
            const double den  = L0 * square_of(opca) + square_of(opa);
            
            return Vmax*num/den;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(mwc);
    };
    
    typedef sample<double,double>              sample_type;
    typedef sample_type::pointer               meta_sample;
    typedef samples<double,double>             samples_type;
    typedef sequential_function<double,double> seq_type;
    typedef seq_type::regular_functor          regular_functor;
    
    static inline void save(const sample_type        &s,
                            const accessible<double> &aorg,
                            const accessible<bool>   &used,
                            const accessible<double> &aerr,
                            regular_functor          &F)
    {
        correlation<double>          corr;

        //std::cerr << "corr2: " << s.compute_corr(corr) << std::endl;
        //std::cerr << "R2:    " << s.compute_R2() << std::endl;
        display_sample::results(std::cerr,s,aorg,used,aerr);
        std::cerr << std::endl;

        {
            ios::ocstream fp("mwc.dat");
            const accessible<double> &X = s.abscissa;
            const accessible<double> &Y = s.ordinate;
            const accessible<double> &Z = s.adjusted;

            for(size_t i=1;i<=N;++i)
            {
                fp("%.15g %.15g %.15g\n",X[i],Y[i],Z[i]);
            }
        }

        {
            const double hmin = 0;
            const double hmax = 1e-6;
            const size_t nmax = 100;
            ios::ocstream fp("mwc-func.dat");
            for(size_t i=0;i<=nmax;++i)
            {
                const double h = hmin + (i * (hmax-hmin)) / nmax;
                fp("%.15g %.15g\n",h,F(h,aorg,s.vars));
            }
        }

    }
}


Y_UTEST(mwc)
{


    meta_sample  m = sample_type::create("mwc", H, Li, N);
    sample_type &s = *m;

    variables &vars = s.vars;
    
    vars << "Kh";
    vars << "Kl";
    vars << "L0";
    vars << "Vmax";
    
    
    mwc             f;
    regular_functor F(&f, &mwc::compute);
    
    const size_t   nvar = s.vars.size();
    vector<double> aorg(nvar,0);
    vector<double> aerr(nvar,0);
    vector<bool>   used(nvar,false);
    
    
    
    vars(aorg,"Kh")    = Kh0;
    vars(aorg,"Kl")    = Kl0;
    vars(aorg,"Vmax")  = 55;
    vars(aorg,"L0")    = L00;

    
    s.setup(aorg);
    s.D2(F,aorg);

    least_squares<double,double> lsf;
    
    lsf.grad().h = 1e-6;
    

    int count=0;
#define CYCLE(ID) do{\
++count;\
vars.only_on(used,ID);\
if(lsf.fit(s,F, aorg, used, aerr))\
{\
save(s,aorg,used,aerr,F);\
}\
else\
{\
throw exception("error @cycle %d",count);\
}\
} while(false)

    for(size_t i=0;i<10;++i)
    {
        CYCLE("L0");
        CYCLE("L0:Vmax");
    }

    //CYCLE("Kl");

    vars.make_all(used,true);
    lsf.errors(s,F,aorg,used,aerr);
    display_sample::results(std::cerr,s,aorg,used,aerr);

    cycles<gls_type> cycle(s,aorg,used,aerr);

    gls_type ls;
    cycle(ls,F,"L0");


}
Y_UTEST_DONE()

