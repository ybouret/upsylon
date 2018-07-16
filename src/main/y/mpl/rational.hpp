//! \file
#ifndef Y_RATIONAL_INCLUDED
#define Y_RATIONAL_INCLUDED 1

#include "y/mpl/integer.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! multiple precision rational
        class rational : public object
        {
        public:
            const integer num;
            const natural den;

            //! update to simplified form
            void check();

            //! default constructor, zero
            inline rational() : num(),den(1) {}
            //! destructor
            inline virtual ~rational() throw() {}
            //! from integral type
            inline rational( const integer_t i ) : num(i), den(1) {}
            //! from mpz
            inline rational( const integer & z ) : num(z), den(1) {}
            //! from integral fraction
            inline rational( const integer_t n, const word_t d) : num(n), den(d) { check(); }
            //! from mpz/mpn
            inline rational( const integer &z, const natural &u) : num(z), den(u) { check(); }

            //! output
            inline friend std::ostream & operator<<( std::ostream &os, const rational &q )
            {
                if(q.den.is_byte(1))
                {
                    os << q.num;
                }
                else
                {
                    os << '(' << q.num << '/' << q.den << ')';
                }
                return os;
            }

        private:
            void __simplify();
        };
    }

    typedef mpl::rational mpq;
}

#endif

