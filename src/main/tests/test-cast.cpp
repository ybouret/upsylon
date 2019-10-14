#include "y/type/class-cast.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>
#include "y/string.hpp"

using namespace upsylon;

namespace {

    template <typename T, typename U> static inline
    void checkCast()
    {
        typedef class_cast<T,U> Cast;
        static const char      *tid = typeid(T).name();
        static const char      *uid = typeid(U).name();

        std::cerr << "<" << tid << "> is " << ( Cast::derived ? "" : "NOT ") << "derived from <" << uid << ">" << std::endl;


    }

}

Y_UTEST(cast)
{
    checkCast<int,double>();
    checkCast<int,int>();
    checkCast<string,memory::ro_buffer>();
    
}
Y_UTEST_DONE()


