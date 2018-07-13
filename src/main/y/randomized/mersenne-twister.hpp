//! \file
#ifndef Y_RANDOMIZED_MERSENNE_TWISTER_INCLUDED
#define Y_RANDOMIZED_MERSENNE_TWISTER_INCLUDED 1

#include "y/randomized/bits.hpp"

namespace upsylon
{
    namespace randomized
    {
        class mersenne_twister : public bits
        {
        public:
            explicit mersenne_twister() throw();
            virtual ~mersenne_twister() throw();

            virtual uint32_t next32() throw();
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

