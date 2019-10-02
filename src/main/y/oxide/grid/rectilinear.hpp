//! \file
#ifndef Y_OXIDE_RECTILINEAR_GRID_INCLUDED
#define Y_OXIDE_RECTILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"
#include "y/oxide/field1d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

        //======================================================================
        //
        //
        //! shared API
        //
        //
        //======================================================================
        class RectilinearGrid_
        {
        public:
            static const char  Name[];              //!< "RectilinearGrid"
            static const char  SqueezedLayout[];    //!< "Squeezed Layout"
            static const char  VTK_DATASET_ID[];    //!< "RECTILINEAR_GRID"
            virtual ~RectilinearGrid_() throw();    //!< cleanup
            explicit RectilinearGrid_() throw();    //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearGrid_);
        };

        //======================================================================
        //
        //
        //! a rectilinear grid
        //
        //
        //======================================================================
        template <typename COORD, typename T>
        class RectilinearGrid : public Grid<COORD,T>, public slots< Field1D<T> >, public RectilinearGrid_
        {
        public:
            //==================================================================
            //
            // types and definitions
            //
            //==================================================================
            Y_DECL_ARGS(T,type);       //!< alias
            Y_OXIDE_GRID_ALIAS();      //!< aliases
            typedef Field1D<T>  Axis;  //!< 1D field of same type
            typedef slots<Axis> Basis; //!< the collection of axis

            //==================================================================
            //
            // C++
            //
            //==================================================================

            //------------------------------------------------------------------
            //! setup
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //! cleanup
            //------------------------------------------------------------------
            inline virtual ~RectilinearGrid() throw()
            {
            }

            //==================================================================
            //
            // Grid interface
            //
            //==================================================================


            //------------------------------------------------------------------
            //! get full vertex
            //------------------------------------------------------------------
            inline virtual const_vertex operator()( const_coord c ) const throw()
            {
                assert(this->has(c));
                
                mutable_type  f[4] = { 0,0,0,0 };
                const unit_t *u    = (const unit_t *)&c;

                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const unit_t idx = u[dim];
                    const Axis  &axs = (*this)[dim];
                    assert(axs.has(idx));
                    f[dim] = axs[idx];
                }
                return *(vertex *)f;
            }



            //------------------------------------------------------------------
            //! write to vtk
            //------------------------------------------------------------------
            inline virtual void write( vtk &VTK, ios::ostream &fp, const LayoutType &sub ) const
            {
                static const vtk::Writer &tw = VTK.get<type>();

                // emit dataset
                fp << vtk::DATASET << ' ' << VTK_DATASET_ID << '\n';

                // emit dimensions, increased for vtk
                VTK.writeDimensions(fp,sub.width) << '\n';
                
                // emit axis
                mutable_type delta=0;
                size_t       count=0;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Axis     &a   = (*this)[dim];
                    const unit_t    w   = Coord::Of(sub.width,dim);
                    const unit_t    ini = Coord::Of(sub.lower,dim);
                    const unit_t    end = Coord::Of(sub.upper,dim);

                    fp << char('X'+dim) << vtk::_COORDINATES << ' ';
                    VTK(fp,w);
                    fp << ' ' << tw.dataType()  << '\n';
                    for(unit_t i=ini;i<=end;++i)
                    {
                        if(i>ini) { fp << ' '; }
                        tw.write(fp,&a[i]);
                    }
                    fp << '\n';
                    delta += abs_of(a[end]-a[ini]);
                    count += w;
                }

                if(count<=0)
                {
                    delta=1;
                }
                else
                {
                    delta/=count;
                }

                for(size_t dim=Dimensions;dim<3;++dim)
                {
                    fp << char('X'+dim) << vtk::_COORDINATES << " 2 " << tw.dataType()  << '\n';
                    fp << '0' << ' ';
                    tw.write(fp, &delta);
                    fp << '\n';
                }

            }

            //==================================================================
            //
            // Specific
            //
            //==================================================================

            //------------------------------------------------------------------
            //! inter/extrapolate
            //------------------------------------------------------------------
            inline void mapRegular(const LayoutType  &sub,
                                   const_vertex       ini,
                                   const_vertex       end)
            {
                Basis      &self = *this;
                const_type *I    = (const_type *) &ini;
                const_type *E    = (const_type *) &end;

                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    // get sub parameters in that dimension
                    const unit_t lo = Coord::Of(sub.lower,dim);
                    const unit_t hi = Coord::Of(sub.upper,dim);
                    if(lo>=hi)
                    {
                        Grid_::ExceptionLEQZ(RectilinearGrid_::Name,
                                             RectilinearGrid_::SqueezedLayout,
                                             dim);
                    }
                    const unit_t dd = hi-lo;

                    // get this parameters in that dimension
                    const unit_t i0    = Coord::Of(this->lower,dim);
                    const unit_t i1    = Coord::Of(this->upper,dim);
                    Axis        &ax    = self[dim];
                    const_type   start = I[dim];
                    const_type   stop  = E[dim];
                    const_type   delta = stop-start;

                    // loop over self parameters with coincidence on
                    // sub parameters
                    for(unit_t i=i0;i<=i1;++i)
                    {
                        ax[i] = start + (delta * (i-lo) ) / dd;
                    }

                    // special cases to avoid roundoff
                    if(ax.has(lo))
                    {
                        ax[lo] = start;
                    }
                    if(ax.has(hi))
                    {
                        ax[hi] = stop;
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearGrid);
        };


    }
}

#endif
