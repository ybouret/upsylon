
//! \file
#ifndef Y_PTR_INTR_INCLUDED
#define Y_PTR_INTR_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon
{
    //! A Reference Counted with key
    /**
     T must have the following methods
     - void   withhold() throw()
     - bool   liberate() throw()
     - size_t refcount() const throw()
     - const_key_type & key() const throw()
     */
    template <typename KEY,typename T>
    class intr_ptr : public ptr<T>
    {
    public:
        Y_DECL_ARGS(KEY,key_type); //!< alias

        //! attache and withhold address
        inline intr_ptr(T *addr) throw() : ptr<T>(addr)
        {
            assert(this->pointee||die("not accepting null address"));
            this->pointee->withhold();
        }

        //! copy by withholding
        inline intr_ptr(const intr_ptr &other) throw() :
        ptr<T>(other)
        {
            this->pointee->withhold();
        }

        //! assign another pointer
        inline intr_ptr & operator=(const intr_ptr &other) throw()
        {
            intr_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }

        //! delete resource if necessary
        inline virtual ~intr_ptr() throw()
        {
            if(this->pointee->liberate())
            {
                delete (this->pointee);
            }
        }

        //! transitive key access
        inline const const_key_type & key() const throw()
        {
            assert(this->pointee);
            return this->pointee->key();
        }

    };
}

#endif


