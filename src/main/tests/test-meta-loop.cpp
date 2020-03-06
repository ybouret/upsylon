#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    template <size_t I>
    struct doStep
    {
        static const size_t level = I;
        static inline void dec()
        {
            std::cerr << "\t<dec@" << level << std::endl;
            doStep<I-1>::dec();
        }

        static inline void inc()
        {
            doStep<I-1>::inc();
            std::cerr << "\t>inc@" << level << std::endl;

        }
    };

    template <>
    struct doStep<0>
    {
        static const size_t level = 0;
        static inline void dec() {
            std::cerr << "\t<dec@" << level << std::endl;
        }

        static inline void inc() {
            std::cerr << "\t>inc@" << level << std::endl;
        }
    };

    template <size_t N>
    static inline void doLoop()
    {
        doStep<N>::dec();
        doStep<N>::inc();
    }

#define DO_LOOP(I) \
std::cerr << "<Loop " << I << ">" << std::endl;\
doLoop<I>();\
std::cerr << "<Loop " << I << "/>" << std::endl << std::endl;


    static inline void doLoops()
    {
        DO_LOOP(0);
        DO_LOOP(1);
        DO_LOOP(2);
        DO_LOOP(3);
        DO_LOOP(4);
        DO_LOOP(32);
    }

}

Y_UTEST(metaloop)
{
    doLoops();
}
Y_UTEST_DONE()

