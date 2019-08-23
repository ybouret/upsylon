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
            rows(0),
            row(0),
            rowLayout(this->lower.x,this->upper.x)
            {
                setup(NULL,NULL);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field2D);
            size_t   rows;
            RowType *row;

        public:
            const Layout1D rowLayout;

        private:
            inline void setup(void *rowsAddr,
                              void *dataAddr )
            {
                const size_t nr = static_cast<size_t>(this->width.y);
                if(!rowsAddr)
                {
                    assert(!dataAddr);
                    const size_t rowsSize = memory::align( sizeof(RowType) * nr );
                    const size_t dataSize = memory::align( this->bytes );
                    this->privateSize = rowsSize + dataSize;
                    this->acquirePrivate();
                    rowsAddr = this->privateData;
                    dataAddr = memory::io::__shift(rowsAddr,rowsSize);

                    // build owned
                    this->makeData(dataAddr,*this);
                }


                this->entry = static_cast<type *>(dataAddr);

            }
        };

    }

}

#endif

