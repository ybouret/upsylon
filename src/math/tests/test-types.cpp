#include "y/math/utils.hpp"
#include "y/math/show.hpp"
#include "y/math/round.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
#define ROUND(VAR) \
std::cerr << #VAR << "=" << VAR << std::endl; \
std::cerr << "\tlog_round_floor=" << log_round_floor(VAR) << std::endl;\
std::cerr << "\tlog_round_ceil =" << log_round_ceil(VAR) << std::endl

    template <typename T>
    void do_round()
    {
        std::cerr << std::endl << "-- Rounding" << std::endl;
        ROUND(numeric<T>::epsilon);
    }
}

Y_UTEST(types)
{
    math::show::info();

    do_round<float>();
    do_round<double>();

    vector<float> v( 1+alea.leq(10) );
    list<double>  l;
    vector< complex<float> > c;
    for(size_t i=1;i<=v.size();++i)
    {
        v[i] = support::get<float>()-0.5f;
        l.push_back(v[i]);
        c.push_back( support::get< complex<float> >() );
    }
    std::cerr << "v=" << v << std::endl;
    std::cerr << "l=" << l << std::endl;
    std::cerr << "c=" << c << std::endl;

    std::cerr << "max_of(v)    =" << __find<float>::max_of(v) << std::endl;
    std::cerr << "abs_max_of(v)=" << __find<float>::abs_max_of(v) << std::endl;
    std::cerr << "max_of(l)    =" << __find<float>::max_of(l) << std::endl;
    std::cerr << "abs_max_of(l)=" << __find<float>::abs_max_of(l) << std::endl;
    std::cerr << "abs_max_of(c)=" << __find<float>::abs_max_of(c) << std::endl;

}
Y_UTEST_DONE()

