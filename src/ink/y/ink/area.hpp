//! \file
#ifndef Y_INK_AREA_INCLUDED
#define Y_INK_AREA_INCLUDED 1

#include "y/ink/types.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace ink
    {
       

        //! how to specify an area
        enum area_build_type
        {
            area_upper,
            area_sizes
        };

        //! area
        class area : public object
        {
        public:
            static const size_t DIMENSION = 2; //!< physical dimension

            const coord  lower;  //!< lower coordinate
            const coord  upper;  //!< upper coordinate
            const coord  sizes;  //!< upper-lower+1m or zero if invalid
            const unit_t pixels; //!< w*h

            //! default area, origin
            inline area() throw() : lower(0,0), upper(0,0), sizes(1,1), pixels(1)
            {
            }

            //! copy
            inline area(const area &other) throw() :
            lower(other.lower), upper(other.upper), sizes(other.sizes), pixels(other.pixels) {}

            //! destructor
            inline ~area() throw() {}

            //! build by upper or by sizes
            /**
             it is then possible to manage an empty area
             */
            inline area(const coord           l,
                        const coord           p,
                        const area_build_type t = area_upper) throw() :
            lower(l),
            upper(p),
            sizes(p),
            pixels(0)
            {
                switch(t)
                {
                    case area_upper:
                        setup_by_upper();
                        break;

                    case area_sizes:
                        setup_by_sizes();
                        break;
                }
                (unit_t &)pixels = sizes.x * sizes.y;
            }

            //! test if coordinate is insinde
            inline bool has( const coord c ) const throw()
            {
                return (c.x>=lower.x) && (c.y>=lower.y) && (c.x<=upper.x) && (c.y<=upper.y);
            }

            //! test if contains a sub-area
            inline bool contains(const area &sub) const throw()
            {
                return has(sub.lower) && has(sub.upper);
            }

            //! output info
            inline friend std::ostream & operator<<( std::ostream &os, const area &a )
            {
                os << '{' << a.lower << "->" << a.upper << " : " << a.sizes << " #" << a.pixels << '}';
                return os;
            }
        private:
            Y_DISABLE_ASSIGN(area);
            inline void setup_by_upper() throw()
            {
                unit_t *l = (unit_t *)&lower;
                unit_t *u = (unit_t *)&upper;
                unit_t *w = (unit_t *)&sizes;
                for(size_t dim=0;dim<DIMENSION;++dim)
                {
                    if( l[dim]>u[dim] ) { cswap(l[dim],u[dim]); }
                    w[dim] = u[dim]+1-l[dim];
                }
            }

            inline void setup_by_sizes() throw()
            {
                unit_t *l = (unit_t *)&lower;
                unit_t *u = (unit_t *)&upper;
                unit_t *w = (unit_t *)&sizes;
                for(size_t dim=0;dim<DIMENSION;++dim)
                {
                    u[dim] = l[dim] + (abs_of(w[dim])-1);
                }
            }

        };

        //! sequence of areas base type
        typedef vector<area> __areas;

        //! areas for tiling
        class areas : public __areas
        {
        public:
            //! destructor
            inline virtual ~areas() throw() {}

            //! copy
            inline          areas(const areas &other) : dynamic(), __areas( other ) {}

            //! tile
            areas( const area &source, const size_t n);
            
            
        private:
            Y_DISABLE_ASSIGN(areas);
        };
    }
}

#endif

