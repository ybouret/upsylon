#include "y/math/fit/samples-io.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(lsf3)
{
    if(argc<=1) return 0;

    Fit::Vectors<float> Vecs("vecs");

    const size_t n = Fit::IO::Load(argv[1], Vecs, 1, 2);
    std::cerr << "Loaded #" << n << std::endl;
    Y_CHECK(Vecs.AreValid());

}
Y_UTEST_DONE()


