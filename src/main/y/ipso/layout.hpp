//! \file
#ifndef Y_IPSO_LAYOUT_INCLUDED
#define Y_IPSO_LAYOUT_INCLUDED 1

#include "y/ipso/types.hpp"

namespace upsylon
{
    namespace ipso
    {
        //! layout base class
        class layout
        {
        public:
            virtual ~layout() throw();                  //!< destructor
            explicit layout(const size_t dims) throw(); //!< initialize
            layout(const layout &) throw();             //!< copy

            const size_t dimensions;     //!< dimensions
            const size_t items;          //!< number of items in layout

        private:
            Y_DISABLE_ASSIGN(layout);
        };

        //! layout for a given coordinate
        template <typename COORD>
        class layout_of : public layout
        {
        public:
            static const size_t DIMENSIONS = dim_of<COORD>::value; //!< number of dimensions

            inline virtual ~layout_of() throw() {} //!< desctructor

            //! initialize
            inline explicit layout_of( const COORD lo, const COORD up) throw() :
            layout(DIMENSIONS),
            lower(lo),
            upper(up),
            width(lo),
            pitch(lo)
            {
                size_t  &n = (size_t  &)items;
                coord1D *l = (coord1D *)&lower;
                coord1D *u = (coord1D *)&upper;
                coord1D *w = (coord1D *)&width;
                coord1D *p = (coord1D *)&pitch;
                n=1;
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    if(l[dim]>u[dim]) cswap(l[dim],u[dim]);
                    p[dim] = n;
                    n *= ( w[dim] = (u[dim]+1-l[dim]));
                }
            }

            const COORD lower; //!< lower coordinate
            const COORD upper; //!< upper coordinate
            const COORD width; //!< width
            const COORD pitch; //!< 1 x xy

            //! test content
            inline bool has( const COORD q) const throw()
            {
                for(size_t i=0;i<DIMENSIONS;++i)
                {
                    const coord1D p = coord_of(q,i);
                    if(p<coord_of(lower,i)||p>coord_of(upper,i)) return false;
                }
                return true;
            }

            //! test containts sub layout
            inline bool contains(const layout_of &sub ) const throw()
            {
                return has(sub.lower) && has(sub.upper);
            }

            //! index of a coordinate
            inline coord1D index_of(const COORD q) const throw()
            {
                coord1D ans = coord_of(q,0)-coord_of(lower,0);
                for(size_t dim=1;dim<DIMENSIONS;++dim)
                {
                    ans += (coord_of(q,dim)-coord_of(lower,dim))*coord_of(pitch,dim);
                }
                return ans;
            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const layout_of &L )
            {
                os << '{' << L.lower << '-' << '>' << L.upper << ' ' << ':'  << ' ' << L.items << '=' << L.width << '}';
                return os;
            }

            //! test equality
            inline bool same_metrics_than( const layout_of &other ) const throw()
            {
                return (lower==other.lower)  && (upper==other.upper);
            }

        private:
            Y_DISABLE_ASSIGN(layout_of);
        };

        typedef layout_of<coord1D> layout1D; //!< alias
        typedef layout_of<coord2D> layout2D; //!< alias
        typedef layout_of<coord3D> layout3D; //!< alias


    }
}

#endif

