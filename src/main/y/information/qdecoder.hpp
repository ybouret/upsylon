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
            typedef arc_ptr<QDecoder> Pointer; //!< alias
            virtual ~QDecoder() throw();       //!< cleanup


        protected:
            explicit QDecoder() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QDecoder);
        };

    }

}

#endif

