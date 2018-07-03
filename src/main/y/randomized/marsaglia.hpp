//! \file
#ifndef Y_RANDOM_MARSAGLIA_INCLUDED
#define Y_RANDOM_MARSAGLIA_INCLUDED 1

#include "y/randomized/bits.hpp"

namespace upsylon
{
    namespace randomized
    {

        //! 32 bits Marsaglia's generators
        class Marsaglia
        {
        public:
            uint32_t z;      //!< internal space
            uint32_t w;      //!< internal space
            uint32_t jsr;    //!< internal space
            uint32_t jcong;  //!< internal space
            uint32_t a;      //!< internal space
            uint32_t b;      //!< internal space
            uint32_t x;      //!< internal space
            uint32_t y;      //!< internal space
            uint32_t bro;    //!< internal space
            uint8_t  c;      //!< internal space
            uint32_t t[256]; //!< internal space

            explicit Marsaglia() throw(); //!< default initialisation
            virtual ~Marsaglia() throw(); //!< destructor

            typedef uint32_t (Marsaglia::*Generator)(); //!< generator method prototype
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
            static bool test();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Marsaglia);
        };

        //! create bits from Marsaglia's implementation
        template< Marsaglia::Generator G >
        class MarsagliaBits : public bits, public Marsaglia
        {
        public:
            //! full range spanning bits
            inline explicit MarsagliaBits() : bits( 0xffffffff ), Marsaglia() {}
            //! destructor
            inline virtual ~MarsagliaBits() throw() {}
            //! use selected generator
            inline virtual uint32_t next32() throw() { return (*this.*G)(); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MarsagliaBits);
        };

        typedef MarsagliaBits<& Marsaglia::kiss > Kiss32; //!< Keep it Simple Stupid 32 bits


    }
}

#endif

