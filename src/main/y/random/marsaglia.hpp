#ifndef Y_RANDOM_MARSAGLIA_INCLUDED
#define Y_RANDOM_MARSAGLIA_INCLUDED 1

#include "y/random/bits.hpp"

namespace upsylon
{
    namespace random
    {

        //! 32 bits marsaglia generator
        class Marsaglia
        {
        public:
            uint32_t z,w,jsr,jcong,a,b;
            uint32_t x,y,bro;
            uint8_t  c;
            uint32_t t[256];

            explicit Marsaglia() throw();
            virtual ~Marsaglia() throw();

            typedef uint32_t (Marsaglia::*Generator)();
            uint32_t mwc()   throw(); //!< MWC
            uint32_t shr3()  throw(); //!< SHR3
            uint32_t cong()  throw(); //!< CONG
            uint32_t fib()   throw(); //!< FIB
            uint32_t kiss()  throw(); //!< KISS
            uint32_t lfib4() throw(); //!< LFIB4
            uint32_t swb()   throw(); //!< SWB

            //! reset with user's data
            void settable( uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6 ) throw();

            //! reset from one seed
            void reset( uint32_t s ) throw();

            //! reset from hardware
            void initialize() throw();

            //! perform sanity check
            static void test();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Marsaglia);
        };

        template< Marsaglia::Generator G >
        class MarsagliaBits : public bits, public Marsaglia
        {
        public:
            inline explicit MarsagliaBits() : bits( 0xffffffff ), Marsaglia() {}
            inline virtual ~MarsagliaBits() throw() {}
            inline virtual uint32_t next32() throw() { return (*this.*G)(); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MarsagliaBits);
        };

        typedef MarsagliaBits<& Marsaglia::kiss > Kiss32;


    }
}

#endif

