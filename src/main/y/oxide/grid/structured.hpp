
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
        class StructuredGrid_
        {
        public:
            static const char Name[];           //!< "StructuredGrid"
            static const char VTK_DATASET_ID[]; //!< "STRUCTURED_POINTS"

            virtual ~StructuredGrid_() throw();

        protected:
            explicit StructuredGrid_() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StructuredGrid_);
        };

        //======================================================================
        //
        //! Structured Grid for VTK
        //
        //======================================================================
        template <typename COORD, typename T>
        class StructuredGrid : public Grid<COORD,T>, public StructuredGrid_
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
                const_type  *org  = (const type *)&origin;
                const_type  *spc  = (const type *)&spacing;


                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const unit_t u = Coord::Of(c,dim);
                    f[dim] = org[dim] + type(u) * spc[dim];
                }
                return *(vertex *)f;
            }

            //! write to vtk
            void write( vtk &VTK, ios::ostream &fp, const LayoutType &sub ) const
            {
                static const vtk::Writer &tw   = VTK.get<type>();
                const GridType           &self = *this; assert(self.contains(sub));

                // emit dataset
                fp <<  vtk::DATASET << ' ' << VTK_DATASET_ID << '\n';

                // emit dimensions, increased for vtk
                VTK.writeDimensions(fp,sub.width);

                // emit origin=lower bound
                {
                    const_vertex vtx  = self(sub.lower);
                    const_type  *org  = (const_type *)&vtx;
                    fp << vtk::ORIGIN;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        tw.write(fp << ' ', &org[dim]);
                    }
                    for(size_t dim=Dimensions;dim<3;++dim)
                    {
                        fp << ' ' << '0';
                    }
                    fp << '\n';
                }


                // emit spacing
                {
                    mutable_type sum = 0;
                    const_type  *spc = (const_type *)&spacing;
                    fp << vtk::SPACING;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        const_type s = spc[dim];
                        tw.write(fp << ' ', &s);
                        sum += s;
                    }
                    sum /= Dimensions;
                    for(size_t dim=Dimensions;dim<3;++dim)
                    {
                        tw.write(fp << ' ', &sum);
                    }
                    fp << '\n';
                }

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StructuredGrid);
            inline void check() { this->CheckPositive( Name, vtk::SPACING, spacing); }
        };


    }
}

#endif
