// \file
#ifndef Y_OXIDE_GRID_INCLUDED
#define Y_OXIDE_GRID_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/type/args.hpp"

namespace upsylon {

    namespace Oxide {

        //! common function
        struct Grid_
        {
            //! throw an explaining explication
            static void LEQZ(const char *gridName, const char *context, const unsigned dim);
        };

        //! Grid base interface
        template <typename COORD, typename T>
        class Grid
        {
        public:
            Y_DECL_ARGS(T,type);
            static  const size_t Dimensions = Coord::Get<COORD>::Dimensions;
            typedef typename VertexFor<COORD,T>::Type vertex;
            typedef const    vertex                   const_vertex;

            inline virtual ~Grid() throw() {}



        protected:
            inline explicit Grid() throw() {}
            void CheckPositive(const char   *gridName,
                               const char   *context,
                               const_vertex &v ) const
            {
                const_type *p = (const_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    if(p[dim]<=0) Grid_::LEQZ(gridName,context,dim);
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grid);
        };

    }
}

#endif

