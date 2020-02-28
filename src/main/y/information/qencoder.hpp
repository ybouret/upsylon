
//! \file

#ifndef Y_INFORMATION_QENCODER_INCLUDED
#define Y_INFORMATION_QENCODER_INCLUDED 1

#include "y/information/iobits.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Information {

        //! encoding to IObits
        class qencoder : public IOBits::Broker
        {
        public:
            typedef arc_ptr<qencoder> pointer; //!< alias
            virtual ~qencoder() throw();       //!< cleanup
            virtual void writeBits(IOBits &, const char ) = 0; //!< emit and update
            virtual void flushBits(IOBits &)              = 0; //!< send EOS

        protected:
            explicit qencoder() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(qencoder);
        };

        //! default writer
        class qwriter : public qencoder
        {
        public:
            explicit qwriter() throw();                    //!< setup
            virtual ~qwriter() throw();                    //!< cleanup
            virtual void startBits() throw();              //!< do nothing
            virtual void writeBits(IOBits &, const char ); //!< push char
            virtual void flushBits(IOBits &);              //!< do nothing
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(qwriter);
        };
    }

}

#endif

