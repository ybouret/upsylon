#ifndef Y_PTR_BATCH_INCLUDED
#define Y_PTR_BATCH_INCLUDED 1

#include "y/ptr/shared.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    //! assuming T is a cluster-like type, and forward different functions
    template <typename T, template <class> class PTR = shared_ptr >
    class batch_ptr : public PTR<T>
    {
    public:
        Y_DECL_ARGS(T,type);         //!< aliases
        typedef PTR<T> ptype;        //!< alias

        virtual ~batch_ptr() throw() {}                                    //!< cleanup
        inline   batch_ptr(T               *addr) throw() : ptype(addr) {} //!< setup
        inline   batch_ptr(const batch_ptr &_   ) throw() : ptype(_) {}    //!< copy

        //! assign
        inline batch_ptr & operator=( const batch_ptr &other ) throw()
        {
            batch_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }

        //! forward access
        inline type & operator[](const size_t index) throw()
        {
            return (this->pointee)[index];
        }

        //! forward const access
        inline const_type & operator[](const size_t index) const throw()
        {
            return (this->pointee)[index];
        }

        //! forward size
        inline size_t size() const throw() { return this->pointee->size(); }


    };

}

#endif
