//! \file
#ifndef Y_INK_AREA_INCLUDED
#define Y_INK_AREA_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace ink
    {
        //! coordinate
        typedef point2d<unit_t> coord;
        
        //! area
        class area : public object
        {
        public:
            
            const coord lower; //!< lower coordinate
            const coord upper; //!< upper coordinate
            const coord sizes; //!< upper-lower+1
            
            inline area() throw() : lower(0,0), upper(0,0), sizes(1,1)
            {
            }
            
            inline area(const area &other) throw() :
            lower(other.lower), upper(other.upper), sizes(other.sizes) {}
            
            inline ~area() throw() {}
            
            inline area( const coord l, const coord u) throw() :
            lower(l),
            upper(u),
            sizes()
            {
                setup();
            }
            
        private:
            Y_DISABLE_ASSIGN(area);
            inline void setup() throw()
            {
                unit_t *l = (unit_t *)&lower;
                unit_t *u = (unit_t *)&upper;
                unit_t *w = (unit_t *)&sizes;
                for(size_t dim=0;dim<2;++dim)
                {
                    if( l[dim]>u[dim] ) { cswap(l[dim],u[dim]); }
                    w[dim] = u[dim]+1-l[dim];
                }
            }
        };
        
        typedef vector<area> __areas;

        class areas : public __areas
        {
        public:
            inline virtual ~areas() throw() {}
            inline          areas(const areas &other) : __areas( other ) {}
            inline areas( const area &source, const size_t n) : __areas(n,as_capacity)
            {
                if(n>0)
                {
                    
                }
            }
            
        private:
            Y_DISABLE_ASSIGN(areas);
        };
    }
}

#endif

