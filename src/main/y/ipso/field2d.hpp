//! \file
#ifndef Y_IPSO_FIELD2D_INCLUDED
#define Y_IPSO_FIELD2D_INCLUDED 1

#include "y/ipso/field1d.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! wrapper for constructor
#define Y_IPSO_FIELD2D_CTOR() layout_type(L), field<T>(id), row_layout(lower.x,upper.x), rows(0)

        //! field in 2D
        template <typename T>
        class field2D : public layout2D, public field<T>
        {
        public:
            Y_DECL_ARGS(T,type);            //!< aliases
            typedef layout2D   layout_type; //!< alias
            typedef field1D<T> row_type;    //!< alias
            const layout1D     row_layout;  //!< 1D layout for for
            //! setup
            inline explicit field2D( const string &id, const layout_type &L, void *dataEntry=0, void *rowsEntry=0) :
            Y_IPSO_FIELD2D_CTOR() { setup(dataEntry,rowsEntry); }

            //!setup
            inline explicit field2D( const char *id, const layout_type &L, void *dataEntry=0, void *rowsEntry=0) :
            Y_IPSO_FIELD2D_CTOR() { setup(dataEntry,rowsEntry); }

            //!setup helper
            inline explicit field2D(const char *id, const coord1D W, const coord1D H ) :
            layout_type( coord2D(0,0), coord2D(W,H) ),
            field<T>(id),
            row_layout(lower.x,upper.x),
            rows(0)
            {
                setup(0,0);
            }

            //! destructor
            inline virtual ~field2D() throw()
            {
                assert(rows);
                for(coord1D j=upper.y;j>=lower.y;--j)
                {
                    self_destruct( rows[j] );
                }
                if(this->allocated)
                {
                    this->__release(this->items);
                }
            }

            //! row access
            row_type & operator[](const coord1D y) throw()
            {
                assert(rows); assert(y>=lower.y); assert(y<=upper.y); return rows[y];
            }

            //! row access, const
            const row_type & operator[](const coord1D y) const throw()
            {
                assert(rows); assert(y>=lower.y); assert(y<=upper.y); return rows[y];
            }

            //! number of held items
            inline virtual  size_t size() const throw() { return items; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(field2D);
            row_type *rows;

            inline void setup( void *dataEntry, void *rowsEntry )
            {
                (size_t &)(this->bytes) = sizeof(T) * this->items;
                //______________________________________________________________
                //
                // linking data
                //______________________________________________________________
                if(dataEntry)
                {
                    assert(rowsEntry);
                    this->entry = static_cast<type     *>(dataEntry);
                    rows        = static_cast<row_type *>(rowsEntry);
                }
                else
                {
                    assert(!rowsEntry);
                    const size_t data_offset = 0;
                    const size_t data_length = this->bytes;
                    const size_t rows_offset = memory::align(data_length+data_offset);
                    const size_t rows_length = width.y * sizeof(row_type);
                    this->allocated = memory::align(rows_offset+rows_length);
                    this->allocate();
                    mutable_type *p = static_cast<type     *>( memory::io::__shift(this->workspace,data_offset) );
                    rows            = static_cast<row_type *>( memory::io::__shift(this->workspace,rows_offset) );
                    size_t count = 0;
                    try
                    {
                        while(count<items)
                        {
                            new (p+count) type();
                            ++count;
                        }
                    }
                    catch(...)
                    {
                        this->__release(count);
                        throw;
                    }
                    this->entry     = p;
                }

                create_rows();
            }

            inline void create_rows()
            {
                assert(this->entry);
                assert(rows);
                rows -= lower.y;
                mutable_type  *p = (mutable_type *)(this->entry);
                for(coord1D j=lower.y;j<=upper.y;++j)
                {
                    try
                    {
                        const string id = this->name + vformat("[%ld]", long(j) );
                        new (rows+j) row_type(id,row_layout,p);
                        p += width.x;
                    }
                    catch(...)
                    {
                        while(--j>=lower.y)
                        {
                            self_destruct( rows[j] );
                        }
                        if(this->allocated)
                        {
                            this->__release(items);
                        }
                        throw;
                    }
                }
            }
            
        };

    }

}

#endif

