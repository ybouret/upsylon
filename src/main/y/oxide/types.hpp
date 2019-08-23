//! \file
#ifndef Y_OXIDE_TYPES_INCLUDED
#define Y_OXIDE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon
{
    namespace randomized
    {
        class bits; //!< forward declaration
    }

    namespace Oxide
    {
        typedef unit_t           Coord1D;
        typedef point2d<Coord1D> Coord2D;
        typedef point3d<Coord1D> Coord3D;

        template <typename COORD> struct DimensionsOf
        {
            static const size_t Value = sizeof(COORD)/sizeof(Coord1D);
        };

        template <typename COORD> inline
        Coord1D &CoordOf( COORD &c, const size_t dim ) throw()
        {
            assert(dim<DimensionsOf<COORD>::Value);
            return *( ((Coord1D *) &c) + dim );
        }

        template <typename COORD> inline
        const Coord1D &CoordOf( const COORD &c, const size_t dim ) throw()
        {
            assert(dim<DimensionsOf<COORD>::Value);
            return *( ((const Coord1D *) &c) + dim );
        }

        struct CoordOps
        {
            static Coord1D GetNatural( const unit_t, randomized::bits &ran ) throw();
            static Coord1D GetInteger( const unit_t, randomized::bits &ran ) throw();
            template <typename COORD> static inline
            COORD Natural( const unit_t m, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim )
                {
                    CoordOf(ans,dim) = GetNatural(m,ran);
                }
                return ans;
            }

            template <typename COORD> static inline
            COORD Integer( const unit_t m, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim )
                {
                    CoordOf(ans,dim) = GetInteger(m,ran);
                }
                return ans;
            }

            //! range(a,b)
            static Coord1D GetWithin( const Coord1D a, const Coord1D b, randomized::bits &ran) throw();

            template <typename COORD> static inline
            COORD Within( const COORD lo, const COORD up, randomized::bits &ran) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<DimensionsOf<COORD>::Value;++dim )
                {
                    CoordOf(ans,dim) = GetWithin( CoordOf(lo,dim), CoordOf(up,dim), ran);
                }
                return ans;
            }

        };

    }
}

#endif

