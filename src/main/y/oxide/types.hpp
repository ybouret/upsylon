//! \file
#ifndef Y_OXIDE_TYPES_INCLUDED
#define Y_OXIDE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/container/sequence.hpp"
#include "y/strfwd.hpp"
#include "y/type/bzset.hpp"
#include "y/comparison.hpp"

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
            static const Coord1D __Zero[4]; //!< {0,0,0,0}
            static const Coord1D __Ones[4]; //!< {1,1,1,1}

            //==================================================================
            //
            // access
            //
            //==================================================================

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
            void Ld( COORD &c, const Coord1D j ) throw()
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

            //! direct mapping of a [0,[,0[,0]]] coordinate
            template <typename COORD> static inline
            const COORD & Zero() throw()
            {
                return *(const COORD *)__Zero;
            }

            //! direct mapping of a [1,[,1[,1]]] coordinate
            template <typename COORD> static inline
            const COORD & Ones() throw()
            {
                return *(const COORD *)__Ones;
            }


            //==================================================================
            //
            // operations
            //
            //==================================================================

            //! c
            static inline Coord1D Product( const Coord1D &c ) throw() { return c; }

            //! c.x*c.y
            static inline Coord1D Product( const Coord2D &c ) throw() { return c.x*c.y; }

            //! c.x*c.y*c.z
            static inline Coord1D Product( const Coord3D &c ) throw() { return c.x*c.y*c.z; }

            //! <a|b>
            static inline Coord1D Dot( const Coord1D &a, const Coord1D &b ) throw() { return a*b; }
            //! <a|b>
            static inline Coord1D Dot( const Coord2D &a, const Coord2D &b ) throw() { return a.x*b.x+a.y*b.y; }
            //! <a|b>
            static inline Coord1D Dot( const Coord3D &a, const Coord3D &b ) throw() { return a.x*b.x+a.y*b.y+a.z*b.z; }

            //! lexicographic compare
            template <typename COORD> static inline
            int Compare(const COORD &lhs, const COORD &rhs) throw()
            {
                return comparison::increasing_lexicographic((const Coord1D *)&lhs, (const Coord1D *)&rhs, Get<COORD>::Dimensions);
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
            
            //==================================================================
            //
            // random
            //
            //==================================================================
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
            
            //==================================================================
            //
            // parsing
            //
            //==================================================================
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

            //==================================================================
            //
            // local ranks => global rank
            // all functions are checkec
            //
            //==================================================================
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


            //==================================================================
            //
            // global rank => local rank
            // all functions are checked
            //
            //==================================================================

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

            //==================================================================
            //
            // Boolean Conversion
            //
            //==================================================================
            //! conversion to boolean
            template <typename COORD> static inline
            typename Get<COORD>::BooleanType ToBoolean( const COORD &c ) throw()
            {
                typename Get<COORD>::BooleanType Flag(false);
                bool                            *flag = (bool *)&Flag;
                for(size_t dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    flag[dim] = (Of(c,dim) != 0);
                }
                return Flag;
            }
        };


        
    }
    
}

#endif

