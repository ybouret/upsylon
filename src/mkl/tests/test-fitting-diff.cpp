

#include "y/mkl/fitting/samples.hpp"
#include "y/mkl/fitting/sequential/function.hpp"
#include "y/type/point2d.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{
    class Diffusion
    {
    public:

    private:

    };

    template <typename SAMPLE>
    static inline void showSample( const SAMPLE &s )
    {
        std::cerr << "sample <" << s.name << "> #" << s.count() << std::endl;
    }
}

Y_UTEST(fitting_diff)
{
    {
        typedef sample<double,double,double>  sample_type;
        typedef samples<double,double,double> samples_type;

        samples_type sdb("samples");
        sample_type &s1 = sdb("sample1");
        sample_type &s2 = sdb("sample2");
        Y_CHECK( &sdb[s1.name] == &s1 );
        Y_CHECK( &sdb[s2.name] == &s2 );

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

        showSample(s1);
        showSample(s2);
        showSample(sdb);

    }

    //! fitting circles from "pixels"
    {
        typedef point2d<unit_t>              pixel;
#define S_LIST double,pixel,unit_t
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

