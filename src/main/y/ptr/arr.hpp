//! \file

#ifndef Y_PTR_ARR_INCLUDED
#define Y_PTR_ARR_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon {

    //! smart pointer to forward addressable-like behavior
    template <typename ARRAY, template <class> class PTR>
    class arr_ptr : public PTR<ARRAY>, public addressable<typename ARRAY::type>
    {
    public:
        typedef PTR<ARRAY>                   ptr_type;       //!< alias
        typedef typename ARRAY::type         type;           //!< alias
        typedef typename ARRAY::mutable_type mutable_type;   //!< alias
        typedef typename ARRAY::const_type   const_type;     //!< alias
        typedef addressable<type>            base_type;      //!< alias

        //! setup
        inline          arr_ptr( ARRAY *arr ) : collection(), ptr_type(arr), base_type() {}

        //! cleanup
        inline virtual ~arr_ptr() throw() {}

        //! copy
        inline          arr_ptr(const arr_ptr &other ) throw() : collection(), ptr_type(other), base_type() {}

        //! assign
        inline arr_ptr & operator=( const arr_ptr &other ) throw()
        {
            ptr_type &self = *this;
            self           = other;
            return *this;
        }

        //! get size
        inline virtual size_t size() const throw()
        {
            return (this->pointee) ? this->pointee->size() : 0;
        }

        inline virtual type & operator[]( const size_t index )
        {
            return ( *(this->pointee) ) [ index ];
        }

        inline virtual const_type & operator[]( const size_t index ) const
        {
            return ( *(this->pointee) ) [ index ];
        }

        //! use the addressable function to display
        inline friend std::ostream & operator << ( std::ostream &os, const arr_ptr &arr )
        {
            const base_type &self = arr;
            return self.display(os);
        }


        
    };

}

#endif

