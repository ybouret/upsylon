
#include "y/yap/library.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"
#include <iostream>

namespace upsylon
{

    namespace yap
    {
        double natural:: to_double() const
        {
            double ans = 0;
            for(size_t i=words;i>0;)
            {
                ans *= word_radix;
                ans += word[--i];
            }
            return ans;
        }

        namespace {

            struct dnode
            {
                dnode     *next;
                dnode     *prev;
                double     value;
                static int compare(const dnode *lhs, const dnode *rhs, void *) throw()
                {
                    return comparison::decreasing(lhs->value,rhs->value);
                }
            };

            class dlist : public core::list_of<dnode>
            {
            public:
                inline explicit dlist() throw() {}
                inline virtual ~dlist() throw()
                {
                    while(size)
                    {
                        dnode *node = pop_back();
                        object::release1(node);
                    }
                }

                inline void add( const double value )
                {
                    push_back( object::acquire1<dnode>() )->value = value;
                }

                void sort()
                {
                    merging<dnode>::sort(*this, dnode::compare, 0);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(dlist);
            };

        }

        double natural:: ratio_of(const natural &num,const natural &den)
        {
            static const library &apl = library::instance();
            static const natural &ten = apl._10;
            natural q,r;
            //------------------------------------------------------------------
            // integer part
            //------------------------------------------------------------------
            natural::split(q, r, num, den);
            const double ip = q.to_double();

            //------------------------------------------------------------------
            // fractional part
            //------------------------------------------------------------------
            dlist        lst;
            {
                const size_t nd  = den.bits();
                double       fac = 1;
                for(size_t i=1;i<=nd;++i)
                {
                    const natural n = r*ten;
                    natural::split(q,r,n,den);
                    fac *= 0.1;
                    const double x = q.to_double();
                    lst.add( fac*x );
                }
            }
            assert( lst.size == den.bits() );
            double fp = 0;
            while(lst.size)
            {
                dnode *node = lst.pop_back();
                fp += node->value;
                object::release1(node);
            }
            return ip+fp;
        }

    }

}


