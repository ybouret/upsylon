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
        //----------------------------------------------------------------------
        //
        //! a multi-dimensional layout
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Layout : public LayoutMetrics
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef typename type_traits<COORD>::mutable_type   coord;       //!< alias
            typedef const coord                                 const_coord; //!< alias
            typedef mloop<Coord1D,coord>                        Loop;        //!< loop over sub layout
            static const size_t                                 Dimensions = Coord::Get<COORD>::Dimensions;

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Layout() throw()
            {
                _bzset(lower);
                _bzset(upper);
                _bzset(width);
                _bzset(pitch);
            }
            
            //! setup
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
            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const Layout &L )
            {
                os << "{";
                Coord::Disp(os,L.lower) << "->";
                Coord::Disp(os,L.upper) << " : #";
                Coord::Disp(os,L.width) << "=";
                const Coord1D n = L.items;
                Coord::Disp(os,n) << "}";
                return os;
            }
            
            //------------------------------------------------------------------
            //
            // methods for algebra
            //
            //------------------------------------------------------------------
            
            //! compare definitions
            inline bool is_same_than( const Layout &other ) const throw()
            {
                if( lower == other.lower && upper==other.upper )
                {
                    assert(width==other.width);
                    assert(pitch==other.pitch);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            
            
            //! test if a coordinate is inside
            inline bool has( const_coord C ) const throw()
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D l = Coord::Of(lower,dim);
                    const Coord1D u = Coord::Of(upper,dim);
                    const Coord1D c = Coord::Of(C,dim);
                    if(c<l||c>u) return false;
                }
                return true;
            }
            
            //! test if a sub-layout is contained
            inline bool contains( const Layout &sub ) const throw()
            {
                return has( sub.lower ) && has( sub.upper );
            }
            
            //! random coordinate within the layout
            inline COORD rand( randomized::bits &ran ) const throw()
            {
                return Coord::Within(lower,upper,ran);
            }
            
            //------------------------------------------------------------------
            //
            // index<->coordinate conversion
            //
            //------------------------------------------------------------------
            
            //! index of a coordinate
            inline Coord1D indexOf(const_coord q) const throw()
            {
                assert( has(q) );
                Coord1D ans = Coord::Of(q,0)-Coord::Of(lower,0);
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    ans += (Coord::Of(q,dim)-Coord::Of(lower,dim))*Coord::Of(pitch,dim);
                }
                assert(ans>=0);
                assert(ans<Coord1D(items));
                return ans;
            }
            
            //! coordinate of index in [0..items-1]
            inline coord coordOf(const Coord1D idx) const throw()
            {
                assert(idx>=0);
                assert(idx<Coord1D(items));
                coord   q(0);
                Coord1D rem = idx;
                for(unsigned dim=Dimensions-1;dim>0;--dim)
                {
                    const Coord1D den = Coord::Of(pitch,dim);
                    const Coord1D qot = rem / den;
                    Coord::Of(q,dim) = qot + Coord::Of(lower,dim);
                    rem -= qot * den;
                }
                Coord::Of(q,0) = rem + Coord::Of(lower,0);
                return q;
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

