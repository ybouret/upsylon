//! \file
#ifndef Y_IPSO_FIELD1D_INCLUDED
#define Y_IPSO_FIELD1D_INCLUDED 1

#include "y/ipso/field.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! wrapper for constructor
#define Y_IPSO_FIELD1D_CTOR() layout_type(L), field<T>(id), shift(0)

        //! field in 1D
        template <typename T>
        class field1D : public layout1D, public field<T>
        {
        public:
            Y_DECL_ARGS(T,type);          //!< aliases
            typedef layout1D layout_type; //!< alias

            //! setup
            inline explicit field1D( const string &id, const layout_type &L, void *data=0) :
            Y_IPSO_FIELD1D_CTOR() { setup(data); }

            //!setup
            inline explicit field1D( const char *id, const layout_type &L, void *data=0) :
            Y_IPSO_FIELD1D_CTOR() { setup(data); }

            //! destructor
            inline virtual ~field1D() throw()
            {
                if(this->allocated)
                {
                    this->__release(this->items);
                }
            }

            //! access
            inline type & operator[](const coord1D i) throw()
            {
                assert(i>=lower); assert(i<=upper); return shift[i];
            }

            //! access, const
            inline const_type & operator[](const coord1D i) const throw()
            {
                assert(i>=lower); assert(i<=upper); return shift[i];
            }

            //! number of held items
            virtual  size_t size() const throw() { return items; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(field1D);
            mutable_type *shift;

            inline void setup(void *data)
            {
                (size_t &)(this->bytes) = sizeof(T) * this->items;
                if( !(this->workspace=data) )
                {
                    this->allocated = this->bytes;
                    this->allocate();
                }
                assert(this->workspace);
                this->entry = static_cast<type *>(this->workspace);
                shift       = this->entry - this->lower;

                //! is data is passed on, already allocated
                if(!data)
                {
                    this->__make(this->items);
                }
            }

        };

    }
}

#endif

