
// \file
#ifndef Y_OXIDE_STRUCTURED_GRID_INCLUDED
#define Y_OXIDE_STRUCTURED_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"

namespace upsylon {

    namespace Oxide {

        struct StructuredGrid_
        {
            static const char Name[];
            static const char Spacing[];
        };

        //! Structured Grid for VTK
        template <typename COORD, typename T>
        class StructuredGrid : public Grid<COORD,T>
        {
        public:
            typedef          Layout<COORD>            LayoutType;
            typedef typename LayoutType::coord        coord;
            typedef typename LayoutType::const_coord  const_coord;

            typedef typename VertexFor<COORD,T>::Type vertex;
            typedef const vertex                      const_vertex;
            
            inline explicit StructuredGrid(const LayoutType &L,
                                           const_vertex      org,
                                           const_vertex      spc) :
            layout(L), origin(org), spacing(spc)
            {
                check();
            }

            inline virtual ~StructuredGrid() throw()
            {
            }

            const LayoutType layout;
            const_vertex     origin;
            const_vertex     spacing;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(StructuredGrid);
            inline void check()
            {
                this->CheckPositive(StructuredGrid_::Name,
                                    StructuredGrid_::Spacing,
                                    spacing);
            }
        };


    }
}

#endif
