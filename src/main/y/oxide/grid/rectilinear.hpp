// \file
#ifndef Y_OXIDE_RECTILINEAR_GRID_INCLUDED
#define Y_OXIDE_RECTILINEAR_GRID_INCLUDED 1

#include "y/oxide/grid.hpp"
#include "y/oxide/field1d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Oxide {

        //! shared data
        struct RectilinearGrid_
        {
            static const char Name[];           //!< "RectilinearGrid"
            static const char SqueezedLayout[]; //!< "Squeezed Layout"
        };

        //! a rectilinear grid
        template <typename COORD, typename T>
        class RectilinearGrid : public Grid<COORD,T>, public slots< Field1D<T> >
        {
        public:
            Y_DECL_ARGS(T,type);       //!< alias
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

            //! get full vertex
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


            //! inter/extrapolate
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
