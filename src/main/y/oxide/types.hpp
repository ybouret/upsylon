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
        typedef unit_t           Coord1D; //!< 1D coordinate
        typedef point2d<Coord1D> Coord2D; //!< 2D coordinate
        typedef point3d<Coord1D> Coord3D; //!< 3D coordinate

        //! extract dimensions
        template <typename COORD> struct DimensionsOf
        {
            static const size_t Value = sizeof(COORD)/sizeof(Coord1D); //!< the value
        };

        //! get specific coordinate
        template <typename COORD> inline
        Coord1D &CoordOf( COORD &c, const size_t dim ) throw()
        {
            assert(dim<DimensionsOf<COORD>::Value);
            return *( ((Coord1D *) &c) + dim );
        }

        //! get specific const coodinate
        template <typename COORD> inline
        const Coord1D &CoordOf( const COORD &c, const size_t dim ) throw()
        {
            assert(dim<DimensionsOf<COORD>::Value);
            return *( ((const Coord1D *) &c) + dim );
        }

        //! operations on coordinates
        struct CoordOps
        {
            //! get a coordinate in [0..m]
            static Coord1D GetNatural( const unit_t m, randomized::bits &ran ) throw();
            //! get a coordinate in  [-m..m]
            static Coord1D GetInteger( const unit_t m, randomized::bits &ran ) throw();

            //! get a coordinate in [0..m]^DIM
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

            //! get a coordinate in [-m..m]^DIM
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

            //! range(lo,up)
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

