
// \file
#ifndef Y_OXIDE_CURVILINEAR_GRID_INCLUDED
#define Y_OXIDE_CURVILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"
#include "y/oxide/field3d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

        //! a curvilinear grid
        template <typename COORD, typename T>
        class CurvilinearGrid :
        public Grid<COORD,T>,
        public slots< typename FieldFor<COORD,T>::Type >
        {
        public:
            Y_OXIDE_GRID_ALIAS();
            typedef typename FieldFor<COORD,T>::Type Axis;
            typedef slots<Axis>                      Basis;

            inline explicit CurvilinearGrid(const LayoutType &L,
                                            const char       **names = NULL) :
            GridType(L),
            Basis( Dimensions )
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const string    aid = Grid_::GetAxisName(names,dim);
                    this->template  build<string,LayoutType>(aid,*this);
                }
            }

            inline virtual ~CurvilinearGrid() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearGrid);
        };

    }
}

#endif
