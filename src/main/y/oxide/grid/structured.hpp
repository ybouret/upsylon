
// \file
#ifndef Y_OXIDE_STRUCTURED_GRID_INCLUDED
#define Y_OXIDE_STRUCTURED_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"

namespace upsylon {

    namespace Oxide {

        //======================================================================
        //
        //! shared API
        //
        //======================================================================
        struct StructuredGrid_
        {
            static const char Name[];    //!< "StructuredGrid"
            static const char Spacing[]; //!< "spacing"
        };

        //======================================================================
        //
        //! Structured Grid for VTK
        //
        //======================================================================
        template <typename COORD, typename T>
        class StructuredGrid : public Grid<COORD,T>
        {
        public:
            typedef          Layout<COORD>            LayoutType;  //!< alias
            typedef typename LayoutType::coord        coord;       //!< alias
            typedef typename LayoutType::const_coord  const_coord; //!< alias

            typedef typename VertexFor<COORD,T>::Type vertex;       //!< alias
            typedef const vertex                      const_vertex; //!< alias
            
            //! setup
            inline explicit StructuredGrid(const LayoutType &L,
                                           const_vertex      org,
                                           const_vertex      spc) :
            layout(L), origin(org), spacing(spc)
            {
                check();
            }

            //! cleanup
            inline virtual ~StructuredGrid() throw()
            {
                bzset_(origin);
                bzset_(spacing);
            }

            const LayoutType layout;    //!< layout metrics
            const_vertex     origin;    //!< global origin
            const_vertex     spacing;   //!< spacing

            
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
