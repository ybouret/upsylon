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

            //! copy
            inline rational( const rational &q ) : num(q.num), den(q.den) {}

            //! no throw exchange
            inline void xch( rational &q ) throw()
            {
                ( (integer &)num ).xch( (integer &)(q.num) );
                ( (integer &)den ).xch( (integer &)(q.den) );
            }

            //!assign
            inline rational & operator=( const rational &q )
            {
                rational tmp(q);
                xch(tmp);
                return *this;
            }

            inline rational & operator=( const integer_t i )
            {
                rational tmp(i); xch(tmp); return *this;
            }

            inline rational & operator=( const integer & z )
            {
                rational tmp(z); xch(tmp); return *this;
            }

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            

        private:
            void __simplify();
        };
    }

    typedef mpl::rational mpq;
}

#endif

