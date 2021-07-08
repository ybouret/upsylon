#include "y/utest/run.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/mkl/fitting/sample/display.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/samples.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

#define Kh_SCALING 1.0e-7

namespace {

    static const double H[] =
    {
        5.01187E-08,
        1.41254E-07,
        3.0903E-07,
        5.01187E-07,
        5.7544E-07,
        7.4131E-07
    };

    static const double Li[] =
    {
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
    const double c0  = Kh0/Kl0;
    //const double L00 = 5900;
    const double L00 = 5000;

    class mwc
    {
    public:
        size_t calls;
        inline  mwc() throw() : calls(0) {}
        inline ~mwc() throw() {}
        
        
        inline double compute(const double h, const accessible<double> &aorg, const variables &vars)
        {
            const double Kh    = vars(aorg,"Kh") * Kh_SCALING;
            const double c     = vars(aorg, "c");
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
    typedef samples<double,double>             samples_type;
    typedef sequential_function<double,double> seq_type;
    typedef seq_type::function                 function;
    
    static inline void save(const sample_type &s,
                            const accessible<double> &aorg,
                            const accessible<bool>   &used,
                            const accessible<double> &aerr)
    {
        correlation<double>          corr;

        //std::cerr << "corr2: " << s.compute_corr(corr) << std::endl;
        //std::cerr << "R2:    " << s.compute_R2() << std::endl;
        display_sample::results(std::cerr,s,aorg,used,aerr);
        std::cerr << std::endl;
        
        ios::ocstream fp("mwc.dat");
        const accessible<double> &X = s.abscissa;
        const accessible<double> &Y = s.ordinate;
        const accessible<double> &Z = s.adjusted;
        
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g %g\n",X[i],Y[i],Z[i]);
        }
    }
}


Y_UTEST(mwc)
{
    
    samples_type sdb("db");                  // hold sample
    sample_type  &s = sdb.create("sample");  // the sample
    s.add(H,Li,N);                           // with data
    
    variables &vars = s.vars;
    
    vars << "Kh";
    vars << "c";
    vars << "L0";
    vars << "Vmax";
    
    
    mwc        f;
    function F(&f, &mwc::compute);
    
    const size_t   nvar = s.vars.size();
    vector<double> aorg(nvar,0);
    vector<double> aerr(nvar,0);
    vector<bool>   used(nvar,false);
    
    
    
    vars(aorg,"Kh")    = Kh0;
    vars(aorg,"c")     = c0;
    vars(aorg,"Vmax")  = 40;
    vars(aorg,"L0")    = L00;

    
    s.setup(aorg);
    s.D2(F,aorg);
   
    least_squares<double,double> lsf;
    
    lsf.grad().h = 1e-6;
    

    int cycle=0;
#define CYCLE(ID) do{\
++cycle;\
vars.only_on(used,ID);\
if(lsf.fit(s,F, aorg, used, aerr))\
{\
save(s,aorg,used,aerr);\
}\
else\
{\
throw exception("error @cycle %d",cycle);\
}\
} while(false)
 
    CYCLE("L0");

    
    
}
Y_UTEST_DONE()

