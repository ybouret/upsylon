#include "y/math/show.hpp"
#include "y/math/round.hpp"
#include "y/utest/run.hpp"

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

}
Y_UTEST_DONE()

