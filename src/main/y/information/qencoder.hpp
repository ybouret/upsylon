
//! \file

#ifndef Y_INFORMATION_QENCODER_INCLUDED
#define Y_INFORMATION_QENCODER_INCLUDED 1

#include "y/information/iobits.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Information {

        class qencoder : public IOBits::Broker
        {
        public:
            typedef arc_ptr<qencoder> pointer;
            virtual ~qencoder() throw();
            virtual void writeBits(IOBits &, const char ) = 0;
            virtual void flushBits(IOBits &)              = 0;

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
            virtual void startBits() throw();
            virtual void writeBits(IOBits &, const char );
            virtual void flushBits(IOBits &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(qwriter);
        };
    }

}

#endif

