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
            enum Event
            {
                Success, //!< a char was read
                Partial, //!< need more bits
                Stopped  //!< found EOS
            };
            typedef arc_ptr<QDecoder> Pointer; //!< alias
            virtual ~QDecoder() throw();       //!< cleanup

            virtual Event   queryBits(char &C, IOBits &) = 0;


        protected:
            explicit QDecoder() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QDecoder);
        };

        class QReader : public QDecoder
        {
        public:
            explicit QReader() throw();
            virtual ~QReader() throw();

            virtual void  startBits() throw() {}
            virtual Event queryBits(char &C, IOBits &Q)
            {
                if( Q.size() < 8 ) return Partial; else
                {
                    C = Q.pop<char>();
                    return Success;
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(QReader);
        };
    }

}

#endif

