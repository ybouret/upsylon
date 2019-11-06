#include "y/math/adjust/least-squares.hpp"
#include "y/math/adjust/samples.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/lightweight-vector.hpp"
#include "y/math/adjust/sample/load.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace math;
using namespace Adjust;

namespace {
    
    template <typename T>
    Sample<T> *createSample()
    {
        const size_t n = 1 + alea.leq(10);
        typename Type<T>::Series X = new vector<T,memory::global>(n);
        typename Type<T>::Series Y = new vector<T,memory::pooled>(n);
        typename Type<T>::Series Z = new list<T>(n);
        
        support::fill1D(X);
        for(size_t i=n;i>0;--i)
        {
            Y[i] = X[i]*X[i];
        }
        
        return new Sample<T>(X,Y,Z);
    }

    template <typename T>
    void testSamples()
    {
        typename Sample<T>::Pointer SA = createSample<T>();
        typename Sample<T>::Pointer SB = createSample<T>();
        Samples<T> S;
        S << SA << SB;
        Y_CHECK(S.count() == SA->count() + SB->count() );
    }

    struct diffusion
    {

        double compute(const double               t,
                       const accessible<double>  &aorg,
                       const Variables           &vars )
        {
            const double t0    = vars(aorg,"t0");
            const double slope = vars(aorg,"slope");
            if(t<=t0||slope<=0)
            {
                return 0;
            }
            else
            {
                return sqrt( slope * (t-t0) );
            }
        }

    };
    
    template <typename T>
    void showLS( LeastSquares<T> &LS )
    {
        std::cerr << "LS.lambdas=" << LS.lambdas << std::endl;
        for(unit_t i=LS.lambdas.lower;i<=LS.lambdas.upper;++i)
        {
            std::cerr << LS.lambdas[i] << '/';
        }
        std::cerr << std::endl;
    }

}

#include "y/ios/ocstream.hpp"
#include "y/math/kernel/atom.hpp"

Y_UTEST(adjust)
{
    LeastSquares<float>  fLS(true); showLS(fLS);
    LeastSquares<double> dLS(true); showLS(dLS);
    
    testSamples<float>();
    testSamples<double>();


    const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};

    const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};

    Type<double>::Series t1 = new lightweight_vector<double>( aliasing::_(_t1), sizeof(_t1)/sizeof(_t1[0]) );
    Type<double>::Series x1 = new lightweight_vector<double>( aliasing::_(_x1), sizeof(_x1)/sizeof(_x1[0]) );
    Type<double>::Series f1 = new vector<double>(t1->size());

    Type<double>::Series t2 = new lightweight_vector<double>( aliasing::_(_t2), sizeof(_t2)/sizeof(_t2[0]) );
    Type<double>::Series x2 = new lightweight_vector<double>( aliasing::_(_x2), sizeof(_x2)/sizeof(_x2[0]) );
    Type<double>::Series f2 = new vector<double>(t2->size());


    Samples<double> S;
    Sample<double> &S1 = S(t1,x1,f1);
    Sample<double> &S2 = S(t2,x2,f2);

    Y_CHECK(S.count()==S1.count() + S2.count() );
    
    
    Variables &vars = S.variables;
    vars << "t0" << "slope1" << "slope2";
    
    Variables &vars1 = S1.variables;  vars1( vars["t0"] )( vars["slope1"], "slope");
    Variables &vars2 = S2.variables;  vars2( vars["t0"] )( vars["slope2"], "slope");




    diffusion              diff;
    Type<double>::Function F( &diff, & diffusion::compute );
    vector<double>         aorg(3);
#define INI_T0 -100
#define INI_S1 0.1
#define INI_S2 0.2

    vars(aorg,"t0")     = INI_T0;
    vars(aorg,"slope1") = INI_S1;
    vars(aorg,"slope2") = INI_S2;




    std::cerr << "vars:" << std::endl;
    vars.display(std::cerr, aorg);

    std::cerr << "vars1:" << std::endl;
    vars1.display(std::cerr, aorg);

    std::cerr << "vars2:" << std::endl;
    vars2.display(std::cerr, aorg);

    S.ready();

    const double D1 = S1.computeD2(F,aorg);
    const double D2 = S2.computeD2(F,aorg);
    const double D0 = S.computeD2(F,aorg);

    std::cerr << "D0=" << D0 << std::endl;
    std::cerr << "D1=" << D1 << std::endl;
    std::cerr << "D2=" << D2 << std::endl;

    {
        ios::ocstream fp("s1.dat");
        S1.save(fp);
    }

    {
        ios::ocstream fp("s2.dat");
        S2.save(fp);
    }

    vector<bool>     used( aorg.size(), true );
    Gradient<double> grad;
    vector<double>   dFda( aorg.size(), -1 );
    vector<double>   aerr( aorg.size(),  0 );

    SequentialFunction<double> SF( F );

    grad( dFda, SF, 300.0, aorg, vars1, used );
    std::cerr << "dF1da=";
    vars1.display(std::cerr, dFda, "dF_");

    grad( dFda, SF, 300.0, aorg, vars2, used );
    std::cerr << "dF2da=" << dFda << std::endl;
    vars2.display(std::cerr, dFda, "dF_");

    const size_t nvar = aorg.size();
    vector<double> beta(nvar,0);
    matrix<double> alpha(nvar,nvar);


    std::cerr << "S1:" << std::endl;
    const double D12 = S1.computeD2(alpha, beta, SF, aorg, used, grad);
    std::cerr << "beta1="  << beta  << std::endl;
    std::cerr << "alpha1=" << alpha << std::endl;



    std::cerr << "S2: " << std::endl;
    const double D22 = S2.computeD2(alpha, beta, SF, aorg, used, grad);
    std::cerr << "beta2="  << beta  << std::endl;
    std::cerr << "alpha2=" << alpha << std::endl;
    
    
    std::cerr << "S: " << std::endl;
    const double D02 = S.computeD2(alpha, beta, SF, aorg, used, grad);
    std::cerr << "beta="  << beta  << std::endl;
    std::cerr << "alpha=" << alpha << std::endl;
    
    
    std::cerr << "D12=" << D12 << "/" << D1 << std::endl;
    std::cerr << "D22=" << D22 << "/" << D2 << std::endl;
    std::cerr << "D02=" << D02 << "/" << D0 << std::endl;

    
    LeastSquares<double> LS(true);

    //LS.verbose = false;

    Y_ASSERT(LS.fit(S1, SF, aorg, used, aerr));
    S1.variables.display(std::cerr, aorg, aerr, "S1.");
    {
        ios::ocstream fp("s1.dat");
        S1.save(fp);
    }
    correlation<double> corr;
    std::cerr << "S1.corr=" << S1.computeCorrelation(corr) << std::endl;
    std::cerr << "S1.R2  =" << S1.computeR2() << std::endl;
    std::cerr << std::endl;



    Y_ASSERT(LS.fit(S2, SF, aorg, used, aerr));
    S2.variables.display(std::cerr, aorg, aerr, "S2.");
    {
        ios::ocstream fp("s2.dat");
        S2.save(fp);
    }
    std::cerr << "S2.corr=" << S2.computeCorrelation(corr) << std::endl;
    std::cerr << "S2.R2  =" << S2.computeR2() << std::endl;
    std::cerr << std::endl;



    Y_ASSERT(LS.fit(S,SF,aorg,used,aerr));
    S.variables.display(std::cerr, aorg, aerr, "S.");
    {
        ios::ocstream fp("s.dat");
        S1.save(fp);
        fp <<'\n';
        S2.save(fp);
    }
    std::cerr << std::endl;
    std::cerr << "S1.corr=" << S1.computeCorrelation(corr) << std::endl;
    std::cerr << "S2.corr=" << S2.computeCorrelation(corr) << std::endl;
    std::cerr << "S1.R2  =" << S1.computeR2() << std::endl;
    std::cerr << "S2.R2  =" << S2.computeR2() << std::endl;
    std::cerr << "S.R2   =" << S.computeR2()  << std::endl;

    {
        ios::ocstream fp( ios::cstderr );
        S.writeLog(fp, aorg, used, aerr);
    }

    Y_CHECK( vars1.lookFor(aorg,"t0")    );
    Y_CHECK( vars1.lookFor(aorg,"slope") );
    Y_CHECK( vars2.lookFor(aorg,"t0")    );
    Y_CHECK( vars2.lookFor(aorg,"slope") );
    Y_CHECK( vars.lookFor(aorg,"t0") );
    Y_CHECK( vars.lookFor(aorg,"slope1") );
    Y_CHECK( vars.lookFor(aorg,"slope2") );

}
Y_UTEST_DONE()


namespace {

    struct simple
    {
        inline double compute( double x, const accessible<double> &aorg, const Variables &vars )
        {
            return vars(aorg,"a") * x + vars(aorg,"b");
        }
    };

}
Y_UTEST(adjust2)
{
    Sample<double>::Pointer sample = Sample<double>::CreateWith< vector<double> >();
    if(argc>1)
    {
        const size_t np = Load::From(argv[1], *sample, 1, 2);
        std::cerr << "Loaded " << np << std::endl;

        sample->variables << "a" << "b";
        vector<double> aorg( sample->variables.sweep(), 0 );
        vector<double> aerr( aorg.size(), 0 );
        bVector        used( aorg.size(), true );

        LeastSquares<double> ls(true);
        simple line;
        Type<double>::Function F( &line, &simple::compute );

        Y_CHECK(ls.fit(*sample, F, aorg, used, aerr));
        


    }
}
Y_UTEST_DONE()
