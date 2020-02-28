
//! \file

#ifndef Y_INFORMATION_QENCODER_INCLUDED
#define Y_INFORMATION_QENCODER_INCLUDED 1

#include "y/information/iobits.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Information {

        //! encoding to IObits
        class QEncoder : public IOBits::Broker
        {
        public:
            typedef arc_ptr<QEncoder> Pointer; //!< alias
            virtual ~QEncoder() throw();       //!< cleanup
            virtual void writeBits(IOBits &, const char ) = 0; //!< emit and update
            virtual void flushBits(IOBits &)              = 0; //!< send EOS

        protected:
            explicit QEncoder() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QEncoder);
        };

        //! default writer
        class QWriter : public QEncoder
        {
        public:
            static const char CLID[];
            explicit QWriter() throw();                    //!< setup
            virtual ~QWriter() throw();                    //!< cleanup
            virtual void        startBits() throw();              //!< do nothing
            virtual void        writeBits(IOBits &, const char ); //!< push char
            virtual void        flushBits(IOBits &);              //!< do nothing
            virtual const char *label() const throw();            //!< CLID
        private:
            Y_DISABLE_COPY_AND_ASSIGN(QWriter);
        };
    }

}

#endif

