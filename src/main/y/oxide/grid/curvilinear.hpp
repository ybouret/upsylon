
// \file
#ifndef Y_OXIDE_CURVILINEAR_GRID_INCLUDED
#define Y_OXIDE_CURVILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"
#include "y/oxide/field3d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

        //! shared data
        class CurvilinearGrid_
        {
        public:
            static const char Name[];           //!< "CurvilinearGrid"
            static const char SqueezedLayout[]; //!< "Squeezed Layout"
            static const char VTK_DATASET_ID[]; //!< "STRUCTURED_GRID"

            virtual ~CurvilinearGrid_() throw();

        protected:
            explicit CurvilinearGrid_() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearGrid_);
        };

        //! a curvilinear grid
        template <typename COORD, typename T>
        class CurvilinearGrid :
        public Grid<COORD,T>,
        public slots< typename FieldFor<COORD,T>::Type >,
        public CurvilinearGrid_
        {
        public:
            Y_DECL_ARGS(T,type);                             //!< aliases
            Y_OXIDE_GRID_ALIAS();                            //!< forwarded aliases
            typedef typename FieldFor<COORD,T>::Type Axis;   //!< same Dimensions fields
            typedef slots<Axis>                      Basis;  //!< the collection of axis

            //! setup with optional name
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

            //! cleanup
            inline virtual ~CurvilinearGrid() throw() {}

            //! get the full vertex
            inline virtual const_vertex operator()( const_coord c ) const throw()
            {
                assert(this->has(c));
                mutable_type  f[4] = { 0,0,0,0 };
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Axis  &axs = (*this)[dim];
                    assert(axs.has(c));
                    f[dim] = axs(c);
                }
                return *(vertex *)f;
            }


            //! inter/extrapolate
            inline void mapRegular(const LayoutType  &sub,
                                   const_vertex       ini,
                                   const_vertex       end)
            {

                Basis        &self = *this;
                const_vertex  del  = end-ini;
                const_coord   dd   = sub.upper-sub.lower;
                const_type   *I    = (const_type *) &ini;
                const_type   *D    = (const_type *) &del;

                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    if(Coord::Of(dd,dim)<=0)
                    {
                        Grid_::ExceptionLEQZ(Name,SqueezedLayout,dim);
                    }
                }

                typename LayoutType::Loop loop(sub.lower,sub.upper);
                for( loop.start(); loop.valid(); loop.next() )
                {
                    const_coord &c = loop.value;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        self[dim](c) = I[dim] + ( (Coord::Of(c,dim)-Coord::Of(sub.lower,dim))*D[dim]) /Coord::Of(dd,dim);
                    }
                }

            }

            inline virtual void write( vtk &VTK, ios::ostream &fp, const LayoutType &sub ) const
            {
                static const vtk::Writer &tw = VTK.get<type>();

                // emit dataset
                fp << vtk::DATASET << ' ' << VTK_DATASET_ID << '\n';

                // emit dimensions, increased for vtk
                VTK.writeDimensions(fp,sub.width) << '\n';

                // emit POINTS
                VTK(fp << vtk::POINTS << ' ',sub.items) << ' ' << tw.dataType() << '\n';

                emitPoints(VTK,fp,sub, type2type<COORD>() );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearGrid);

            void emitPoints(vtk &VTK, ios::ostream &fp, const LayoutType &sub, type2type<Coord1D> ) const
            {
            }

            void emitPoints(vtk &VTK, ios::ostream &fp, const LayoutType &sub, type2type<Coord2D> ) const
            {
            }

            void emitPoints(vtk &VTK, ios::ostream &fp, const LayoutType &sub, type2type<Coord3D> ) const
            {
                const GridType           &self = *this;

                typename LayoutType::Loop loop(sub.lower,sub.upper);
                for( loop.start(); loop.valid(); loop.next() )
                {
                    const vertex v = self(loop.value);
                    VTK(fp,v) << '\n';
                }
            }




        };

    }
}

#endif
