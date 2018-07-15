//! \file
#ifndef Y_MP_INTEGER_INCLUDED
#define Y_MP_INTEGER_INCLUDED 1

#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! named sign
        enum sign_type
        {
            __negative, //!< -1
            __zero,     //!<  0
            __positive  //!<  1
        };

        //! product of signs
        inline sign_type sign_product(const sign_type a, const sign_type b) throw()
        {
            if(a==__zero||b==__zero)
            {
                return __zero;
            }
            else
            {
                return (a!=b) ? __negative : __positive;
            }
        }

        //! sign for integral type
        inline sign_type sign_for(const integer_t i) throw()
        {
            return (i<0) ? __negative : ( (0<i)  ? __positive : __zero );
        }

        //! absolute value
        inline word_t word_for(const integer_t i) throw()
        {
            return (i<0) ? word_t(-i) : word_t(i);
        }

        //! integer class
        class integer : public object
        {
        public:
            const sign_type s; //!< it sign
            const natural   n; //!< it natural value

            //! zero constructor
            inline integer() : s(__zero), n() {}

            //! destructor
            inline virtual ~integer() throw() {}

            //! copy constructor
            inline integer(const integer &z) : s(z.s), n(z.n) {}

            //!  constructor from integral
            inline integer(const integer_t &i) : s( sign_for(i) ), n( word_for(i) ) {}

            //! constructor from natural
            inline integer(const natural &u) : s( u.is_zero() ? __zero : __positive ), n(u) {}

            //! status update
            inline void update() throw()
            {
                if(n.is_zero()) (sign_type &)s = __zero;
            }

            // output
            inline friend std::ostream & operator<<( std::ostream &os, const integer &z )
            {
                if(__negative==z.s) os << '-';
                return (os << z.n);
            }

            //! assignment
            

        private:

        };

    }
}


#endif

