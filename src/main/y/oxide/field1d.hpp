//! \file
#ifndef Y_OXIDE_FIELD1D_INCLUDED
#define Y_OXIDE_FIELD1D_INCLUDED 1

#include "y/oxide/field.hpp"
#include "y/oxide/layout.hpp"

namespace upsylon
{

    namespace Oxide
    {

        //! common constructor part
#define Y_OXIDE_FIELD1D_CTOR() Field<T>(id,*this), shift(NULL)
        
        //! 1D field
        template <typename T>
        class Field1D : public Layout1D, public Field<T>
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            //! cleanup
            inline virtual ~Field1D() throw() { shift=0; }

            //!  constructor with string and layout
            inline explicit Field1D(const string   &id,
                                    const Layout1D &L) :
            Layout1D(L),
            Y_OXIDE_FIELD1D_CTOR()
            {
                setupAt(NULL);
            }

            //! constructor with text and coordinates
            inline explicit Field1D(const char      *id,
                                    const Coord1D    lo,
                                    const Coord1D    up) :
            Layout1D(lo,up),
            Y_OXIDE_FIELD1D_CTOR()
            {
                setupAt(NULL);
            }


            //! user data size must be greater than this->bytes
            inline explicit Field1D(const string   &id,
                                    const Layout1D &L,
                                    void           *userData) :
            Layout1D(L),
            Y_OXIDE_FIELD1D_CTOR()
            {
                assert(userData);
                setupAt(userData);
            }

            //! access
            inline type & operator[]( const Coord1D i ) throw()
            {
                assert( this->has(i) );
                return shift[i];
            }

            //! access, const
            inline const_type & operator[]( const Coord1D i ) const throw()
            {
                assert( this->has(i) );
                return shift[i];
            }

            //! access by coordinate
            inline type & operator()(const Coord1D i) throw()
            {
                Field1D &self = *this; return self[i];
            }

            //! access by coordinate, const
            inline const_type & operator()(const Coord1D i) const throw()
            {
                Field1D &self = *this; return self[i];
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field1D);
            type  *shift;

            //! length_of(addr) >= linearSize (=bytes for all objects)
            inline void setupAt(void *addr)
            {
                // check memory
                if(!addr)
                {
                    this->privateSize = this->linearExtent;
                    this->makeData( ( addr=this->acquirePrivate() ) );
                }
                
                // update members
                assert(addr);
                this->entry = static_cast<type *>(addr);
                this->shift = this->entry - this->lower;
            }

        };
    }

}


#endif


