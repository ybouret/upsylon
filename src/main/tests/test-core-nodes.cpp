#include "y/core/nodes.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/core/pool.hpp"
#include "y/core/roll.hpp"

using namespace upsylon;

namespace {

    class dummy : public object
    {
    public:
        static int counter;
        const  int idx;
        const  int jdx;

        explicit dummy() throw() : idx(++counter), jdx(0)
        {
            std::cerr << "[+dummy#" << idx << "]" << std::endl;
        }

        virtual ~dummy() throw()
        {
            std::cerr << "[~dummy#" << idx << "]" << std::endl;
        }

        explicit dummy(const int b) throw() : idx(++counter), jdx(b)
        {
            std::cerr << "[+dummy#" << idx << ":" << jdx << "]" << std::endl;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    int dummy::counter = 0;

    typedef core::singly<dummy>      snode;
    typedef core::pool_of_cpp<snode> apool;
    typedef core::roll_of_cpp<snode> aroll;

    typedef core::doubly<dummy>      dnode;
    typedef core::list_of_cpp<dnode> alist;



}

Y_UTEST(core_nodes)
{
    apool p;
    aroll r;
    alist l;
    for(size_t i=1+alea.leq(16);i>0;--i)
    {
        p.store( new snode() );
        if(alea.choice())
        {
            r.push_back( new snode() );
        }
        else
        {
            r.push_front( new snode() );
        }

        p.store( snode::from<int>(1) );

        if(alea.choice())
        {
            l.push_back( new dnode() );
        }
        else
        {
            l.push_front( new dnode() );
        }

        l.push_back( dnode::from<int16_t>( alea.full<int16_t>() ) );
    }

}
Y_UTEST_DONE()

