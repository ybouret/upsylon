//! \file
#ifndef Y_POINTER_AUTO_INCLUDED
#define Y_POINTER_AUTO_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon
{
    //! automatic pointer for resources management
    template <typename T>
    class auto_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::pointee_type pointee_type; //!< instead of using

        //! attach any resource
        inline auto_ptr( T *addr ) throw() : ptr<T>(addr) {}

        //! delete any resource
        inline ~auto_ptr() throw() { kill(); }

        //! get ownership
        inline auto_ptr( const auto_ptr &other ) throw() :
        ptr<T>(other)
        {
            other.zero();
        }

        //! get ownership
        inline auto_ptr & operator=( const auto_ptr &other ) throw()
        {
            if(this!=&other)
            {
                kill();
                this->pointee = other.pointee;
                other.zero();
            }
            return *this;
        }

        //! get ownership
        inline auto_ptr & operator=( T *addr ) throw()
        {
            if(this->pointee!=addr)
            {
                kill();
                this->pointee = pointee_type(addr);
            }
            return *this;
        }

        //! drop ownership, return what was pointed at...
        inline T *yield() throw()
        {
            T *p = this->pointee;
            zero();
            return p;
        }

        //! dismiss ownership
        inline void dismiss() throw() { zero(); }

    private:
        void zero() const throw() { *(pointee_type *)( &(this->pointee) ) = 0;  }
        void kill() throw()       { if(this->pointee) { delete this->pointee; this->pointee=0; } }
    };
}

#endif

