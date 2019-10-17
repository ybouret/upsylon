#ifndef Y_PTR_BATCH_INCLUDED
#define Y_PTR_BATCH_INCLUDED 1


#include "y/ptr/shared.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    //! smart pointer to forward
    template <typename ADDRESSABLE, template <class> class PTR = shared_ptr >
    class batch_ptr : public PTR<ADDRESSABLE> 
    {
    public:
        typedef typename ADDRESSABLE::type         type;           //!< alias
        typedef typename ADDRESSABLE::mutable_type mutable_type;   //!< alias
        typedef typename ADDRESSABLE::const_type   const_type;     //!< alias
        typedef PTR<ADDRESSABLE> ptype;                            //!< alias

        virtual ~batch_ptr() throw() {}                                      //!< cleanup
        inline   batch_ptr(ADDRESSABLE     *addr) throw()  : ptype(addr)  {} //!< setup
        inline   batch_ptr(const batch_ptr &other) throw() : ptype(other) {} //!< copy

        //! assign
        inline batch_ptr & operator=( const batch_ptr &other ) throw()
        {
            batch_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }

        //! forward access
        inline type & operator[](const size_t index)
        {
            return (* (this->pointee) )[index];
        }

        //! forward const access
        inline const_type & operator[](const size_t index) const 
        {
            return (* (this->pointee) )[index];
        }

        //! forward size
        inline size_t size() const throw() { return this->pointee->size(); }


    };

}

#endif
