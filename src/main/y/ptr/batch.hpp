#ifndef Y_PTR_BATCH_INCLUDED
#define Y_PTR_BATCH_INCLUDED 1

#include "y/ptr/shared.hpp"

namespace upsylon {

    //! assuming T is a cluster-like type, and forward different functions
    template <typename T>
    class batch_ptr : public shared_ptr<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef shared_ptr<T> ptype;

        virtual ~batch_ptr() throw() {}

        inline batch_ptr(T               *addr) throw() : ptype(addr) {}
        inline batch_ptr(const batch_ptr &_   ) throw() : ptype(_) {}

        inline batch_ptr & operator=( const batch_ptr &other ) throw()
        {
            batch_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }

        inline type & operator[](const size_t index) throw()
        {
            return (this->pointee)[index];
        }

        inline const_type & operator[](const size_t index) const throw()
        {
            return (this->pointee)[index];
        }

        inline size_t size() const throw() { return this->pointee->size(); }


    };

}

#endif
