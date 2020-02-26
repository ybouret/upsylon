
//! \file

#ifndef Y_INFORMATION_QENCODER_INCLUDED
#define Y_INFORMATION_QENCODER_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace information {

        class qencoder : public qbits::broker
        {
        public:
            typedef arc_ptr<qencoder> pointer;
            virtual ~qencoder() throw();
            virtual void writeBits(qbits &, const char ) = 0;
            virtual void flushBits(qbits &)              = 0;

        protected:
            explicit qencoder() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(qencoder);
        };
        
        class qwriter : public qencoder
        {
        public:
            explicit qwriter() throw();
            virtual ~qwriter() throw();
            virtual void writeBits(qbits &, const char );
            virtual void flushBits(qbits &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(qwriter);
        };
    }

}

#endif

