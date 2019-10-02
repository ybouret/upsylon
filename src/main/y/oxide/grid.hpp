// \file
#ifndef Y_OXIDE_GRID_INCLUDED
#define Y_OXIDE_GRID_INCLUDED 1

#include "y/oxide/vtk.hpp"

namespace upsylon {

    namespace Oxide {

        //! common function
        class Grid_
        {
        public:
            virtual ~Grid_() throw();
            
            //! throw an explaining explication
            static void ExceptionLEQZ(const char *gridName, const char *context, const unsigned dim);

            //! get name[dim] of DefaultAxisName(dim)
            static const char *GetAxisName(const char **names, const size_t dim) throw();

        protected:
            explicit Grid_() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grid_);

        };

        //! Grid base interface
        template <typename COORD, typename T>
        class Grid : public Layout<COORD>, public Grid_
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            static  const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef          Layout<COORD>            LayoutType;            //!< alias
            typedef typename LayoutType::coord        coord;                 //!< alias
            typedef typename LayoutType::const_coord  const_coord;           //!< alias
            typedef typename VertexFor<COORD,T>::Type vertex;                //!< alias
            typedef const    vertex                   const_vertex;          //!< alias

            //! cleanup
            inline virtual ~Grid() throw() {}

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


            //! get a vertex position from a coordinate
            virtual const_vertex operator()(const_coord c) const throw() = 0;


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
typedef typename    GridType::const_vertex const_vertex


    }
}

#endif

