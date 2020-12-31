

#include "y/mkl/fitting/samples.hpp"
#include "y/mkl/fitting/sequential/function.hpp"
#include "y/type/point2d.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"

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

        samples_type sa("samples");
        sample_type &s1 = sa("sample1");
        sample_type &s2 = sa("sample2");
        Y_CHECK( &sa[s1.name] == &s1 );
        Y_CHECK( &sa[s2.name] == &s2 );

        {
            const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
            const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
            const size_t _n1 = sizeof(_t1)/sizeof(_t1[0]);
            s1.add(_t1,_x1,_n1);

            const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
            const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
            const size_t _n2 = sizeof(_t2)/sizeof(_t2[0]);
            s2.add(_t2,_x2,_n2);
        }



        sa.vars << "t0" << "D1" << "D2";
        s1.vars( "t0", sa.vars("t0") );
        s1.vars( "D",  sa.vars("D1") );

        s2.vars( "t0", sa.vars("t0")   );
        s2.vars( "D",  sa.vars("D2") );

        showSample(s1);
        showSample(s2);
        showSample(sa);

        vector<double> aorg(3,0);
        sa.vars(aorg,"D1") = 0.11;
        sa.vars(aorg,"D2") = 0.12;
        sa.vars(aorg,"t0") = -200;

        Diffusion          diff;
        seq_type::function call(&diff,&Diffusion::compute);
        seq_type           F(call);

        sa.setup();

        const double s1_D2 = s1.D2(F,aorg); std::cerr << "s1_D2=" << s1_D2 << std::endl;
        const double s2_D2 = s2.D2(F,aorg); std::cerr << "s2_D2=" << s2_D2 << std::endl;
        const double sa_D2 = sa.D2(F,aorg); std::cerr << "sa_D2=" << sa_D2 << std::endl;


        saveSample("s1.dat", s1);
        saveSample("s2.dat", s2);




    }

    //! fitting circles from "pixels"
    {
        typedef point2d<unit_t>              pixel;
#define S_LIST pixel,float
        typedef sample<S_LIST>  sample_type;
        typedef samples<S_LIST> samples_type;

        samples_type circles("circles");
        sample_type &c1 = circles("circle1");
        sample_type &c2 = circles("circle2");

        showSample(c1);
        showSample(c2);
        showSample(circles);
    }

}
Y_UTEST_DONE()

