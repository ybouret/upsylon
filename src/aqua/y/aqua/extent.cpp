

#include "y/aqua/extent.hpp"
#include "y/sort/heap.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Aqua
    {

        std::ostream & operator<<(std::ostream &os, const Extent &x)
        {
            if(x.limited)
            {
                if(x.maximum>0)
                {
                    os << "limited@" << x.maximum;
                }
                else
                {
                    os << "blocked";
                }
            }
            else
            {
                os << "unlimited";
            }
            return os;
        }
        
        Extent:: ~Extent() throw()
        {
        }

        Extent:: Extent(const Extent &_) throw() :
        limited(_.limited),
        maximum(_.maximum),
        blocked(_.blocked)
        {
        }


        static inline
        double findMaximumExtent(const accessible<double> &C,
                                 const Component::List    &L,
                                 double                   *A) throw()
        {
            size_t num=0;
            for(const Component *r=L.head;r;r=r->next)
            {
                const size_t j  = r->sp.indx;
                const double Cj = max_of(C[j],0.0);
                const int    nu = r->nu; assert(nu>0);
                A[num++] = Cj/nu;
            }
            hsort(A,num,comparison::increasing<double>);
            return *A;
        }


        Extent:: Extent(const accessible<double> &C,
                        const Component::List    &L,
                        double                   *A) throw() :
        limited(L.size>0),
        maximum(limited ? findMaximumExtent(C,L,A) : 0),
        blocked(limited&&(maximum<=0))
        {
            
        }

        double Extent:: cut( const double value ) const throw()
        {
            assert(value>=0);
            if(limited)
            {
                return min_of(maximum,value);
            }
            else
            {
                return value;
            }
        }

    }

}



