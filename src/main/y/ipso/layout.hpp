//! \file
#ifndef Y_IPSO_LAYOUT_INCLUDED
#define Y_IPSO_LAYOUT_INCLUDED 1

#include "y/ipso/types.hpp"

namespace upsylon
{
    namespace ipso
    {
        class layout
        {
        public:
            virtual ~layout() throw();                  //!< destructor
            explicit layout(const size_t dims) throw(); //!< initialize
            layout(const layout &) throw();             //!< copy
\
            const size_t dimensions;     //!< dimensions
            const size_t items;          //!< number of items in layout

        private:
            Y_DISABLE_ASSIGN(layout);
        };

        template <typename COORD>
        class layout_of : public layout
        {
        public:
            static const size_t DIMENSIONS = dim_of<COORD>::value;
            inline virtual ~layout_of() throw() {}
            inline explicit layout_of( const COORD lo, const COORD up) throw() :
            layout(DIMENSIONS),
            lower(lo),
            upper(up),
            width(lo)
            {
                size_t  &n = (size_t  &)items;
                coord1D *l = (coord1D *)&lower;
                coord1D *u = (coord1D *)&upper;
                coord1D *w = (coord1D *)&width;
                n=1;
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    if(l[dim]>u[dim]) cswap(l[dim],u[dim]);
                    n *= ( w[dim] = (u[dim]+1-l[dim]));
                }
            }

            const COORD lower;
            const COORD upper;
            const COORD width;

            inline friend std::ostream & operator<<(std::ostream &os, const layout_of &L )
            {
                os << '{' << L.lower << '-' << '>' << L.upper << ' ' << ':'  << ' ' << L.items << '}';
                return os;
            }

        private:
            Y_DISABLE_ASSIGN(layout_of);
        };

        typedef layout_of<coord1D> layout1D;
        typedef layout_of<coord2D> layout2D;
        typedef layout_of<coord3D> layout3D;


    }
}

#endif

