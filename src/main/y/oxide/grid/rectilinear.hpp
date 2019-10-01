// \file
#ifndef Y_OXIDE_RECTILINEAR_GRID_INCLUDED
#define Y_OXIDE_RECTILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"
#include "y/oxide/field1d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

        //! a rectilinear grid
        template <typename COORD, typename T>
        class RectilinearGrid : public Grid<COORD,T>, public slots< Field1D<T> >
        {
        public:
            Y_OXIDE_GRID_ALIAS();      //!< aliases
            typedef Field1D<T>  Axis;  //!< 1D field of same type
            typedef slots<Axis> Basis; //!< the collection of axis


            //! setup
            inline explicit RectilinearGrid(const LayoutType    &L,
                                            const char          **names = NULL ) :
            GridType(L),
            Basis(Dimensions)
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Layout1D  sub = this->projectOn(dim);
                    const string    aid = Grid_::GetAxisName(names,dim);
                    this->template  build<string,Layout1D>(aid,sub);
                }
            }


            //! cleanup
            inline virtual ~RectilinearGrid() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearGrid);
        };


    }
}

#endif
