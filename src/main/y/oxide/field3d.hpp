//! \file
#ifndef Y_OXIDE_FIELD3D_INCLUDED
#define Y_OXIDE_FIELD3D_INCLUDED 1

#include "y/oxide/field2d.hpp"

namespace upsylon
{
    
    namespace Oxide
    {
        //! common constructor part
#define Y_OXIDE_FIELD3D_CTOR()                     \
Field<T>(id,*this),                                \
slice(0), slices(0),                               \
sliceLayout(this->lower.xy(),this->upper.xy()),    \
rowLayout(sliceLayout.lower.x,sliceLayout.upper.x)
        
        //! field in 3D
        template <typename T>
        class Field3D : public Layout3D, public Field<T>
        {
        public:
            Y_DECL_ARGS(T,type);                           //!< aliases
            typedef Field2D<T> SliceType;                  //!< a slice
            typedef typename   SliceType::RowType RowType; //!< alias

            //! setup by string and full layout
            inline explicit Field3D(const string  &id,
                                    const Layout3D &L ) :
            Layout3D(L), Y_OXIDE_FIELD3D_CTOR()
            {
                setup();
                assert(this->entry);
            }

            //! setup by text and coordinates
            inline explicit Field3D(const char     *id,
                                    const Coord3D   lo,
                                    const Coord3D   hi) :
            Layout3D(lo,hi), Y_OXIDE_FIELD3D_CTOR()
            {
                setup();
                assert(this->entry);
            }
            
            //! destruct
            inline virtual ~Field3D() throw()
            {
                destructSlices();
            }

            //! slice access
            SliceType & operator[]( const Coord1D k ) throw()
            {
                assert(k>=this->lower.z); assert(k<=this->upper.z);
                return slice[k];
            }

            //! slice access, const
            const SliceType & operator[]( const Coord1D k ) const throw()
            {
                assert(k>=this->lower.z); assert(k<=this->upper.z);
                return slice[k];
            }

            //! direct access
            type & operator()(const_coord C) throw()
            {
                Field3D &self = *this;
                return self[C.z][C.y][C.x];
            }

            //! direct access, const
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
            const Layout2D   sliceLayout; //!< layout for one slice
            const Layout1D   rowLayout;   //!< layout for one row
            
        private:
            inline void destructSlices() throw()
            {
                
                size_t &ns = (size_t &)slices;
                slice += this->lower.z;
                while(ns>0)
                {
                    self_destruct( slice[--ns] );
                }
                slice = 0;
            }
            
            inline void setup()
            {
                //--------------------------------------------------------------
                // create memory chunks
                //--------------------------------------------------------------
                RowType      *rowsAddr = NULL;
                mutable_type *dataAddr = NULL;
                const size_t rows_per_slice = size_t(this->width.y);
                {
                    const size_t  num_slices = size_t(this->width.z);
                    memory::embed chunks[]   =
                    {
                        memory::embed::as<SliceType>    (slice,    num_slices               ),
                        memory::embed::as<RowType>      (rowsAddr, num_slices*rows_per_slice),
                        memory::embed::as<mutable_type> (dataAddr, this->linearExtent       )
                    };
                    this->privateData = memory::embed::create_global(chunks, sizeof(chunks)/sizeof(chunks[0]), this->privateSize);
                }

                //--------------------------------------------------------------
                // link memory
                //--------------------------------------------------------------
                slice -= this->lower.z;
                this->makeData(dataAddr);
                this->entry = dataAddr;
                try
                {
                    const size_t data_per_slice = sliceLayout.items; assert( data_per_slice == size_t(this->width.x*this->width.y) );
                    size_t      &ns             = (size_t &)slices;
                    for(Coord1D k=this->lower.z;k<=this->upper.z;++k)
                    {
                        const string id = this->subName(k);
                        new ( &slice[k] ) SliceType(id,sliceLayout,rowsAddr,dataAddr);
                        ++ns;
                        dataAddr += data_per_slice;
                        rowsAddr += rows_per_slice;
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

