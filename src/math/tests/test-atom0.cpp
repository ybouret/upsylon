#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"
#include "y/mpl/rational.hpp"
#include "y/sequence/list.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    template <typename T> T generate()
    {
         return alea.symm<T>();
    }

    template < > mpq generate()
    {
        const mpl::integer_t num = alea.full<short>();
        const mpl::word_t    den = 1 + unsigned(alea.full<uint8_t>());
        return mpq(num,den);
    }



    template <typename T>
    static inline void doCut()
    {
        list<T>   X;
        vector<T> Y;
        vector<T> Z;
        std::cerr << "With " << typeid(T).name() << std::endl;
        for(size_t i=0;i<64;++i)
        {
            const T x = generate<T>();
            const T y = generate<T>();
            X.push_back(x);
            Y.push_back(y);
            const T z = atom::tool::__cut(x,y);
            Z.push_back(z);
            std::cerr << "\tcut(" << x << "," << y << ")=" << z << std::endl;
        }
        atom::tool::cut(X,Y);
        std::cerr << "X=" << X << std::endl;
        Y_ASSERT( atom::tool::deltaSquared1D(X,Z) <= 0 );
        std::cerr << std::endl;
    }

}

Y_UTEST(atom0)
{

    doCut<float>();
    doCut<double>();
    doCut<mpq>();

}
Y_UTEST_DONE()


