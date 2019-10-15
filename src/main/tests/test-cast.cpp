#include "y/type/auto-cast.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>
#include "y/string.hpp"

using namespace upsylon;

namespace {

    template <typename T, typename U> static inline
    void checkCast()
    {
        typedef auto_cast<T,U> __cast;
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
        double x = auto_cast<double,int>::_(i);
        std::cerr << i << "=>" << x << std::endl;
        double &y = auto_cast<double,double>::_(x);
        std::cerr << x << "=>" << y << std::endl; ++x;
        std::cerr << x << "=>" << y << std::endl;
    }

#if 1
    {
        string s1       = "hello";
        const string s2 = "world";
        std::cerr << auto_cast<string,string>::_(s1) << std::endl;
        std::cerr << auto_cast<memory::ro_buffer,string>::_(s2).length() << std::endl;
    }
#endif
    
}
Y_UTEST_DONE()


