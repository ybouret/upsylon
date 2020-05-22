//! \file

#ifndef Y_SPADE_TYPES_INCLUDED
#define Y_SPADE_TYPES_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/strfwd.hpp"
#include "y/comparison.hpp"

namespace upsylon {
    
    namespace randomized
    {
        class bits; //!< forward declaration
    }
    
    namespace Spade
    {
        //----------------------------------------------------------------------
        //
        // Global types and definitions
        //
        //----------------------------------------------------------------------

        typedef unit_t           Coord1D;   //!< 1D coordinate
        typedef point2d<Coord1D> Coord2D;   //!< 2D coordinates
        typedef point3d<Coord1D> Coord3D;   //!< 3D coordinates
        typedef counted_object   Object;    //!< alias
        
        //----------------------------------------------------------------------
        //
        //! operations on coordinates
        //
        //----------------------------------------------------------------------
        class Coord
        {
        public:
            static const  Coord1D __Zero[4];    //!< binary mapping of [0[,0[,0]]]
            static const  Coord1D __Ones[4];    //!< binary mapping of [1[,1[,1]]]
            static const  bool    __OK[4];      //!< binary mapping of [true[,true[,true]]]
            static const  bool    __NO[4];      //!< binary mapping of [false[,false[,false]]]
            static size_t         DispWidth;    //!< display width per dimension
            
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

            //! lexicographic
            template <typename COORD> static inline
            int Increasing(const COORD &lhs, const COORD &rhs) throw()
            {
                return comparison::increasing_lexicographic( (const Coord1D *)&lhs, (const Coord1D *)&rhs, Get<COORD>::Dimensions );
            }

            //! lexicographic
            template <typename COORD> static inline
            int Decreasing(const COORD &lhs, const COORD &rhs) throw()
            {
                return comparison::decreasing_lexicographic( (const Coord1D *)&lhs, (const Coord1D *)&rhs, Get<COORD>::Dimensions );
            }


            
            //! map to true
            template <typename BOOLEAN> static inline
            const BOOLEAN & True() throw()
            {
                return *(const BOOLEAN *) &__OK[0];
            }
            
            //! map to false
            template <typename BOOLEAN> static inline
            const BOOLEAN & False() throw()
            {
                return *(const BOOLEAN *) &__NO[0];
            }
            
            //! bool extraction
            template <typename BOOLEAN> static inline
            bool &Flag(BOOLEAN &b, const unsigned dim) throw()
            {
                assert(dim<sizeof(BOOLEAN)/sizeof(bool));
                return *(((bool *)&b)+dim);
            }
            
            //! bool extraction
            template <typename BOOLEAN> static inline
            const bool &Flag(const BOOLEAN &b, const unsigned dim) throw()
            {
                assert(dim<sizeof(BOOLEAN)/sizeof(bool));
                return *(((const bool *)&b)+dim);
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

            //! check that coordinate is a unit vector
            template <typename COORD> static inline
            bool IsUnitVector(const unsigned dim, const COORD &C) throw()
            {
                assert(dim<Get<COORD>::Dimensions);
                for(unsigned d=0;d<dim;++d)
                {
                    if( 0 != Of(C,d) ) return false;
                }
                if( 1 != Of(C,dim) ) return false;
                for(unsigned d=dim+1;d<Get<COORD>::Dimensions;++d)
                {
                    if( 0 != Of(C,d) ) return false;
                }
                return true;
            }
            
            //! load to same value
            template <typename COORD> static inline
            const COORD Ld(const Coord1D value) throw()
            {
                COORD ans(value);
                for(unsigned dim=1;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = value;
                }
                return ans;
            }
            
            //! display a coordinate with fixed-width
            template <typename COORD> static inline
            std::ostream & Disp(std::ostream &os, const COORD C)
            {
                return Disp(os, (const Coord1D *) &C, Get<COORD>::Dimensions);
            }
           
            //! random in [0:C]
            template <typename COORD> static inline
            COORD Natural( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim< Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Natural1D( Of(C,dim), ran);
                }
                return ans;
            }
            
            //! random in [-C:C]
            template <typename COORD> static inline
            COORD Integer( const COORD C, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Integer1D( Of(C,dim), ran);
                }
                return ans;
            }
            
            //! random in [A,B]
            template <typename COORD> static inline
            COORD Within(const COORD A, const COORD B, randomized::bits &ran ) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Within1D( Of(A,dim), Of(B,dim), ran);
                }
                return ans;
            }
            
            //! product of coordinate
            static inline Coord1D Product(const Coord1D  C) throw() { return C; }
           
            //! product of coordinates
            static inline Coord1D Product(const Coord2D C) throw() { return C.prod(); }
            
            //! product of coordinates
            static inline Coord1D Product(const Coord3D C) throw() { return C.prod(); }
            
            //! Norm1 of coordinate
            static inline Coord1D Norm1(const Coord1D C) throw() { return abs_of(C); }
            
            //! Norm1 of coordinates
            static inline Coord1D Norm1(const Coord2D &C) throw() { return abs_of(C.x) + abs_of(C.y); }

            //! Norm1 of coordinates
            static inline Coord1D Norm1(const Coord3D &C) throw() { return abs_of(C.x) + abs_of(C.y) + abs_of(C.z); }
            
            //! sizes>1
            template <typename COORD>
            static inline COORD ToParallel(const COORD &sizes) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = Of(sizes,dim) > 1;
                }
                return ans;
            }
            
            //! value != 0
            template <typename COORD>
            static inline COORD ToBoolean(const COORD &value) throw()
            {
                COORD ans(0);
                for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)
                {
                    Of(ans,dim) = (Of(value,dim) != 0);
                }
                return ans;
            }

            
            //! parse a coordinate X[:Y[:Z]]
            template <typename COORD> static inline
            COORD Parse(const string &args)
            {
                COORD C(0);
                Parse( (Coord1D *)&C, Get<COORD>::Dimensions, args);
                return C;
            }
            
            //! operator implementation
#define Y_SPADE_COORD_OP(NAME,OP)                     \
template <typename COORD> static inline               \
bool NAME(const COORD lhs, const COORD rhs) throw() { \
for(unsigned dim=0;dim<Get<COORD>::Dimensions;++dim)  \
{                                                     \
if( Of(lhs,dim) OP Of(rhs,dim) ) continue;            \
return false;                                         \
}                                                     \
return true;                                          \
}
            //! operator implementations
#define Y_SPADE_COORD_OPS() \
Y_SPADE_COORD_OP(LT,<)      \
Y_SPADE_COORD_OP(LEQ,<=)    \
Y_SPADE_COORD_OP(GT,>)      \
Y_SPADE_COORD_OP(GEQ,>=)

            Y_SPADE_COORD_OPS()


        private:
            //! random in [0:m]
            static Coord1D Natural1D(const Coord1D m, randomized::bits & ) throw();
            
            //! random in [-m:m]
            static Coord1D Integer1D(const Coord1D m, randomized::bits & ) throw();
            
            //! random in [a:b]
            static Coord1D Within1D(const Coord1D a, const Coord1D b, randomized::bits & ) throw();
            
            //! display some fixed-width coordinates
            static std::ostream & Disp(std::ostream &,const Coord1D *,const unsigned);

            //! parse coordinates
            static void Parse( Coord1D *c, const unsigned dim, const string &args);

        };
        
    }
    
}

#endif

