//! \file

#ifndef Y_SPADE_LAYOUT_INCLUDED
#define Y_SPADE_LAYOUT_INCLUDED 1

#include "y/spade/layout-metrics.hpp"
#include "y/counting/mloop.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/cswap.hpp"
#include "y/sort/unique.hpp"

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
            typedef typename type_traits<COORD>::mutable_type   coord;         //!< alias
            typedef const coord                                 const_coord;   //!< alias
            typedef mloop<Coord1D,coord>                        Loop;          //!< loop over sub layout
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;  //!< Dimension(s)
            
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
            inline explicit Layout(const_coord lo, const_coord up) :
            LayoutMetrics(Dimensions),
            lower(lo),
            upper(up),
            width(Coord::Zero<COORD>()),
            pitch(Coord::Ones<COORD>())
            {
                
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    Coord1D &w = aliasing::_( Coord::Of(width,dim) );
                    Coord1D &l = aliasing::_( Coord::Of(lower,dim) );
                    Coord1D &u = aliasing::_( Coord::Of(upper,dim) );
                    if(u<l) cswap(l,u);
                    w=1+u-l;
                }
                
                aliasing::_(items) = Coord::Of(width,0);
                for(unsigned dim=1,prv=0;dim<Dimensions;++dim,++prv)
                {
                    aliasing::_(items) *= Coord::Of(width,dim);
                    aliasing::_( Coord::Of(pitch,dim) ) =
                    Coord::Of(pitch,prv) * Coord::Of(width,prv);
                }
                
            }
            
            //! copy
            inline Layout(const Layout &L) throw() :
            LayoutMetrics(L),
            lower(L.lower),
            upper(L.upper),
            width(L.width),
            pitch(L.pitch)
            {
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
            
            //! compare dimensions
            inline bool isSameThan( const Layout &other ) const throw()
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
            inline size_t indexOf(const_coord q) const throw()
            {
                assert( has(q) );
                size_t ans = static_cast<size_t>(Coord::Of(q,0)-Coord::Of(lower,0));
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    ans += static_cast<size_t>( (Coord::Of(q,dim)-Coord::Of(lower,dim))*Coord::Of(pitch,dim) );
                }
                assert(ans<items);
                return ans;
            }
            
            //! coordinate of index in [0..items-1]
            inline coord coordOf(const size_t idx) const throw()
            {
                assert(idx<items);
                coord   q(0);
                Coord1D rem = Coord1D(idx);
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
            
            //------------------------------------------------------------------
            //
            // other methods
            //
            //------------------------------------------------------------------
            
            //! gatherings indices of a sub layout
            template <typename SEQUENCE> inline
            void collect(SEQUENCE &indices, const Layout &sub) const
            {
                assert(this->contains(sub));
                Loop       loop(sub.lower,sub.upper); assert(sub.items==loop.count);
                indices.ensure( indices.size() + loop.count );
                for(loop.boot();loop.good();loop.next())
                {
                    indices.push_back( indexOf(loop.value) );
                }
                unique(indices);
            }
            
            
            //! split this according to local ranks among sizes
            Layout split(const_coord sizes,
                         const_coord ranks) const
            {
                coord length = width;
                coord offset = lower;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    Split1D(Coord::Of(length,dim),
                            Coord::Of(offset,dim ),
                            Coord::Of(sizes,dim),
                            Coord::Of(ranks,dim),
                            dim);
                    
                }
                coord up = length + offset;
                up -= Coord::Ones<COORD>();
                return Layout(offset,up);
            }
            
            //! find possible mappings
            template <typename SEQUENCE>
            inline size_t findMappings(SEQUENCE    &mappings,
                                       const size_t cores) const
            {
                assert(cores>0);
                //______________________________________________________________
                //
                // prepare loop
                //______________________________________________________________
                const Coord1D target = static_cast<Coord1D>(cores);
                Loop          loop( Coord::Ones<coord>(), Coord::Ld<coord>(target));
                const_coord  &sizes = loop.value;
                mappings.free();
                
                //______________________________________________________________
                //
                // loop over theoretical mappings
                //______________________________________________________________
                for(loop.boot();loop.good();loop.next())
                {
                    // may match ?
                    if( target != Coord::Product(sizes) )
                    {
                        continue; //! no match
                    }
                    // check possibility per dimension
                    bool possible = true;
                    for(unsigned dim=0;dim<Dimensions;++dim)
                    {
                        if( Coord::Of(sizes,dim) > Coord::Of(width,dim) )
                        {
                            possible = false; //! cannot split in that dimension
                            break;
                        }
                    }
                    if(!possible) continue;
                    mappings.push_back( sizes );
                }
                return mappings.size();
            }
            
        private:
            Y_DISABLE_ASSIGN(Layout);
            
        };
      
        typedef Layout<Coord1D> Layout1D; //!< 1D Layout
        typedef Layout<Coord2D> Layout2D; //!< 2D Layout
        typedef Layout<Coord3D> Layout3D; //!< 3D Layout

    }
}

#endif

