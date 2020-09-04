#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/object.hpp"

using namespace upsylon;

namespace {

    class dummy : public object
    {
    public:
        dummy *next, *prev;
        int a;

        inline  dummy() throw() : next(0), prev(0), a( alea.full<short>() ) {}
        inline virtual ~dummy() throw() {}

        inline friend std::ostream & operator<<( std::ostream &os, const dummy &d)
        {
            os << "(" << d.a << ")" ;
            return os;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };



}

Y_UTEST(merge_sort)
{
    core::list_of_cpp<dummy> D;
    for(size_t i=alea.leq(20);i>0;--i)
    {
        D.push_back( new dummy() );
        std::cerr << *D.tail << std::endl;
    }



}
Y_UTEST_DONE()

