//! \file
#ifndef Y_INFORMATION_DELTA_INCLUDED
#define Y_INFORMATION_DELTA_INCLUDED 1

#include "y/information/modulation.hpp"

namespace upsylon {

    namespace information {

        //! delta modulation base class
        class delta_modulation : public modulation
        {
        public:
            virtual        ~delta_modulation() throw(); //!< cleanup
            virtual void    reset()            throw(); //!< set last=0

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
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            explicit            delta_encoder()      throw(); //!< initialize
            virtual            ~delta_encoder()      throw(); //!< destructor
            virtual uint8_t     fetch(const uint8_t) throw(); //!< encode a byte
            virtual modulation *clone()                const; //!< a new delta with same last

        private:
            Y_DISABLE_COPY_AND_ASSIGN(delta_encoder);
        };

        //! delta decoder method
        class delta_decoder : public delta_modulation
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            explicit            delta_decoder()      throw(); //!< initialize
            virtual            ~delta_decoder()      throw(); //!< destructor
            virtual uint8_t     fetch(const uint8_t) throw(); //!< decode a byte
            virtual modulation *clone()                const; //!< a new delta with same last

        private:
            Y_DISABLE_COPY_AND_ASSIGN(delta_decoder);
        };

    }
}

#endif

