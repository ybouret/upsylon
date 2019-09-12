//! \file
#ifndef Y_OXIDE_FIELD2D_INCLUDED
#define Y_OXIDE_FIELD2D_INCLUDED 1

#include "y/oxide/field1d.hpp"
#include "y/memory/embed.hpp"

namespace upsylon
{

    namespace Oxide
    {
        //! common constructor part
#define Y_OXIDE_FIELD2D_CTOR()         \
FieldOf<T>(id,*this),                  \
row(0), rows(0),                       \
rowLayout(this->lower.x,this->upper.x)
        
        //! field in 2D
        template <typename T>
        class Field2D : public Layout2D, public FieldOf<T>
        {
        public:
            Y_DECL_ARGS(T,type);           //!< aliases
            typedef Layout2D   LayoutType; //!< alias
            typedef Field1D<T> RowType;    //!< a RowType

            //! construct by string and layout
            explicit Field2D(const string   &id,
                             const Layout2D &L) :
            Layout2D(L), Y_OXIDE_FIELD2D_CTOR()
            {
                setup(NULL,NULL);
            }

            //! construct by string and layout
            explicit Field2D(const string   &id,
                             const Layout2D &L,
                             void           *rowsAddr,
                             void           *dataAddr) :
            Layout2D(L), Y_OXIDE_FIELD2D_CTOR()
            {
                setup(rowsAddr,dataAddr);
            }

            //! construct by label and two coordinated
            explicit Field2D(const char     *id,
                             const Coord2D   lo,
                             const Coord2D   hi) :
            Layout2D(lo,hi), Y_OXIDE_FIELD2D_CTOR()
            {
                setup(NULL,NULL);
            }


            //! construct as matrix
            explicit Field2D(const char     *id,
                             const Coord1D   w,
                             const Coord1D   h) :
            Layout2D( Coord2D(0,0), Coord2D(w-1,h-1) ), Y_OXIDE_FIELD2D_CTOR()
            {
                setup(NULL,NULL);
            }


            //! cleanup
            inline virtual ~Field2D() throw()
            {
                destructRows();
            }

            //! access row
            inline RowType & operator[]( const Coord1D j ) throw()
            {
                assert(j>=this->lower.y); assert(j<=this->upper.y);
                return row[j];
            }

            //! access row, CONST
            inline const RowType & operator[]( const Coord1D j ) const throw()
            {
                assert(j>=this->lower.y); assert(j<=this->upper.y);
                return row[j];
            }

            //! access type
            inline type & operator()( const Coord2D c ) throw()
            {
                assert( this->has(c) );
                Field2D &self = *this;
                return self[c.y][c.x];
            }


            //! acess type, CONST
            inline const_type & operator()( const Coord2D c ) const throw()
            {
                assert( this->has(c) );
                const Field2D &self = *this;
                return self[c.y][c.x];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field2D);
            RowType       *row;
        public:
            const size_t   rows;      //!< currently built rows
            const Layout1D rowLayout; //!< layout for a single row

        private:
            void destructRows() throw()
            {
                row        += this->lower.y;
                size_t &r   = (size_t &)rows;
                while(r>0)
                {
                    self_destruct( row[--r] );
                }
                row = 0;
            }

            void setup(void *rowsAddr,
                       void *dataAddr)
            {
                //--------------------------------------------------------------
                // extract the number of rows to build
                //--------------------------------------------------------------
                const size_t nr = size_t( this->width.y );

                if(!rowsAddr)
                {
                    //----------------------------------------------------------
                    // fill embedded info
                    //----------------------------------------------------------
                    assert(!dataAddr);
                    {
                        memory::embed chunks[] =
                        {
                            memory::embed( &rowsAddr, sizeof(RowType)*nr ),
                            memory::embed( &dataAddr, this->linearExtent )
                        };
                        this->privateData = memory::embed::create_global(chunks, sizeof(chunks)/sizeof(chunks[0]),this->privateSize);
                    }
                    //----------------------------------------------------------
                    // build local data@dataAddr,
                    // that shall be released by ~Field
                    //----------------------------------------------------------
                    this->makeData(dataAddr);
                }
                else
                {
                    assert( dataAddr!=NULL );
                }

                //--------------------------------------------------------------
                // build rows@rowsAddr
                //--------------------------------------------------------------
                try
                {
                    mutable_type *ptr = static_cast<mutable_type *>(dataAddr);
                    const size_t  jmp = rowLayout.items;
                    size_t       &r   = (size_t &)rows;
                    row               = static_cast<RowType *>(rowsAddr) - this->lower.y;

                    for(Coord1D j=this->lower.y;j<=this->upper.y;++j)
                    {
                        const string id = this->subName(j);
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

                //--------------------------------------------------------------
                // link entry
                //--------------------------------------------------------------
                this->entry = static_cast<type *>(dataAddr);
            }
        };

    }

}

#endif

