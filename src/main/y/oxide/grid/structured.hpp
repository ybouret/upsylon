
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
            Y_DECL_ARGS(T,type);  //!< aliases
            Y_OXIDE_GRID_ALIAS(); //!< aliases from Grid
            
            //! setup
            inline explicit StructuredGrid(const LayoutType &L,
                                           const_vertex      org,
                                           const_vertex      spc) :
            GridType(L), origin(org), spacing(spc)
            {
                check();
            }

            //! cleanup
            inline virtual ~StructuredGrid() throw()
            {
                bzset_(origin);
                bzset_(spacing);
            }

            const_vertex     origin;    //!< global origin
            const_vertex     spacing;   //!< spacing

            //! recompose a vertex
            inline virtual const_vertex operator()(const_coord c) const throw()
            {
                assert(this->has(c));
                mutable_type f[4] = {0,0,0,0};
                return *(vertex *)f;
            }

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
