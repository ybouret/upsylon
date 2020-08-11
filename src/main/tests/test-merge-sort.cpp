
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/utest/run.hpp"
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



    template <typename NODE>
    struct merging {

        typedef core::list_of<NODE> list_base;

        class list_type : public object, public list_base
        {
        public:
            list_type *next;

            inline explicit list_type() throw() : list_base(), next(0) {}
            inline virtual ~list_type() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(list_type);
        };

        typedef int (*compare)(const NODE *lhs, const NODE *rhs, void *);

        template <typename LIST> static inline
        void display(const LIST &L)
        {
            std::cerr << "{";
            for(const NODE *node=L.head;node;node=node->next)
            {
                std::cerr << " " << *node;
            }
            std::cerr << " }" <<std::endl;
        }

        template <typename LIST> static inline
        void sort( LIST &L, compare proc, void *args)
        {
            core::pool_of<list_type> raw;
            core::pool_of<list_type> srt;

            std::cerr << "data: "; display(L);

            // initialize
            {
                list_type *sub = raw.store( new list_type() );
                 while( L.size )
                {
                    sub->push_back( L.pop_front() );
                }
                std::cerr << "ini: "; display( *sub );
            }

            //loop
            while(raw.size)
            {
                list_type &top = *raw.head;

                switch(top.size)
                {
                    case 1:
                        return;

                    default:
                    {
                        size_t        n  = top.size>>1;
                        list_type   *sub = raw.store( new list_type() );
                        while(n-- > 0 )
                        {
                            sub->push_back(top.pop_front());
                        }
                        std::cerr << "sub: "; display(*sub);
                    }
                }
            }


            raw.reset();

        }

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

    merging<dummy>::sort(D,NULL,NULL);
}
Y_UTEST_DONE()

