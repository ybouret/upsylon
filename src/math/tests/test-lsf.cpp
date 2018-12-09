#include "y/math/fit/ls.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    static inline void test_vars()
    {

        Fit::Variables var;
        var << "t0" << "D1" << "D2";
        std::cerr << "var=" << var << std::endl;

        {
            Fit::Variables tmp = var;
            std::cerr << "tmp=" << tmp << std::endl;
            tmp.release();
            tmp = var;
            std::cerr << "tmp=" << tmp << std::endl;
        }

        Fit::Variables var1;
        var1("t0",var["t0"])("D",var["D1"]);
        std::cerr << "var1=" << var1 << std::endl;

        Fit::Variables var2;
        var2("t0",var["t0"])("D",var["D2"]);
        std::cerr << "var2=" << var2 << std::endl;

        std::cerr << "var.size ="  << var.size() << std::endl;
        std::cerr << "var1.size=" << var1.size() << std::endl;
        std::cerr << "var2.size=" << var2.size() << std::endl;

    }

    static inline void save(const string &fn, const array<double> &x, const array<double> &y)
    {
        ios::ocstream fp(fn);
        for(size_t i=1;i<=x.size();++i)
        {
            fp("%g %g\n", x[i], y[i]);
        }
    }

    struct diffusion
    {
        size_t ncall;
        double compute( double t, const array<double> &a, const Fit::Variables &var)
        {
            ++ncall;
            const double t0    = var(a,"t0");
            const double slope = var(a,"slope");
            return sqrt( slope*fabs(t-t0) );
        }

    };
}
Y_UTEST(lsf)
{
    std::cerr << std::endl;
    std::cerr << "-- testing some vars" << std::endl;
    test_vars();

    std::cerr << std::endl;
    std::cerr << "-- building samples" << std::endl;
    const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};

    const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};

    const lightweight_array<double> t1( (double*)_t1, sizeof(_t1)/sizeof(_t1[0]) );
    const lightweight_array<double> x1( (double*)_x1, sizeof(_x1)/sizeof(_x1[0]) );
    const lightweight_array<double> t2( (double*)_t2, sizeof(_t2)/sizeof(_t2[0]) );
    const lightweight_array<double> x2( (double*)_x2, sizeof(_x2)/sizeof(_x2[0]) );

    save("d1.dat",t1,x1);
    save("d2.dat",t2,x2);
#define INI_T0 -100
#define INI_S1 0.1
#define INI_S2 0.2

    vector<double> z1(t1.size(),0);
    vector<double> z2(t2.size(),0);

    Fit::Sample<double> S1(t1,x1,z1);
    S1.variables << "t0" << "slope";
    std::cerr << "S1.variables=" << S1.variables << std::endl;

    vector<double> a1(2);
    double &start1 = S1.variables(a1,"t0");
    double &slope1 = S1.variables(a1,"slope");

    start1 = INI_T0;
    slope1 = INI_S1;
    std::cerr << "a1=" << a1 << std::endl;

    Fit::Sample<double> S2(t2,x2,z2);
    S2.variables << "t0" << "slope";
    std::cerr << "S2.variables=" << S1.variables << std::endl;
    vector<double> a2(2);
    double &start2 = S2.variables(a2,"t0");
    double &slope2 = S2.variables(a2,"slope");

    start2 = INI_T0;
    slope2 = INI_S2;
    std::cerr << "a2=" << a2 << std::endl;



    Fit::Samples<double> SSf;
    SSf.variables << "t0" << "slope1" << "slope2";
    std::cerr << "SS.variables =" << SSf.variables << std::endl;
    vector<double> aa(3);


    double &start = SSf.variables(aa,"t0");
    double &diff1 = SSf.variables(aa,"slope1");
    double &diff2 = SSf.variables(aa,"slope2");

    start = INI_T0;
    diff1 = slope1;
    diff2 = slope2;


    correlation<double> corr;

    Fit::Sample<double>  &SS1 = SSf.add(t1,x1,z1);
    SS1.variables("t0")("slope",SSf.variables["slope1"]);
    std::cerr << "SS1.variables=" << SS1.variables << std::endl;

    Fit::Sample<double>  &SS2 = SSf.add(t2,x2,z2);
    SS2.variables("t0")("slope",SSf.variables["slope2"]);
    std::cerr << "SS2.variables=" << SS2.variables << std::endl;


    diffusion DD;
    Fit::Type<double>::Function F(&DD, & diffusion::compute );

    const double D21 = S1.computeD2(F,a1); std::cerr << "D21=" << D21 << std::endl;
    const double D22 = S2.computeD2(F,a2); std::cerr << "D22=" << D22 << std::endl;

    const double DD21 = SS1.computeD2(F,aa); std::cerr << "DD21=" << DD21 << std::endl;
    const double DD22 = SS2.computeD2(F,aa); std::cerr << "DD22=" << DD22 << std::endl;

    Y_CHECK( fabs(D21-DD21)<=0 );
    Y_CHECK( fabs(D22-DD22)<=0 );

    const double DD2 = SSf.computeD2(F,aa);
    std::cerr << "DD2=" << DD2 << std::endl;

    vector<bool>   used(2,true);
    vector<double> aerr(2);

    Fit::LeastSquares<double> ls;

    //ls.verbose = true;
    std::cerr << "Fit 1" << std::endl;
    if(ls.fit(S1,F,a1,aerr,used))
    {
        S1.variables.diplay(std::cerr,a1,aerr,"\t\t");
        save("fit1.dat",t1,z1);
        std::cerr << "\t\t\tS1.R2   =" << S1.computeR2() << std::endl;
        std::cerr << "\t\t\tS1.corr =" << S1.computeCorr(corr) << std::endl;

    }
    else
    {
        std::cerr << "S1 failure" << std::endl;
    }

    std::cerr << "Fit 2" << std::endl;
    if(ls.fit(S2,F,a2,aerr,used))
    {
        S2.variables.diplay(std::cerr,a2,aerr,"\t\t");
        save("fit2.dat",t2,z2);
        std::cerr << "\t\t\tS2.R2   =" << S2.computeR2() << std::endl;
        std::cerr << "\t\t\tS2.corr =" << S2.computeCorr(corr) << std::endl;
    }
    else
    {
        std::cerr << "S2 failure" <<std::endl;
    }

    std::cerr << "Fit 1&2" << std::endl;
    aerr.make(3,0);
    used.make(3,true);
    if(ls.fit(SSf,F,aa,aerr,used))
    {
        SSf.variables.diplay(std::cerr,aa,aerr,"\t\t");
        save("fit1b.dat",t1,z1);
        save("fit2b.dat",t2,z2);
        std::cerr << "\t\t\tSSf.R2  =" << SSf.computeR2() << std::endl;
        std::cerr << "\t\t\tS2.corr =" << SSf.computeCorr(corr) << std::endl;

    }
    else
    {
        std::cerr << "SSf failure" <<std::endl;
    }

    
}
Y_UTEST_DONE()


