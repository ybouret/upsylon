

//! \file
#ifndef Y_AP_INTEGER_INCLUDED
#define Y_AP_INTEGER_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {



    namespace yap {

        //! integer type
        class integer : public number
        {
        public:
            static const char CLASS_NAME[];


            //C++
            integer(); //!< 0
            virtual ~integer() throw();
            integer(const integer &);
            integer(const itype    );
            integer(const sign_type, const natural &);
            integer & operator=(const integer &);
            integer & operator=(const itype    );

            integer(const sign_type, randomized::bits &, const size_t nbits);
            integer(randomized::bits &, const size_t nbits);

            // methods
            void xch(integer &) throw();

            // serializable
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream &) const;
            static  integer     read(ios::istream &, size_t &, const char *); //!< relaod

            // helpers
            static uint8_t   sign2byte(const sign_type) throw();
            static sign_type byte2sign(const uint8_t  );

            // members
            const sign_type s;
            const natural   n;

            friend std::ostream & operator<<( std::ostream &, const integer &z);

            // comparison
            inline friend bool operator==( const integer &lhs, const integer &rhs ) throw()
            {
                return (lhs.s == rhs.s) && (lhs.n==rhs.n);
            }

            inline friend bool operator!=(const integer &lhs, const integer &rhs) throw()
            {
                return ( lhs.s != rhs.s) || (lhs.n!=rhs.n);
            }

        private:
            void update() throw();
        };

    }

}


#endif
