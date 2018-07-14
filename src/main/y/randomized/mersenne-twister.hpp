//! \file
#ifndef Y_RANDOMIZED_MERSENNE_TWISTER_INCLUDED
#define Y_RANDOMIZED_MERSENNE_TWISTER_INCLUDED 1

#include "y/randomized/bits.hpp"

namespace upsylon
{
    namespace randomized
    {
        //! Mersenne's Twister 32 bits generator
        class mersenne_twister : public bits
        {
        public:
            //! initialize
            explicit mersenne_twister() throw();
            //! desctructor
            virtual ~mersenne_twister() throw();

            //! bits interface
            virtual uint32_t next32() throw();

            //! regenerate
            virtual void     reseed(bits &b) throw();

        private:
            int           mti;
            unsigned long mt[624];
            Y_DISABLE_COPY_AND_ASSIGN(mersenne_twister);
            void initialize(const unsigned long) throw();
        };
    }
}

#endif

