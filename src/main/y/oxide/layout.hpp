//! \file
#ifndef Y_OXIDE_LAYOUT_INCLUDED
#define Y_OXIDE_LAYOUT_INCLUDED 1

#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        class LayoutInfo
        {
        public:
            const size_t dimension;   //!< [1|2|3]
            const size_t items;       //!< linear items

            virtual ~LayoutInfo() throw();

        protected:
            explicit LayoutInfo(const size_t d ) throw();
            explicit LayoutInfo( const LayoutInfo &other) throw();

        private:
            Y_DISABLE_ASSIGN(LayoutInfo);
        };

        template <typename COORD>
        class Layout : public LayoutInfo
        {
        public:
            static const size_t Dimensions = DimensionsOf<COORD>::Value;
            typedef COORD       coord;
            typedef const coord const_coord;

            const_coord lower;
            const_coord upper;
            const_coord width;
            const_coord pitch;
            
            inline virtual ~Layout() throw() {}

            inline explicit Layout(const_coord lo,
                                   const coord up) throw() :
            LayoutInfo(Dimensions),
            lower(lo),
            upper(up),
            width(up),
            pitch(up)
            {
                size_t  &n = (size_t  &)items;
                Coord1D *l = (Coord1D *)&lower;
                Coord1D *u = (Coord1D *)&upper;
                Coord1D *w = (Coord1D *)&width;
                Coord1D *p = (Coord1D *)&pitch;
                n=1;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    if(l[dim]>u[dim]) cswap(l[dim],u[dim]);
                    p[dim] = n;
                    n *= ( w[dim] = (u[dim]+1-l[dim]));
                }
            }

            inline Layout( const Layout &other ) throw() :
            LayoutInfo(other),
            lower( other.lower ),
            upper( other.upper ),
            width( other.width ),
            pitch( other.pitch )
            {
            }

            friend inline std::ostream & operator<<( std::ostream &os, const Layout &L )
            {
                os << "{" << L.lower << "->" << L.upper << " : #" << L.width << "=" << L.items << "}";
                return os;
            }

            inline bool has( const_coord C ) const throw()
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D l = CoordOf(lower,dim);
                    const Coord1D u = CoordOf(upper,dim);
                    const Coord1D c = CoordOf(C,dim);
                    if(c<l||c>u) return false;
                }
                return true;
            }

            inline bool contains( const Layout &sub ) const throw()
            {
                return has( sub.lower ) && has( sub.upper );
            }


            //! index of a coordinate
            inline Coord1D indexOf(const COORD q) const throw()
            {
                Coord1D ans = CoordOf(q,0)-CoordOf(lower,0);
                for(size_t dim=1;dim<Dimensions;++dim)
                {
                    ans += (CoordOf(q,dim)-CoordOf(lower,dim))*CoordOf(pitch,dim);
                }
                return ans;
            }

            inline COORD rand( randomized::bits &ran ) const throw()
            {
                return CoordOps::Within(lower,upper,ran);
            }

        private:
            Y_DISABLE_ASSIGN(Layout);
        };


        typedef Layout<Coord1D> Layout1D;
        typedef Layout<Coord2D> Layout2D;
        typedef Layout<Coord3D> Layout3D;
        
    }
}

#endif

