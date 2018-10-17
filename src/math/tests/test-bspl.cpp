#include "y/math/signal/b-splines.hpp"
#include "y/utest/run.hpp"
#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T,typename U>
    static inline void do_test()
    {
        vector<U> points;

        {
            U tmp;
            points << tmp;
            for(size_t i=1+alea.leq(10);i>0;--i)
            {
                tmp += support::get<U>();
                points << tmp;
            }
        }

        const unsigned dim = sizeof(U)/sizeof(T);
        {
            ios::ocstream fp( vformat("curv%u.dat",dim) );
            for(size_t i=1;i<=points.size();++i)
            {
                T *p = (T *)&points[i];
                fp("%g", p[0]);
                for(size_t j=1;j<dim;++j)
                {
                    fp(" %g",p[j]);
                }
                fp << "\n";
            }
        }

        {
            ios::ocstream fp( vformat("bspl%u.dat",dim) );
            for(T x=0;x<=T(1);x+=T(0.001))
            {
                const U v = Cubic_Bsplines(x,points);
                const T *p = (T *)&v;
                fp("%g", p[0]);
                for(size_t j=1;j<dim;++j)
                {
                    fp(" %g",p[j]);
                }
                fp << "\n";
            }
        }

    }

}


Y_UTEST(bspl)
{

    do_test< float,point2d<float> >();
    do_test< double,point3d<double> >();

}
Y_UTEST_DONE()


