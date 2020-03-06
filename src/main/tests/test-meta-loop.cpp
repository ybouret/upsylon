#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    template <size_t I>
    struct decStep
    {
        static inline void run()
        {
            std::cerr << "\tdec@" << I << std::endl;
            decStep<I-1>::run();
        }
    };

    template <>
    struct decStep<0>
    {
        static inline void run()
        {
            std::cerr << "\tdec@" << 0 << std::endl;
        }
    };

    template <size_t I>
    struct incStep
    {
        static inline void run()
        {
            incStep<I-1>::run();
            std::cerr << "\tinc@" << I << std::endl;
        }
    };

    template <>
    struct incStep<0>
    {
        static inline void run()
        {
            std::cerr << "\tinc@" << 0 << std::endl;
        }
    };




    template <size_t N>
    static inline void doLoop()
    {
        std::cerr << "Decreasing..." << std::endl;
        decStep<N>::run();
        std::cerr << "Increasing..." << std::endl;
        incStep<N>::run();
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

#if 1
namespace {

    template <size_t N> struct some_work;

    template <
    const size_t K,
    const size_t N,
    typename     T,
    typename     FUNC>
    struct doWork
    {
        static inline void run( T *a, FUNC &proc )
        {
            doWork<K-1,N,T,FUNC>::run(a,proc);
            const size_t i = some_work<N>::I[K];
            std::cerr << "\tI[" << K << "]=" << i << std::endl;
            proc( a[i] );
        }
    };

    template <
    const size_t N,
    typename     T,
    typename     FUNC>
    struct doWork<0,N,T,FUNC>
    {
        static inline void run( T *a, FUNC &proc )
        {
            const size_t i = some_work<N>::I[0];
            std::cerr << "\tI[" << 0 << "]=" << i << std::endl;
            proc( a[i] );
        }
    };



    template <> struct some_work<1>
    {
        static const size_t I[4];

        template <typename T, typename FUNC>
        static inline void on( T *a, FUNC &proc )
        {
            std::cerr << "some_work<1>" << std::endl;
            doWork<sizeof(I)/sizeof(I[0])-1,1,T,FUNC>::run(a,proc);
        }
    };

    const size_t some_work<1>::I[4] =
    {
        0,0,0,0
    };


    template <> struct some_work<3>
    {
        static const size_t I[5];

        template <typename T, typename FUNC>
        static inline void on( T *a, FUNC &proc )
        {
            std::cerr << "some_work<3>" << std::endl;
            doWork<sizeof(I)/sizeof(I[0])-1,3,T,FUNC>::run(a,proc);
        }
    };

    const size_t some_work<3>::I[5] =
    {
        0,1,2,1,0
    };



    template <typename T>
    void doProc( const T &a )
    {
        std::cerr << "\t-> " << a << std::endl;
    }



}
#endif


Y_UTEST(metaloop)
{
    doLoops();

    int a[8] = { 0,1,2,3,4,5,6,7 };
    some_work<1>::on(a, doProc<int> );

    some_work<3>::on(a, doProc<int> );

}
Y_UTEST_DONE()

