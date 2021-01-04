
#include "y/mkl/fitting/samples.hpp"
#include "y/mkl/fitting/sequential/function.hpp"
#include "y/mkl/fitting/sequential/gradient.hpp"
#include "y/type/point2d.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/lu.hpp"


using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{
    class Diffusion
    {
    public:
        inline  Diffusion() throw() {}
        inline ~Diffusion() throw() {}


        inline double compute(const double t, const accessible<double> &aorg, const variables &vars)
        {
            const double t0 = vars(aorg,"t0");
            const double D  = vars(aorg,"D");
            //std::cerr << "diffusion(" << t << "," << t0 << "," << D << ")" << std::endl;
            return t <= t0 ? 0 : sqrt( D*(t-t0) );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Diffusion);
    };

    template <typename SAMPLE>
    static inline void showSample( const SAMPLE &s )
    {
        std::cerr << "sample <" << s.name << "> #" << s.count() << std::endl;
        std::cerr << "       |_variables: " << s.vars << std::endl;
    }

    template <typename SAMPLE>
    static inline void saveSample(const char *filename, const SAMPLE &s)
    {
        ios::ocstream fp(filename);
        for(size_t i=1;i<=s.count();++i)
        {
            fp("%g %g %g\n", s.abscissa[i], s.ordinate[i], s.adjusted[i]);
        }
    }

}

Y_UTEST(fitting_diff)
{
    {
        typedef sample<double,double>  sample_type;
        typedef samples<double,double> samples_type;
        typedef sequential_function<double,double> seq_type;
        
        Diffusion          diff;
        seq_type::function call(&diff,&Diffusion::compute);
        seq_type           F(call);
        sequential_gradient<double,double> G;

        G.F = &F;

        // standalone samples
        samples_type  db("db");
        sample_type  &S1 = db.create("S1");
        sample_type  &S2 = db.create("S2");

        *S1 << "t0" << "D";
        *S2 = *S1;
        
      
      

        {
            const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
            const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
            const size_t _n1 = sizeof(_t1)/sizeof(_t1[0]);
            S1.add(_t1,_x1,_n1);

            const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
            const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
            const size_t _n2 = sizeof(_t2)/sizeof(_t2[0]);
            S2.add(_t2,_x2,_n2);
        }

        showSample(S1);
        showSample(S2);
        
        
        samples_type  sa("samples");
        sample_type  &s1 = sa.create("s1",S1);
        sample_type  &s2 = sa.create("s2",S2);
        
       
        
        *sa << "t0" << "D1" << "D2";
        
        (*s1)( "t0", (*sa)("t0") );
        (*s1)( "D", (*sa)("D1") );

        (*s2)( "t0", (*sa)("t0") );
        (*s2)( "D", (*sa)("D2") );

        showSample(sa);
        showSample(s1);
        showSample(s2);
        
        std::cerr << "independent: " << std::endl;
        vector<double> a2(2,0);
        vector<bool>   u2(2,true);
        vector<double> beta2(2,0);
        matrix<double> alpha2(2,2);
        db.setup(a2);
        (*S1)(a2,"D")  = 0.11;
        (*S1)(a2,"t0") = -200;
        const double S1_D2 = S1.D2(F,a2); std::cerr << "S1_D2=" << S1_D2 << std::endl;
        const double S2_D2 = S2.D2(F,a2); std::cerr << "S2_D2=" << S2_D2 << std::endl;
        
        const double S1_D2a = S1.fullD2(alpha2, beta2, F, G, a2, u2);
        std::cerr << "S1_D2a=" << S1_D2a << std::endl;
        std::cerr << "beta  =" << beta2  << std::endl;
        std::cerr << "alpha =" << alpha2 << std::endl;
        
        const double S2_D2a = S2.fullD2(alpha2, beta2, F, G, a2, u2);
        std::cerr << "S2_D2a=" << S2_D2a << std::endl;
        std::cerr << "beta  =" << beta2  << std::endl;
        std::cerr << "alpha =" << alpha2 << std::endl;
        

        std::cerr << std::endl;
        std::cerr << "coupled: " << std::endl;

        vector<double> a3(3,0);
        vector<bool>   u3(3,true);
        vector<double> beta3(3,0);
        matrix<double> alpha3(3,3);
        sa.setup(a3);
        (*sa)(a3,"D1") = 0.11;
        (*sa)(a3,"D2") = 0.11;
        (*sa)(a3,"t0") = -200;

        const double s1_D2  = s1.D2(F,a3); std::cerr << "s1_D2=" << s1_D2 << std::endl;
        const double s2_D2  = s2.D2(F,a3); std::cerr << "s2_D2=" << s2_D2 << std::endl;
        const double sa_D2  = sa.D2(F,a3); std::cerr << "sa_D2=" << sa_D2 << std::endl;
        const double sa_D2a = sa.fullD2(alpha3, beta3, F, G, a3, u3);
        std::cerr << "sa_D2a=" << sa_D2a << std::endl;
        std::cerr << "beta  =" << beta3  << std::endl;
        std::cerr << "alpha =" << alpha3 << std::endl;
        
        if(LU::build(alpha2)) std::cerr << "OK2" << std::endl;
        if(LU::build(alpha3)) std::cerr << "OK3" << std::endl;
        
        std::cerr << std::endl;
    }

    
}
Y_UTEST_DONE()

