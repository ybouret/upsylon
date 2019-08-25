//! \file
#ifndef Y_OXIDE_LAYOUT_INCLUDED
#define Y_OXIDE_LAYOUT_INCLUDED 1

#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! common data for layout
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

        //! a layout is a metrics based on a coordinate
        template <typename COORD>
        class Layout : public LayoutInfo
        {
        public:
            static const size_t Dimensions = DimensionsOf<COORD>::Value;
            typedef COORD       coord;
            typedef const coord const_coord;

            const_coord lower; //!< lower coordinate
            const_coord upper; //!< upper coordinate
            const_coord width; //!< witdh in each dimension
            const_coord pitch; //!< pitch 1, nx, nx*ny to compute indices

            //! cleanup
            inline virtual ~Layout() throw() {}

            //! setup by two coordinates
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

            //! copy
            inline Layout( const Layout &other ) throw() :
            LayoutInfo(other),
            lower( other.lower ),
            upper( other.upper ),
            width( other.width ),
            pitch( other.pitch )
            {
            }

            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const Layout &L )
            {
                os << "{" << L.lower << "->" << L.upper << " : #" << L.width << "=" << L.items << "}";
                return os;
            }

            //! test if a coordinate is inside
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

            //! test if a sub-layout is contained
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

            //! coordinate of index
            inline COORD coordOf(const Coord1D idx) const throw()
            {
                return COORD(0);
            }

            //! random coordinate within the layout
            inline COORD rand( randomized::bits &ran ) const throw()
            {
                return CoordOps::Within(lower,upper,ran);
            }

        private:
            Y_DISABLE_ASSIGN(Layout);
        };


        typedef Layout<Coord1D> Layout1D; //!< 1D layout
        typedef Layout<Coord2D> Layout2D; //!< 2D layout
        typedef Layout<Coord3D> Layout3D; //!< 3D layout
        
    }
}

#endif

