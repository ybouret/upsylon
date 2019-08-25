//! \file
#ifndef Y_OXIDE_FIELD3D_INCLUDED
#define Y_OXIDE_FIELD3D_INCLUDED 1

#include "y/oxide/field2d.hpp"

namespace upsylon
{

    namespace Oxide
    {
        //! field in 3D
        template <typename T>
        class Field3D :
        public Layout3D,
        public Field<T>
        {
        public:
            Y_DECL_ARGS(T,type);                           //!< aliases
            typedef Field2D<T> SliceType;                  //!< a slice
            typedef typename   SliceType::RowType RowType; //!< alias

            inline explicit Field3D(const string  &id,
                                    const Layout3D &L ) :
            Layout3D(L),
            Field<T>(id,*this),
            slice(0), slices(0),
            sliceLayout(this->lower.xy(),this->upper.xy()),
            rowLayout(sliceLayout.lower.x,sliceLayout.upper.x)
            {
                setup();
            }

            inline virtual ~Field3D() throw()
            {
                destructSlices();
            }

            SliceType & operator[]( const Coord1D k ) throw()
            {
                assert(k>=this->lower.z); assert(k<=this->upper.z);
                return slice[k];
            }

            const SliceType & operator[]( const Coord1D k ) const throw()
            {
                assert(k>=this->lower.z); assert(k<=this->upper.z);
                return slice[k];
            }

            type & operator()(const_coord C) throw()
            {
                Field3D &self = *this;
                return self[C.z][C.y][C.x];
            }

            const_type & operator()(const_coord C) const throw()
            {
                const Field3D &self = *this;
                return self[C.z][C.y][C.x];
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field3D);
            SliceType       *slice;  //!< location of slice
        public:
            const size_t     slices;      //!< number of constructed slices
            const Layout2D   sliceLayout;
            const Layout1D   rowLayout;

        private:

            void destructSlices() throw()
            {

                size_t &ns = (size_t &)slices;
                slice += this->lower.z;
                std::cerr << "-slice@" << (void *)slice << std::endl;
                while(ns>0)
                {
                    destruct( &slice[--ns] );
                }
                slice = 0;
            }

            void setup()
            {
                // compute needed memory
                const size_t num_slices     = size_t(this->width.z);
                const size_t s_bytes        = memory::align( num_slices * sizeof(SliceType) );
                const size_t data_per_slice = sliceLayout.items; assert( data_per_slice == size_t(this->width.x*this->width.y) );

                const size_t rows_per_slice = size_t(this->width.y);
                const size_t num_rows       = rows_per_slice * num_slices;
                const size_t r_bytes        = memory::align( num_rows * sizeof(RowType) );

                const size_t d_bytes        = memory::align( this->items * sizeof(T) );

                std::cerr << "s:" << s_bytes << " r:" << r_bytes << " d:" << d_bytes << std::endl;
                this->privateSize = s_bytes + r_bytes + d_bytes;
                void *addr = this->acquirePrivate();

                // link memory
                slice           = static_cast<SliceType    *>( memory::io::__shift(addr,0)        );
                RowType      *r = static_cast<RowType      *>( memory::io::__shift(slice,s_bytes) );
                mutable_type *d = static_cast<mutable_type *>( memory::io::__shift(r,r_bytes)     );
                std::cerr << "+slice@" << (void *)slice << std::endl;
                slice -= this->lower.z;
                this->entry = d;
                try
                {
                    size_t &ns = (size_t &)slices;
                    for(Coord1D k=this->lower.z;k<=this->upper.z;++k)
                    {
                        const string id = this->subName(k);
                        new ( &slice[k] ) SliceType(id,sliceLayout,r,d);
                        ++ns;
                        d += data_per_slice;
                        r += rows_per_slice;
                    }

                }
                catch(...)
                {
                    destructSlices();
                    throw;
                }


            }

        };

    }

}

#endif

