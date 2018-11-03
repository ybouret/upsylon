//! \file
#ifndef Y_IPSO_FIELD3D_INCLUDED
#define Y_IPSO_FIELD3D_INCLUDED 1

#include "y/ipso/field2d.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! wrapper for constructor
#define Y_IPSO_FIELD3D_CTOR() layout_type(L), field<T>(id), slices(0)

        //! field in 3D
        template <typename T>
        class field3D : public layout3D, public field<T>
        {
        public:
            Y_DECL_ARGS(T,type);            //!< aliases
            typedef layout3D   layout_type; //!< alias
            typedef field1D<T> row_type;    //!< alias
            typedef field2D<T> slice_type;  //!< alias

            //! setup
            inline explicit field3D( const string &id, const layout_type &L) :
            Y_IPSO_FIELD3D_CTOR() { setup(); }

            //!setup
            inline explicit field3D( const char *id, const layout_type &L) :
            Y_IPSO_FIELD3D_CTOR() { setup(); }

            //! desctructor
            inline virtual ~field3D() throw()
            {

                this->__release(items);
            }

            //! number of held items
            virtual size_t size() const throw() { return items; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(field3D);
            slice_type *slices;

            void setup()
            {
                (size_t &)(this->bytes)        = items * sizeof(T);

                // get memory requirement
                const size_t    num_slices     = width.z;
                const size_t    rows_per_slice = width.y;
                const size_t    num_rows       = num_slices*rows_per_slice;

                const size_t    data_offset    = 0;
                const size_t    data_length    = this->bytes;
                const size_t    rows_offset    = memory::align(data_offset+data_length);
                const size_t    rows_length    = num_rows  * sizeof(row_type);
                const size_t    slices_offset  = memory::align(rows_offset+rows_length);
                const size_t    slices_length  = num_slices * sizeof(slice_type);
                this->allocated = memory::align(slices_offset+slices_length);
                this->allocate();

                mutable_type *d = static_cast<mutable_type *>( memory::io::__shift(this->workspace,data_offset)   );
                row_type     *r = static_cast<row_type     *>( memory::io::__shift(this->workspace,rows_offset)   );
                slices          = static_cast<slice_type   *>( memory::io::__shift(this->workspace,slices_offset) );

                // create data
                this->entry = d;
                this->__make(items);
                
                const layout2D sub( lower.xy(), upper.xy() );


            }
        };

    }

}

#endif

