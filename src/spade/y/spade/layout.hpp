//! \file

#ifndef Y_SPADE_LAYOUT_INCLUDED
#define Y_SPADE_LAYOUT_INCLUDED 1

#include "y/spade/layout-metrics.hpp"
#include "y/counting/mloop.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {
    
    namespace Spade
    {
        template <typename COORD>
        class Layout : public LayoutMetrics
        {
        public:
            typedef typename type_traits<COORD>::mutable_type   coord;       //!< alias
            typedef const coord                                 const_coord; //!< alias
            typedef mloop<Coord1D,coord>                        Loop;        //!< loop over sub layout
                                                                             //! get dimensions
            static const size_t                                 Dimensions = Coord::Get<COORD>::Dimensions;
            inline virtual ~Layout() throw()
            {
                _bzset(lower);
                _bzset(upper);
                _bzset(width);
                _bzset(pitch);
            }
            
            explicit Layout(const_coord lo, const_coord up) :
            LayoutMetrics(Dimensions),
            lower(lo),
            upper(up),
            width(Coord::Zero<COORD>()),
            pitch(Coord::Zero<COORD>())
            {
               
                Coord1D *w = & Coord::Of( aliasing::_(width), 0);
                {
                    Coord1D *l = & Coord::Of( aliasing::_(lower), 0);
                    Coord1D *u = & Coord::Of( aliasing::_(upper), 0);
                    for(unsigned dim=0;dim<Dimensions;++dim)
                    {
                        Coord1D &L = l[dim];
                        Coord1D &U = u[dim];
                        if(U<L) cswap(L,U);
                        w[dim] = (U-L) + 1;
                    }
                }
                
                Coord1D *p = & Coord::Of( aliasing::_(pitch), 0);
                size_t  &n = aliasing::_(items); assert(0==items);
                p[0] = 1;
                n    = w[0];
                for(unsigned dim=1,prv=0;dim<Dimensions;++dim,++prv)
                {
                    n *= w[dim];
                    p[dim] = p[prv] * w[prv];
                }
            }
            
            const_coord lower; //!< lower coordinate
            const_coord upper; //!< upper coordinate
            const_coord width; //!< witdh in each dimension
            const_coord pitch; //!< pitch 1, nx, nx*ny to compute indices
            
        private:
            Y_DISABLE_ASSIGN(Layout);
            
        };
    }
}

#endif

