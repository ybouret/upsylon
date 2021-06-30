#include "y/core/pool.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/ptr/auto.hpp"
#include "y/core/dnode.hpp"
#include "y/core/snode.hpp"

using namespace upsylon;

namespace {

    class dummy : public object {

    public:
        dummy *next;
        dummy *prev;
        size_t data;

        inline dummy() throw() : next(0), prev(0), data( alea.leq(100) ) {

        }

        inline virtual ~dummy() throw()
        {

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    class toto : public object, public dnode<toto>
    {
    public:
        explicit toto() throw() : object(), dnode<toto> ()
        {
        }

        virtual ~toto() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(toto);
    };

    class titi : public object, public snode<titi>
    {
    public:
        explicit titi() throw() : object(), snode<titi> ()
        {
        }

        virtual ~titi() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(titi);
    };


}

Y_UTEST(core_pool)
{

    core::pool_of_cpp<dummy> dpool, dpool2;

    for(size_t iter=10+alea.leq(1000);iter>0;--iter)
    {
        dpool.store( new dummy() );
    }
    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    dpool.swap_with(dpool2);

    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    dpool.swap_with(dpool2);
    std::cerr << "dpool .size=" << dpool.size << std::endl;
    std::cerr << "dpool2.size=" << dpool2.size << std::endl;

    {
        const size_t n = dpool.size/2;
        while( dpool.size > n )
        {
            dpool.reverse();
            auto_ptr<dummy> tmp = dpool.query();
            Y_ASSERT( !dpool.owns( tmp.content() ) );
        }
    }

    {
        const size_t n = dpool.size/2;
        while( dpool.size > n )
        {
            dpool.pop();
        }
    }
    
    dpool.release();
    dpool.reset();

    {
        core::list_of_cpp<dummy> dlist;
        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            dpool.store( new dummy() );
        }
        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            dlist.push_back( new dummy() );
        }
        std::cerr << "-- dpool: " << dpool.size << std::endl;
        std::cerr << "-- dlist: " << dlist.size << std::endl;
        dpool.store( dlist );
        std::cerr << "-- dpool: " << dpool.size << std::endl;
        std::cerr << "-- dlist: " << dlist.size << std::endl;
    }


    {
        dpool.release();
        for(size_t i=10;i>0;--i)
        {
            dpool.store( new dummy() );
        }
        for(const dummy *d = dpool.head; d; d=d->next )
        {
            std::cerr << " " << d->data;
        }
        std::cerr << std::endl;

        alea.shuffle_pool(dpool);
        for(const dummy *d = dpool.head; d; d=d->next )
        {
            std::cerr << " " << d->data;
        }
        std::cerr << std::endl;


    }

    {
        std::cerr << "-- fast dnode testing..." << std::endl;
        core::list_of_cpp<toto> tlist;
        core::pool_of_cpp<toto> tpool;

        for(size_t i=100+alea.lt(1000);i>0;--i)
        {
            if( alea.choice() )
            {
                tlist.push_back( new toto() );
            }
            else
            {
                tlist.push_front( new toto() );
            }
        }

        while( tlist.size )
        {
            toto *tmp = alea.choice() ? tlist.pop_back() : tlist.pop_front();
            tpool.store(tmp);
        }

    }

    {
        std::cerr << "-- fast snode testing..." << std::endl;
        core::pool_of_cpp<titi> tpool;
        for(size_t i=100+alea.lt(1000);i>0;--i)
        {
            tpool.store( new titi() );
        }
    }


}
Y_UTEST_DONE()
