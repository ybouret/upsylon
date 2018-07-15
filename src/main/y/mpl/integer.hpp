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

            void update() throw()
            {
                if(n.is_zero()) (sign_type &)s = __zero;
            }
        };

    }
}


#endif

