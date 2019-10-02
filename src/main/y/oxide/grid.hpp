//! \file
#ifndef Y_OXIDE_GRID_INCLUDED
#define Y_OXIDE_GRID_INCLUDED 1

#include "y/oxide/vtk.hpp"
#include "y/math/types.hpp"

namespace upsylon {

    namespace Oxide {

        //======================================================================
        //
        //
        //! common functions/data
        //
        //
        //======================================================================
        class Grid_
        {
        public:
            virtual ~Grid_() throw(); //!< cleanup
            explicit Grid_() throw(); //!< setup

            //! throw an explaining explication
            static void ExceptionLEQZ(const char *gridName, const char *context, const unsigned dim);

            //! get name[dim] of DefaultAxisName(dim)
            static const char *GetAxisName(const char **names, const size_t dim) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grid_);

        };

        //======================================================================
        //
        //
        //! Grid base interface
        //
        //
        //======================================================================
        template <typename COORD, typename T>
        class Grid : public Layout<COORD>, public Grid_
        {
        public:
            //==================================================================
            //
            // types and definitions
            //
            //==================================================================
            Y_DECL_ARGS(T,type);                                             //!< aliases
            static  const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef          Layout<COORD>            LayoutType;            //!< alias
            typedef typename LayoutType::coord        coord;                 //!< alias
            typedef typename LayoutType::const_coord  const_coord;           //!< alias
            typedef typename VertexFor<COORD,T>::Type vertex;                //!< alias
            typedef const    vertex                   const_vertex;          //!< alias
            typedef typename LayoutType::Loop         Loop;                  //!< alias

            //==================================================================
            //
            // virtual interface
            //
            //==================================================================

            //! get a vertex position from a coordinate
            virtual const_vertex operator()(const_coord c) const throw() = 0;

            //! save a sub layout of this grid to VTK
            virtual void write( vtk &, ios::ostream &, const LayoutType &) const = 0;

            //! cleanup
            inline virtual ~Grid() throw() {}

            //==================================================================
            //
            // non-virtual interface
            //
            //==================================================================

            //! zero vertex
            static inline const_vertex &Zero() throw()
            {
                static const T __zero[4] = { 0,0,0,0 };
                return *(const_vertex *)__zero;
            }

            //! vertex of ones
            static inline const_vertex &Ones() throw()
            {
                static const T __ones[4] = { 1,1,1,1 };
                return *(const_vertex *)__ones;
            }

            //! find the bounding box and return the difference
            inline vertex boundingBox( vertex &lo, vertex &hi ) const
            {
                const Grid  &self = *this;
                Loop        loop(self.lower,self.upper);

                loop.start();
                lo = hi = self(loop.value);


                {
                    mutable_type *l = (mutable_type *) &lo;
                    mutable_type *h = (mutable_type *) &hi;

                    for(loop.next();loop.valid();loop.next())
                    {
                        const vertex tmp = self(loop.value);
                        const_type  *t   = (const_type *) &tmp;

                        for(size_t dim=0;dim<Dimensions;++dim)
                        {
                            l[dim] = min_of(l[dim],t[dim]);
                            h[dim] = max_of(h[dim],t[dim]);
                        }
                    }
                }
                return hi-lo;
            }

            //! return the bounding space
            inline vertex boundingSpace() const
            {
                vertex lo = Zero(), hi=lo;
                return boundingBox(lo,hi);
            }

            //! compute the scaling length
            inline type scalingLength() const
            {
                static const_type q     = const_type(1)/Dimensions;
                const_vertex      space = boundingSpace();
                const_type  *s          = (const_type *) &space;
                mutable_type prod       = s[0];
                for(size_t dim=1;dim<Dimensions;++dim)
                {
                    prod *= s[dim];
                }
                assert(prod>=0);
                prod /= this->items;
                return math::pow_of(prod,q);
            }



        protected:
            //! setup
            inline explicit Grid(const LayoutType &L ) throw() : LayoutType(L)
            {

            }

            //! check positive components
            void CheckPositive(const char   *gridName,
                               const char   *context,
                               const_vertex &v ) const
            {
                const_type *p = (const_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    if(p[dim]<=0) Grid_::ExceptionLEQZ(gridName,context,dim);
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grid);
        };

        //! forwarding info to grids
#define Y_OXIDE_GRID_ALIAS()                            \
typedef             Grid<COORD,T>          GridType;    \
static const size_t Dimensions = GridType::Dimensions;  \
typedef typename    GridType::LayoutType   LayoutType;  \
typedef typename    GridType::coord        coord;       \
typedef typename    GridType::const_coord  const_coord; \
typedef typename    GridType::vertex       vertex;      \
typedef typename    GridType::const_vertex const_vertex;\
typedef typename    GridType::Loop         Loop


    }
}

#endif

