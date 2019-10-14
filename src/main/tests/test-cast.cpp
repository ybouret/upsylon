#include "y/type/class-cast.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>
#include "y/string.hpp"

using namespace upsylon;

namespace {

    template <typename T, typename U> static inline
    void checkCast()
    {
        typedef class_cast<T,U> __cast;
        static const char      *tid = typeid(T).name();
        static const char      *uid = typeid(U).name();

        std::cerr << "<" << tid << "> is " << ( __cast::derived ? "" : "NOT ") << "derived from <" << uid << ">" << std::endl;
    }

    
}

Y_UTEST(cast)
{
    checkCast<int,double>();
    checkCast<int,int>();
    checkCast<memory::ro_buffer,string>();
    checkCast<string,memory::rw_buffer>();

    {
        int    i = 10;
        double x = class_cast<double,int>::_(i);
        std::cerr << i << "=>" << x << std::endl;
    }
    
    
}
Y_UTEST_DONE()


