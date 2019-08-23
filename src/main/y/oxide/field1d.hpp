//! \file
#ifndef Y_OXIDE_FIELD1D_INCLUDED
#define Y_OXIDE_FIELD1D_INCLUDED 1

#include "y/oxide/field/info.hpp"
#include "y/oxide/field/data.hpp"
#include "y/oxide/layout.hpp"

namespace upsylon
{

    namespace Oxide
    {

        namespace Kernel
        {
            class Block1D
            {
            public:
                virtual ~Block1D() throw();

            private:
                size_t privateMemory;

            protected:
                void  *dataAddress;

                explicit Block1D(const size_t items,
                                 const size_t item_size) ;

                explicit Block1D() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Block1D);

            };
        }


#define Y_OXIDE_FIELD1D_ITEM() item( this->entry - this->lower )

#define Y_OXIDE_FIELD1D_CTOR()        \
Field<T>(id),                          \
LayoutType(L),                          \
BlockType(this->items,sizeof(T)),        \
FDataType(this->dataAddress,this->items), \
Y_OXIDE_FIELD1D_ITEM()

        template <typename T>
        class Field1D :
        public Field<T>,
        public Layout1D,
        public Kernel::Block1D,
        public FieldData<T>
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef Layout1D        LayoutType;
            typedef Kernel::Block1D BlockType;
            typedef FieldData<T>    FDataType;

            inline virtual ~Field1D() throw()
            {
            }

            inline explicit Field1D(const string     &id,
                                    const LayoutType &L) :
            Y_OXIDE_FIELD1D_CTOR()
            {
            }

            inline explicit Field1D(const char      *id,
                                    const LayoutType &L) :
            Y_OXIDE_FIELD1D_CTOR()
            {
            }


            inline explicit Field1D(const string     &id,
                                    const LayoutType &L,
                                    void             *userData) :
            FieldInfo(id),
            LayoutType(L),
            BlockType(),
            FDataType(userData,0),
            Y_OXIDE_FIELD1D_ITEM()
            {
            }

            inline type & operator[]( const Coord1D i ) throw()
            {
                assert( this->has(i) );
                return item[i];
            }

            inline const_type & operator[]( const Coord1D i ) const throw()
            {
                assert( this->has(i) );
                return item[i];
            }

            inline type & operator()(const Coord1D i) throw()
            {
                Field1D &self = *this; return self[i];
            }

            inline const_type & operator()(const Coord1D i) const throw()
            {
                Field1D &self = *this; return self[i];
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field1D);
            type *item;

        };

    }

}


#endif


