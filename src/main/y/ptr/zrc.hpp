
//! \file
#ifndef Y_PTR_ZRC_INCLUDED
#define Y_PTR_ZRC_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon
{
    //! A Reference Counted with possible NULL
    /**
     T must have the following methods
     - void   withhold() throw()
     - bool   liberate() throw()
     - size_t refcount() const throw()
     */
    template <typename T>
    class zrc_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::pointee_type pointee_type; //!< alias

        //! attache and withhold address
        inline zrc_ptr(T *addr) throw() : ptr<T>(addr)
        {
            if(this->pointee)
            {
                this->pointee->withhold();
            }
        }
        
        //! copy by withholding
        inline zrc_ptr(const zrc_ptr &other) throw() :
        ptr<T>(other)
        {
            if(this->pointee)
            {
                this->pointee->withhold();
            }
        }
        
        //! assign another pointer
        inline zrc_ptr & operator=(const zrc_ptr &other) throw()
        {
            zrc_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }
        
        //! delete resource if necessary
        inline virtual ~zrc_ptr() throw()
        {
            if((this->pointee)  &&
               (this->pointee->liberate()) )
            {
                delete (this->pointee);
                this->zero();
            }
        }

        //! get pointee reference count
        inline size_t refcount() const throw()
        {
            if(this->pointee)
            {
                return this->pointee->refcount();
            }
            else
            {
                return 0;
            }
        }

        //! yield pointee by decreasing #refcount
        T *yield() throw()
        {
            pointee_type *p = this->pointee;
            if(p)
            {
                (void) p->liberate();
            }
            this->zero();
            return p;
        }
        

    };
}

#endif


