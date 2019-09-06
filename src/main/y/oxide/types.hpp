//! \file
#ifndef Y_OXIDE_TYPES_INCLUDED
#define Y_OXIDE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/container/sequence.hpp"
#include "y/strfwd.hpp"
#include "y/type/bzset.hpp"

#include <cstring>
#include <cstdlib>

namespace upsylon
{
    namespace randomized
    {
        class bits; //!< forward declaration
    }
    
    namespace Oxide
    {
        typedef unit_t           Coord1D; //!< 1D coordinate
        typedef point2d<Coord1D> Coord2D; //!< 2D coordinates
        typedef point3d<Coord1D> Coord3D; //!< 3D coordinate3

        typedef bool             Bool1D; //!< 1D boolean
        typedef point2d<bool>    Bool2D; //!< 2D booleans
        typedef point3d<bool>    Bool3D; //!< 3D booleans

        //! Boolean vector companion
        template <typename T> struct Boolean;
        //! in 1D
        template <> struct Boolean<Coord1D> { typedef Bool1D Type; //!< 1D bool
        };
        //! in 2D
        template <> struct Boolean<Coord2D> { typedef Bool2D Type; //!< 2D bool
        };
        //! in 3D
        template <> struct Boolean<Coord3D> { typedef Bool3D Type; //!< 3D bool
        };


        //! operations on coordinates
        struct Coord
        {

            //! get static info
            template <typename COORD> struct Get
            {
                static const unsigned                 Dimensions = sizeof(COORD)/sizeof(Coord1D);   //!< the dimension
                typedef typename Boolean<COORD>::Type BooleanType; //!< Boolean Companion Type
            };

            //! display with width
            static std::ostream & Display( std::ostream &os, const Coord1D *addr, const size_t size, const unsigned w);

            //! display a coordinate as a vector with width for each component
            template <typename COORD> static inline
            std::ostream & Disp( std::ostream &os, const COORD &c, const unsigned w=3)
            {
                return Display(os, (const Coord1D *) &c, Get<COORD>::Dimensions, w);
            }


            //! x,y,z
            static const char *AxisName(const size_t dim) throw();


            //! set coordinate to j[,j[,j]]
            template <typename COORD> static inline
            void LD( COORD &c, const Coord1D j ) throw()
            {
                Coord1D *p = (Coord1D*)&c;
                for(size_t i=0;i<Get<COORD>::Dimensions;++i)
                {
                    p[i] = j;
                }
            }


            //! get specific coordinate
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

            //! lexicographic compare
            template <typename COORD> static inline
            int Compare(const COORD &lhs, const COORD &rhs) throw()
            {
                const Coord1D *L = (const Coord1D *)&lhs;
                const Coord1D *R = (const Coord1D *)&rhs;
                for(size_t i=0;i<Get<COORD>::Dimensions;++i)
                {
                    const Coord1D l = L[i];
                    const Coord1D r = R[i];
                    if(l<r)       { return -1; }
                    else if( r<l) { return  1; }
                    else            continue;
                }
                return 0;
            }

            //! norm for coordinate
            template <typename COORD> static inline
            Coord1D Norm1( const COORD &c ) throw()
            {
                const Coord1D *p = (const Coord1D*)&c;
                Coord1D ans = abs_of(p[0]);
                for(size_t i=1;i<Get<COORD>::Dimensions;++i)
                {
                    ans += abs_of(p[i]);
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
            static void Parse(Coord1D *p, const size_t n, const string &args, const char *id);
            
            //! parse wrapper
            static void Parse(Coord1D *p, const size_t n, const char   *text, const char *id);
            
            //! parse a full coodinate
            template <typename COORD> static inline
            COORD Parse( const string &args )
            {
                COORD ans(0);
                Parse((Coord1D *)&ans,Get<COORD>::Dimensions, args);
                return ans;
            }
            
            //! parse a full coordinate
            template <typename COORD> static inline
            COORD Parse( const char *args, const char *id=0 )
            {
                COORD ans(0);
                Parse((Coord1D *)&ans, Get<COORD>::Dimensions, args, id);
                return ans;
            }

            //------------------------------------------------------------------
            //
            // local ranks => global rank
            //
            //------------------------------------------------------------------
            //! check consistency
            static void CheckRanks(const Coord1D *size, const Coord1D *rank, const unsigned dim);

            //! return global rank 1D: ranks.x
            static inline Coord1D GlobalRank( const Coord1D &sizes, const Coord1D &ranks )
            {
                CheckRanks(&sizes,&ranks,1);
                return ranks;
            }
            
            //! return global rank: 2D: ranks.x + ranks.y * sizes.x
            static inline Coord1D GlobalRank( const Coord2D &sizes, const Coord2D &ranks )
            {
                CheckRanks(&sizes.x,&ranks.x,2);
                return ranks.x + ranks.y * sizes.x;
            }
            
            //!return global rank: 3D:ranks.x + ranks.y * sizes.x + ranks.z * sizes.x * sizes.y = ranks.x + sizes.x * ( ranks.y + sizes.y * ranks.z)
            static inline Coord1D GlobalRank( const Coord3D &sizes, const Coord3D &ranks )
            {
                CheckRanks(&sizes.x,&ranks.x,3);
                return ranks.x + sizes.x * ( ranks.y + sizes.y * ranks.z);
            }


            //------------------------------------------------------------------
            //
            // global rank => local rank
            //
            //------------------------------------------------------------------

            //! return local ranks 1D: (ranks.x=r)
            static inline Coord1D LocalRanks( const Coord1D &sizes, const Coord1D &r)
            {
                CheckRanks(&sizes, &r,1);
                return r;
            }
            
             //! return local ranks 2D: (ranks.y=r/sizes.x,ranks.x=r%sizes.x)
            static inline Coord2D LocalRanks( const Coord2D &sizes, const Coord1D &rank)
            {
                const ldiv_t  dx = ldiv(rank,sizes.x);
                const Coord2D ranks(dx.rem,dx.quot);
                CheckRanks(&sizes.x, &ranks.x, 2);
                return ranks;
            }
            
            //! return local ranks 3D
            static inline Coord3D LocalRanks( const Coord3D &sizes, const Coord1D &rank)
            {
                const ldiv_t dx = ldiv(rank,sizes.x);
                const ldiv_t dy = ldiv(dx.quot,sizes.y);
                const Coord3D ranks(dx.rem,dy.rem,dy.quot);
                CheckRanks(&sizes.x,&ranks.x,3);
                return ranks;
            }

            //! regularize a neighbour set of ranks
            template <typename COORD>
            static inline COORD Regularized( const COORD &sizes, const COORD &ranks ) throw()
            {
                COORD ans = ranks;
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim )
                {
                    Coord1D      &rank = Of(ans,dim);
                    const Coord1D size = Of(sizes,dim);  
                    if(rank<0)
                    {
                        rank = size-1;
                    }
                    else if(rank>=size)
                    {
                        rank = 0;
                    }
                }
                return ans;
            }

            //------------------------------------------------------------------
            //
            // Boolean Operations
            //
            //------------------------------------------------------------------
            //! conversion to boolean
            template <typename COORD> static inline
            typename Get<COORD>::BooleanType ToBoolean( const COORD &c ) throw()
            {
                typename Get<COORD>::BooleanType ans(false);
                bool *flag = (bool *)&ans;
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    flag[dim] = (Of(c,dim) != 0);
                }
                return ans;
            }
        };

        
    }
    
}

#endif

