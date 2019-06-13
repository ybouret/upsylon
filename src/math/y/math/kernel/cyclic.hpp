//! \file
#ifndef Y_MATH_KERNEL_CYCLIC_INCLUDED
#define Y_MATH_KERNEL_CYCLIC_INCLUDED 1

#include "y/math/kernel/tridiag.hpp"


namespace upsylon
{
    namespace math
    {

        class cyclic_info
        {
        public:
            static const size_t cyclic_reserved = 3;
            const size_t        cyclic_offset;
            const size_t        cyclic_extraneous;

            virtual ~cyclic_info() throw();

        protected:
            explicit cyclic_info(const size_t internal, const size_t avail) throw();
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cyclic_info);
        };

        template <typename T>
        class cyclic :  public tridiag<T>, public cyclic_info
        {
        public:
            typedef typename arrays<T>::array_type  array_type;  //!< alias
            typedef typename real_for<T>::type      scalar_type; //!< alias

            inline virtual ~cyclic() throw() {}

            inline cyclic(const size_t n, const size_t extra=0) :
            tridiag<T>(n,cyclic_reserved+extra),
            cyclic_info(this->tridiag_reserved+cyclic_reserved,extra),
            bb( this->next() ),
            u(  this->next() ),
            z(  this->next() )
            {
            }

            //! get array [0..extraneous-1]
            inline virtual const array_type & get_array(const size_t i) const throw()
            {
                assert(i<cyclic_extraneous);
                return this->raw_get(i+cyclic_offset);
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(cyclic);
            array_type &bb;
            array_type &u;
            array_type &z;

        };

    }
}

#endif

