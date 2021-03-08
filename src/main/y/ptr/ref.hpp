
//! \file
#ifndef Y_PTR_REF_INCLUDED
#define Y_PTR_REF_INCLUDED 1

#include "y/ptr/arc.hpp"

namespace upsylon
{
    //! A CONSTANT Reference Counted, without assign
    /**
     T must have the following methods
     - void   withhold() throw()
     - bool   liberate() throw()
     - size_t refcount() const throw()
     */
    template <typename T>
    class ref_ptr : public ptr<T>
    {
    public:
        //! attache and withhold address
        inline ref_ptr(T *addr) throw() : ptr<T>(addr)
        {
            assert(this->pointee||die("not accepting null address"));
            this->pointee->withhold();
        }

        //! copy by withholding
        inline ref_ptr(const ref_ptr &other) throw() : ptr<T>(other)
        {
            this->pointee->withhold();
        }

        //! copy by withholding
        inline ref_ptr(const arc_ptr<T> &other) throw() : ptr<T>(other)
        {
            this->pointee->withhold();
        }


        //! delete resource if necessary
        inline virtual ~ref_ptr() throw()
        {
            if(this->pointee->liberate())
            {
                delete (this->pointee);
            }
            this->zero();
        }

        //! get pointee reference count
        inline size_t refcount() const throw()
        {
            assert( this->pointee ); return this->pointee->refcount();
        }

    private:
        Y_DISABLE_ASSIGN(ref_ptr);
    };
}

#endif


