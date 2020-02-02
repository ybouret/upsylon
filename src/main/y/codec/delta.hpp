//! \file
#ifndef Y_CODEC_DELTA_INCLUDED
#define Y_CODEC_DELTA_INCLUDED 1

#include "y/codec/shaker.hpp"

namespace upsylon {

    namespace information {

        //! delta modulation base class
        class delta_modulation : public shaker
        {
        public:
            virtual      ~delta_modulation() throw(); //!< cleanup
            virtual  void reset()            throw(); //!< set last=0

        protected:
            explicit delta_modulation() throw(); //!< setup
            uint8_t  last; //!< last encoded/decoded bytes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(delta_modulation);
        };

        //! delta encoder
        class delta_encoder : public delta_modulation
        {
        public:
            explicit        delta_encoder()      throw(); //!< initialize
            virtual        ~delta_encoder()      throw(); //!< destructor
            virtual uint8_t fetch(const uint8_t) throw(); //!< encode a byte

        private:
            Y_DISABLE_COPY_AND_ASSIGN(delta_encoder);
        };

        //! delta decoder method
        class delta_decoder : public delta_modulation
        {
        public:
            explicit        delta_decoder()      throw(); //!< initialize
            virtual        ~delta_decoder()      throw(); //!< destructor
            virtual uint8_t fetch(const uint8_t) throw(); //!< decode a byte

        private:
            Y_DISABLE_COPY_AND_ASSIGN(delta_decoder);
        };

    }
}

#endif

