
#include "y/core/list.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

namespace {

    class dummy : public object {

    public:
        dummy *next;
        dummy *prev;
        size_t data;

        inline dummy() throw() : next(0), prev(0), data( alea.leq(100) ) {

        }

        inline dummy(const dummy &d) throw() : object(), next(0), prev(0), data( d.data ) {}

        inline virtual ~dummy() throw()
        {

        }

        inline dummy *clone() const { return new dummy(*this); }
        

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    template <typename LIST>
    void fill( LIST &l )
    {
        for(size_t iter=100+alea.leq(1000);iter>0;--iter)
        {
            l.push_back(  new dummy() );
            l.push_front( new dummy() );
        }
    }

    template <typename LIST>
    void merges( LIST &l )
    {
        LIST tmp;
        fill(tmp);
        l.merge_back_copy(tmp);
        l.merge_back(tmp);
        fill(tmp);
        l.merge_front_copy(tmp);
        l.merge_front(tmp);
    }

}

Y_UTEST(core_list)
{

    std::cerr << "...fill" << std::endl;
    core::list_of_cpp<dummy>       dlist;
    core::list_of_cloneable<dummy> dclist;

    fill(dlist);
    fill(dclist);

    core::list_of_cpp<dummy>       dlist2( dlist );
    core::list_of_cloneable<dummy> dclist2( dclist);

    std::cerr << "dlist  .size=" << dlist.size    << std::endl;
    std::cerr << "dlist2 .size=" << dlist2.size   << std::endl;
    std::cerr << "dclist .size=" << dclist.size   << std::endl;
    std::cerr << "dlist2 .size=" << dclist2.size  << std::endl;

    std::cerr << "...merges" << std::endl;

    merges(dlist);
    merges(dclist);

    {
        core::list_of_cpp<dummy>   tmp;
        tmp.push_back( new dummy() );
        const size_t n       = dlist.size;
        size_t      *indices = new size_t[n];
        for(size_t i=0;i<n;++i) indices[i] = i;
        alea.shuffle(indices,n);
        std::cerr << "...move to front" << std::endl;
        for(size_t i=0;i<n;++i)
        {
            dummy *d = dlist.fetch(i);
            dlist.move_to_front(d);
            Y_ASSERT(d==dlist.head);
        }

        for(size_t iter=1024;iter>0;--iter)
        {
            dummy *d = dlist.fetch( alea.lt(dlist.size) );
            dlist.move_to_front(d);
            Y_ASSERT(d==dlist.head);
        }
        
        std::cerr << "...insert after" <<std::endl;
        for(size_t iter=8;iter>0;--iter)
        {
            alea.shuffle(indices,n);
            for(size_t i=0;i<n;++i)
            {
                dummy *yours = dlist.unlink( dlist.fetch(i) );
                dummy *mine  = dlist.fetch( alea.lt( dlist.size ) );
                dlist.insert_after(mine,yours);
            }
        }

        std::cerr << "...insert before" <<std::endl;
        for(size_t iter=8;iter>0;--iter)
        {
            alea.shuffle(indices,n);
            for(size_t i=0;i<n;++i)
            {
                dummy *yours = dlist.unlink( dlist.fetch(i) );
                dummy *mine  = dlist.fetch( alea.lt( dlist.size ) );
                dlist.insert_before(mine,yours);
            }
        }

        std::cerr << "...replace" << std::endl;
        alea.shuffle(indices,n);
        for(size_t i=0;i<n;++i)
        {
            tmp.push_back( dlist.replace( dlist.fetch(i), tmp.pop_front() ) );
        }

        delete [] indices;
    }

    std::cerr << "...reverse" << std::endl;
    alea.shuffle(dlist);
    alea.shuffle(dclist);
    dlist2.release();
    dclist2.release();

    dlist2.merge_back_copy(dlist);   Y_ASSERT(dlist2.size==dlist.size);
    dclist2.merge_back_copy(dclist); Y_ASSERT(dclist2.size==dclist.size);

    dlist.reverse();
    for(const dummy *up=dlist.head,*dn=dlist2.tail;up;up=up->next,dn=dn->prev)
    {
        Y_ASSERT(up->data==dn->data);
    }
    dclist.reverse();
    for(const dummy *up=dclist.head,*dn=dclist2.tail;up;up=up->next,dn=dn->prev)
    {
        Y_ASSERT(up->data==dn->data);
    }


}
Y_UTEST_DONE()

