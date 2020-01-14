
#include "y/mpl/rational.hpp"
#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/list.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    template <typename T> static inline
    void doTest1bMOD2()
    {
        typedef typename real_for<T>::type real_t;

        vector<T> v;
        list<T>   l;
        for(size_t i=alea.leq(10);i>0;--i)
        {
            const T tmp = support::get<T>();
            v.push_back(tmp);
            l.push_back(tmp);
        }
        const real_t m2v = atom::mod2(v);
        const real_t m2l = atom::mod2(l);
        std::cerr << typeid(T).name() << " : " << std::endl;
        std::cerr << "\tmod2/1=" << m2v << "," << m2l << std::endl;

        alea.shuffle(*v, v.size());
        const real_t m2a = atom::mod2(v,l), m2b = atom::mod2(l,v);
        std::cerr << "\tmod2/2=" << m2a << "," << m2b << std::endl;

    }

    template <typename T> static inline
    void doTest1bRMS()
    {
        typedef typename real_for<T>::type real_t;

        vector<T> v;
        list<T>   l;
        for(size_t i=alea.leq(10);i>0;--i)
        {
            const T tmp = support::get<T>();
            v.push_back(tmp);
            l.push_back(tmp);
        }
        std::cerr << typeid(T).name() << " : " << std::endl;

        const real_t m2v = atom::rms(v);
        const real_t m2l = atom::rms(l);
        std::cerr << "\trms/1= " << m2v << "," << m2l << std::endl;
        alea.shuffle(*v, v.size());
        const real_t m2a = atom::rms(v,l), m2b = atom::rms(l,v);
        std::cerr << "\trms/2=" << m2a << "," << m2b << std::endl;
    }

}


Y_UTEST(atom1b)
{
    std::cerr << "MOD2" << std::endl;
    doTest1bMOD2<float>();
    doTest1bMOD2<double>();
    doTest1bMOD2<unit_t>();
    doTest1bMOD2< complex<float>  >();
    doTest1bMOD2< complex<double> >();

    //doTest1bMOD2<mpn>();
    doTest1bMOD2<mpz>();
    doTest1bMOD2<mpq>();
    std::cerr << std::endl;

    std::cerr << "RMS" << std::endl;
    doTest1bRMS<float>();
    doTest1bRMS<double>();
    doTest1bRMS< complex<float>  >();
    doTest1bRMS< complex<double> >();
    std::cerr << std::endl;
}
Y_UTEST_DONE()
