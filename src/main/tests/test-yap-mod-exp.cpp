
#include "y/yap/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(yap_mod_exp)
{
    if(argc>3)
    {
        const apn A = apn::parse(argv[1]);
        const apn B = apn::parse(argv[2]);
        const apn N = apn::parse(argv[3]);
        (std::cerr << A << "^" << B << " [" << N << "] = ").flush();
        const apn X = apn::mod_exp(A,B,N);
        std::cerr << X << std::endl;
    }
    else
    {
        std::cerr << "usage: " << program << " A B N => A^B [N]" << std::endl;
    }
}
Y_UTEST_DONE()

