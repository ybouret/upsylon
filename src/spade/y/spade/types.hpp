//! \file

#ifndef Y_SPADE_TYPES_INCLUDED
#define Y_SPADE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon {
    
    namespace randomized
    {
        class bits; //!< forward declaration
    }
    
    namespace Spade
    {
        typedef unit_t           Coord1D; //!< 1D coordinate
        typedef point2d<Coord1D> Coord2D; //!< 2D coordinates
        typedef point3d<Coord1D> Coord3D; //!< 3D coordinates
        
        //! operations on coordinates
        struct Coord
        {
            static const  Coord1D __Zero[4]; //!< binary mapping of [0[,0[,0]]]
            static const  Coord1D __Ones[4]; //!< binary mapping of [1[,1[,1]]]
            static size_t         DispWidth; //!< display width per dimension
            
            //! get parameters...
            template <typename COORD>
            struct Get
            {
                static const unsigned Dimensions = sizeof(COORD)/sizeof(Coord1D); //!< Dimension(s)
            };
            
            //! map to zero
            template <typename COORD> static inline
            const COORD & Zero() throw()
            {
                return *(const COORD *) &__Zero[0];
            }
            
            //! map to ones
            template <typename COORD> static inline
            const COORD & Ones() throw()
            {
                return *(const COORD *) &__Ones[0];
            }
            
            //! get coordinate[dim]
            template <typename COORD> static inline
            Coord1D & Of(COORD &C, const unsigned dim) throw()
            {
                assert(dim< Get<COORD>::Dimensions );
                return  *(( (Coord1D *)&C ) + dim);
            }
            
            //! get coordinate[dim], const
            template <typename COORD> static inline
            const Coord1D & Of(const COORD &C, const unsigned dim) throw()
            {
                assert(dim<Get<COORD>::Dimensions);
                return  *(( (const Coord1D *)&C ) + dim);
            }
            
            //! display some fixed-width coordinates
            static std::ostream & Disp(std::ostream &,const Coord1D *,const unsigned);
            
            //! display a coordinate with fixed-width
            template <typename COORD> static inline
            std::ostream & Disp(std::ostream &os, const COORD C)
            {
                return Disp(os, (const Coord1D *) &C, Get<COORD>::Dimensions);
            }
            
            //! random in [0:m]
            static Coord1D Natural1D(const Coord1D m, randomized::bits & ) throw();
            
            //! random in [-m:m]
            static Coord1D Integer1D(const Coord1D m, randomized::bits & ) throw();
            
            //! random in [0:C]
            template <typename COORD>
            static inline COORD Natural( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim< Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Natural1D( Of(C,dim), ran);
                }
                return ans;
            }
            
            //! random in [-C:C]
            template <typename COORD>
            static inline COORD Integer( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Integer1D( Of(C,dim), ran);
                }
                return ans;
            }
            
            
        };
        
    }
    
}

#endif

