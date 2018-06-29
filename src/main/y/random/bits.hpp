//! \file
#ifndef Y_RANDOM_BITS_INCLUDED
#define Y_RANDOM_BITS_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/type/bswap.hpp"
#include <cstdlib>

namespace upsylon
{

    namespace random
    {
        //! interface to bits generation
        class bits
        {
        public:
            virtual ~bits() throw(); //!< destructor
            const uint32_t span; //!< next32 in 0..span
            const size_t   nbit; //!< bits_for(span)
            const uint32_t half; //!< span/2
            const double   denD; //!< 1.0+double(span)
            const float    denF; //!< 1.0f+float(span)

            virtual uint32_t next32() throw() = 0; //!< next 32-bits value in 0..span

            template <typename T> T to() throw(); //!< in 0:1 exclusive [float|double]

            //! random full integral type
            template <typename T> inline
            T full() throw()
            {

                static const size_t full_bits = sizeof(T)*8;
                const  size_t       self_bits = nbit;
                size_t              read_bits = 0;
                T ans(0);
                do
                {
                    (ans <<= self_bits) |= T(next32());
                    read_bits += self_bits;
                }
                while(read_bits<full_bits);
                return ans;
            }

            //! random unsigned integral in 0..X-1
            template <typename T> inline
            T __lt(const T X) throw()
            {
                return ( (X<=0) ? T(0) : ( full<T>() % X ) );
            }

            //! 0..n-1
            inline size_t lt(const size_t n) throw()
            {
                return __lt<size_t>(n);
            }

            //! returns in 0..n
            inline size_t leq( size_t n ) throw()
            {
                return full<size_t>() % (++n);
            }

            //! Knuth shuffle of a[0..n-1]
            template <typename T>
            inline void shuffle( T *a, const size_t n ) throw()
            {
                assert(!(NULL==a&&n>0));
                if( n > 1 )
                {
                    for( size_t i=n-1;i>0;--i)
                    {
                        const size_t j   = leq(i);
                        bswap( a[i], a[j] );
                    }
                }
            }

            //! Knuth co shuffle of a[0..n-1] and b[0..n-1]
            template <typename T, typename U>
            inline void shuffle( T *a, U *b, const size_t n) throw()
            {
                assert(!(NULL==a&&n>0));
                if( n > 1 )
                {
                    for( size_t i=n-1;i>0;--i)
                    {
                        const size_t j   = leq(i);
                        bswap( a[i], a[j] );
                        bswap( b[i], b[j] );
                    }
                }
            }

            //! a random boolean
            inline bool choice() throw()
            {
                return (next32() <= half);
            }



        protected:
            //! sets span and auxiliary values
            explicit bits(const uint32_t maxValue) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bits);
        };

        //! fast demo based on rand()
        class cstdbits : public bits
        {
        public:
            inline explicit cstdbits() throw() : bits(RAND_MAX) {}
            inline virtual ~cstdbits() throw() {}
            inline virtual uint32_t next32() throw() { return rand(); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(cstdbits);
        };


    }

}

#endif

