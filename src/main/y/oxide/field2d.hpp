//! \file
#ifndef Y_OXIDE_FIELD2D_INCLUDED
#define Y_OXIDE_FIELD2D_INCLUDED 1

#include "y/oxide/field1d.hpp"

namespace upsylon
{

    namespace Oxide
    {
        template <typename T>
        class Field2D :
        public Layout2D,
        public Field<T>
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef Field1D<T> RowType;

            explicit Field2D(const string   &id,
                             const Layout2D &L) :
            Layout2D(L),
            Field<T>(id,*this),
            row(0),
            rows(0),
            rowLayout(this->lower.x,this->upper.x)
            {
                setup(NULL,NULL);
            }

            inline virtual ~Field2D() throw()
            {
                destructRows();
            }

            inline RowType & operator[]( const Coord1D j ) throw()
            {
                assert(j>=this->lower.y); assert(j<=this->upper.y);
                return row[j];
            }

            inline const RowType & operator[]( const Coord1D j ) const throw()
            {
                assert(j>=this->lower.y); assert(j<=this->upper.y);
                return row[j];
            }

            inline type & operator()( const Coord2D c ) throw()
            {
                assert( this->has(c) );
                Field2D &self = *this;
                return self[c.y][c.x];
            }

            inline const_type & operator()( const Coord2D c ) const throw()
            {
                assert( this->has(c) );
                Field2D &self = *this;
                return self[c.y][c.x];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field2D);
            RowType       *row;
        public:
            const size_t   rows;      //!< currently built rows
            const Layout1D rowLayout;

        private:
            void destructRows() throw()
            {
                row        += this->lower.y;
                size_t &r   = (size_t &)rows;
                while(r>0)
                {
                    destruct( &row[--r] );
                }
            }

            void setup(void *rowsAddr,
                       void *dataAddr)
            {
                const size_t nr = size_t( this->width.y );
                if(!rowsAddr)
                {
                    assert(!dataAddr);
                    const size_t rowsSize = memory::align(sizeof(RowType)*nr);
                    const size_t dataSize = memory::align(this->linearSize);
                    this->privateSize     = rowsSize+dataSize;
                    rowsAddr              = this->acquirePrivate();
                    dataAddr              = memory::io::__shift( rowsAddr, rowsSize );

                    // build local data
                    this->makeData(dataAddr,*this);
                }

                // build rows
                try
                {
                    mutable_type *ptr = static_cast<mutable_type *>(dataAddr);
                    const size_t  jmp = rowLayout.items;
                    size_t       &r   = (size_t &)rows;
                    row               = static_cast<RowType *>(rowsAddr) - this->lower.y;

                    for(Coord1D j=this->lower.y;j<=this->upper.y;++j)
                    {
                        const string id;
                        new ( &row[j] ) RowType(id,rowLayout,ptr);
                        ++r;
                        ptr += jmp;
                    }
                }
                catch(...)
                {
                    destructRows();
                    throw;
                }


            }
        };

    }

}

#endif

