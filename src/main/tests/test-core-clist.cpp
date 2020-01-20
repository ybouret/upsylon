
#include "y/core/clist.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace upsylon;

namespace {

    class dummy : public object{

    public:
        dummy *next;
        dummy *prev;
        int    data;

        inline dummy() throw() : next(0), prev(0), data( alea.leq(100) ) {

        }

        inline dummy(const dummy &d) throw() : next(0), prev(0), data( d.data ) {}

        inline virtual ~dummy() throw()
        {

        }

        dummy *clone() const
        {
            return new dummy(*this);
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
        };

}


Y_UTEST(core_clist)
{
    core::clist_of_cpp<dummy> clist;

    for(size_t n=100+alea.leq(1000);n>0;--n)
    {
        clist.push_back( new dummy() );
        clist.push_front( new dummy() );
    }




}
Y_UTEST_DONE()

