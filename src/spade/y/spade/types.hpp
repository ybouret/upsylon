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
        typedef unit_t           Coord1D;
        typedef point2d<Coord1D> Coord2D;
        typedef point3d<Coord1D> Coord3D;
        
        struct Coord
        {
            static const  Coord1D __Zero[4];
            static const  Coord1D __Ones[4];
            static size_t         DispWidth;
            
            template <typename COORD>
            struct Get
            {
                static const unsigned Dim = sizeof(COORD)/sizeof(Coord1D);
            };
            
            template <typename COORD> static inline
            const COORD & Zero() throw()
            {
                return *(const COORD *) &__Zero[0];
            }
            
            template <typename COORD> static inline
            const COORD & Ones() throw()
            {
                return *(const COORD *) &__Ones[0];
            }
            
            template <typename COORD> static inline
            Coord1D & Of(COORD &C, const unsigned dim) throw()
            {
                assert(dim< Get<COORD>::Dim );
                return  *(( (Coord1D *)&C ) + dim);
            }
            
            template <typename COORD> static inline
            const Coord1D & Of(const COORD &C, const unsigned dim) throw()
            {
                assert(dim< Get<COORD>::Dim );
                return  *(( (const Coord1D *)&C ) + dim);
            }
            
            static std::ostream & Disp(std::ostream &,const Coord1D *,const unsigned);
            
            template <typename COORD> static inline
            std::ostream & Disp(std::ostream &os, const COORD C)
            {
                return Disp(os, (const Coord1D *) &C, Get<COORD>::Dim);
            }
            
            //! [0:m]
            static Coord1D Natural1D(const Coord1D m, randomized::bits & ) throw();
            
            //! [-m:m]
            static Coord1D Integer1D(const Coord1D m, randomized::bits & ) throw();
            
            template <typename COORD>
            static inline COORD Natural( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim< Get<COORD>::Dim;++dim)
                {
                    Of(ans,dim) = Natural1D( Of(C,dim), ran);
                }
                return ans;
            }
            
            template <typename COORD>
            static inline COORD Integer( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim< Get<COORD>::Dim;++dim)
                {
                    Of(ans,dim) = Integer1D( Of(C,dim), ran);
                }
                return ans;
            }
            
        };
        
        
        
        
    }
    
}

#endif

