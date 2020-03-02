//! \file

#ifndef Y_INFORMATION_QDECODER_INCLUDED
#define Y_INFORMATION_QDECODER_INCLUDED 1

#include "y/information/iobits.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Information {

        //! decoding fron IObits
        class QDecoder : public IOBits::Broker
        {
        public:
            //! reading status
            enum Status
            {
                Success, //!< a char was read
                Partial, //!< need more bits
                Stopped  //!< found EOS
            };
            typedef arc_ptr<QDecoder> Pointer; //!< alias
            virtual ~QDecoder() throw();       //!< cleanup

            virtual Status   queryBits(char &C, IOBits &) = 0; //!< try to get a new char


        protected:
            explicit QDecoder() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QDecoder);
        };


        //! reading from IOBits
        class QReader : public QDecoder
        {
        public:
            static const char CLID[];   //!< QReader
            explicit QReader() throw(); //!< setup
            virtual ~QReader() throw(); //!< cleanup

            virtual void        startBits() throw();           //!< do nothing
            virtual Status      queryBits(char &C, IOBits &Q); //!< success if more than 8 bits
            virtual const char *label() const throw();         //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QReader);
        };
    }

}

#endif

