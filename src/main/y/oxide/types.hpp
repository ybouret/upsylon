//! \file
#ifndef Y_OXIDE_TYPES_INCLUDED
#define Y_OXIDE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/container/sequence.hpp"
#include "y/strfwd.hpp"

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
        
        struct Coord
        {
            template <typename COORD> struct Get
            {
                static const size_t Dimensions = sizeof(COORD)/sizeof(Coord1D); //!< the value
            };
            
            template <typename COORD> static inline
            Coord1D & Of( COORD &c, const size_t dim) throw()
            {
                assert(dim<Get<COORD>::Dimensions);
                return *( ((Coord1D *) &c) + dim );
            }
            
            //! get specific const coodinate
            template <typename COORD> static inline
            const Coord1D &Of( const COORD &c, const size_t dim ) throw()
            {
                assert(dim<Get<COORD>::Dimensions);
                return *( ((const Coord1D *) &c) + dim );
            }
            
            //! get product
            template <typename COORD> static inline
            Coord1D Product( const COORD &c ) throw()
            {
                const Coord1D *p = (const Coord1D*)&c;
                Coord1D ans = p[0];
                for(size_t i=1;i<Get<COORD>::Dimensions;++i)
                {
                    ans *= p[i];
                }
                return ans;
            }
            
            //! decrease all components
            template <typename COORD> static inline
            COORD & Decrease( COORD &c ) throw()
            {
                Coord1D *p = (Coord1D*)&c;
                for(size_t i=0;i<Get<COORD>::Dimensions;++i)
                {
                    --p[i];
                }
                return c;
            }
            
            //! return a decreased coordinate
            template <typename COORD> inline
            COORD  Decreased( const COORD &c ) throw()
            {
                COORD  d = c;
                return Decrease(d);
            }
            
            //! get a coordinate in [0..m]
            static Coord1D Natural1D( const unit_t m, randomized::bits &ran ) throw();
            
            //! get a coordinate in  [-m..m]
            static Coord1D Integer1D( const unit_t m, randomized::bits &ran ) throw();
            
            //! get a coordinate in [0..m]^DIM
            template <typename COORD> static inline
            COORD Natural( const unit_t m, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim )
                {
                    CoordOf(ans,dim) = Natural1D(m,ran);
                }
                return ans;
            }
            
            //! get a coordinate in [-m..m]^DIM
            template <typename COORD> static inline
            COORD Integer( const unit_t m, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim )
                {
                    Coord::Of(ans,dim) = Integer1D(m,ran);
                }
                return ans;
            }
            
            //! range(a,b)
            static Coord1D Within1D( const Coord1D a, const Coord1D b, randomized::bits &ran) throw();
            
            //! range(lo,up)
            template <typename COORD> static inline
            COORD Within( const COORD lo, const COORD up, randomized::bits &ran) throw()
            {
                COORD ans(0);
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim )
                {
                    Coord::Of(ans,dim) = Within1D( Coord::Of(lo,dim), Coord::Of(up,dim), ran);
                }
                return ans;
            }
            
            //! parse x[,y[,z]]
            static void Parse(Coord1D *p, const size_t n, const string &args);
            
            //! parse wrapper
            static void Parse(Coord1D *p, const size_t n, const char   *text);
            
            template <typename COORD> static inline
            COORD Parse( const string &args )
            {
                COORD ans(0);
                Parse((Coord1D *)&ans,Get<COORD>::Dimensions, args);
                return ans;
            }
            
            template <typename COORD> static inline
            COORD Parse( const char *args )
            {
                COORD ans(0);
                Parse((Coord1D *)&ans, Get<COORD>::Dimensions, args);
                return ans;
            }
        };
        
        
        
        
        
        
        
        
    }
    
    
}

#endif

