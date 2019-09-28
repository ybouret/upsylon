#include "y/core/clist.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/core/loop.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace  {
    class aNode
    {
    public:
        int    data;
        aNode *next;
        aNode *prev;
        static int count;
        
        aNode() throw() : data( alea.full<int>() ), next(0), prev(0) { ++count; }
        ~aNode() throw() { --count; }
        
        aNode( const aNode &other ) throw() : data( other.data ), next(0), prev(0) { ++count; }
        
        inline aNode * clone() const { return new aNode( *this ); }
        
        
    private:
        Y_DISABLE_ASSIGN(aNode);
    };
    
    int aNode::count = 0;
    
    template <typename LIST>
    void do_list_test( LIST &l )
    {
        std::cerr << "[";
        const size_t n = 10 + alea.leq(1000);
        for(size_t i=0;i<n;++i)
        {
            aNode *node = new aNode();
            if( alea.choice() )
            {
                std::cerr << ">";
                l.push_back(node);
            }
            else
            {
                std::cerr << "<";
                l.push_front(node);
            }
        }
        std::cerr << "]" << std::endl;
        Y_CHECK(l.size==n);
        {
            LIST l_copy(l);
            Y_CHECK(l.size==l_copy.size);
            l.merge_back(l_copy);
        }
        {
            LIST l_copy(l);
            Y_CHECK(l.size==l_copy.size);
            l.merge_front(l_copy);
        }
        
        
        
        core::pool_of_cpp<aNode> p;
        while( l.size ) p.store( l.pop_back() );
        
        std::cerr << "MoveToFront 1->" << n << std::endl;
        for(size_t i=0;i<n;++i)
        {
            l.push_back( new aNode() );
            for(size_t j=0;j<l.size;++j)
            {
                aNode *node = l.fetch(j);
                if(l.index_of(node)!=j)
                {
                    throw exception("invalid fetch/index");
                }
                l.move_to_front( l.fetch(j) );
            }
        }
        
        std::cerr << "l.size=" << l.size << std::endl;
        std::cerr << "Replacing..." << std::endl;
        for(size_t i=10+alea.leq(90);i>0;--i)
        {
            aNode *node = new aNode();
            aNode *scan = l.fetch( alea.lt(l.size) );
            l.replace(scan,node);
            Y_ASSERT(scan->next==0);
            Y_ASSERT(scan->prev==0);
            delete scan;
        }
        
        std::cerr << "Reversing..." << std::endl;
        {
            const size_t old_size = l.size;
            l.reverse();
            Y_CHECK(old_size==l.size);
            l.reverse_last( l.size / 8 );
            Y_CHECK(old_size==l.size);
        }
        
        std::cerr << "Inserting..." << std::endl;
        for(size_t i=10+alea.leq(90);i>0;--i)
        {
            l.insert_after( l.fetch( alea.lt(l.size) ), new aNode() );
            l.insert_before( l.fetch( alea.lt(l.size) ), new aNode() );
        }
        
        
    }
    
    
    template <typename CLIST>
    void do_clist_test( CLIST &l )
    {
        std::cerr << "[";
        const size_t n = 10 + alea.leq(1000);
        for(size_t i=0;i<n;++i)
        {
            aNode *node = new aNode();
            if( alea.choice() )
            {
                std::cerr << ">";
                l.push_back(node);
            }
            else
            {
                std::cerr << "<";
                l.push_front(node);
            }
        }
        std::cerr << "]" << std::endl;
        std::cerr << "clist.size=" << l.size << std::endl;

        {
            CLIST tmp = l;
            std::cerr << "ctemp.size=" << tmp.size << std::endl;
        }

        while( l.size )
        {
            l.scroll_forward( alea.leq(1000) );
            l.scroll_reverse( alea.leq(1000) );
            if( alea.choice() )
            {
                delete l.pop_back();
            }
            else
            {
                delete l.pop_front();
            }
        }
        
    }
    
    static inline
    void do_loop_test()
    {
        int a[256];
        
        for(size_t iter=0;iter<10;++iter)
        {
            const size_t n = 1+alea.leq(1000);
            std::cerr << "...loop " << n << std::endl;
            size_t count=0;
            Y_LOOP(0,++count);
            Y_LOOP(n,++count);
            Y_CHECK(n==count);
            count=0;
            Y_LOOP_(n,++count);
            Y_CHECK(n==count);
#define THE_CODE(IDX) a[IDX] = alea.full<int>()
            Y_LOOP_FUNC( sizeof(a)/sizeof(a[0]),THE_CODE,0);
            Y_LOOP_FUNC_(sizeof(a)/sizeof(a[0]),THE_CODE,0);
            Y_LOOP_FUNC( sizeof(a)/sizeof(a[0])-1,THE_CODE,1);
            Y_LOOP_FUNC_(sizeof(a)/sizeof(a[0])-1,THE_CODE,1);
        }
    }
}



Y_UTEST(core)
{
    std::cerr << "---- Test Loop ----" << std::endl;
    do_loop_test();
    
    std::cerr << "---- Test List ----" << std::endl;
    {
        core::list_of_cpp<aNode> aList;
        do_list_test(aList);
        core::list_of_cloneable<aNode> bList;
        do_list_test(bList);
    }
    Y_CHECK(0==aNode::count);
    
    std::cerr << "---- Test cList ----" << std::endl;
    
    std::cerr << "sizeof(core::pool_of)  =" << sizeof(core::pool_of<aNode>) << std::endl;
    std::cerr << "sizeof(core::list_of)  =" << sizeof(core::list_of<aNode>) << std::endl;
    std::cerr << "sizeof(core::clist_of) =" << sizeof(core::clist_of<aNode>) << std::endl;
    {
        core::clist_of_cpp<aNode> cList;
        do_clist_test(cList);
    }
    std::cerr << "#aNode=" << aNode::count << std::endl;
    
}
Y_UTEST_DONE()

